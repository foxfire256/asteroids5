#include "gfx.hpp"

#include <iostream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/range/iterator_range.hpp>

#include "fox/counter.hpp"

//------------------------------------------------------------------------------
gfx::gfx(events::manager_interface *emi) : events::observer(emi)
{
	renderer = nullptr;
	window = nullptr;
}

//------------------------------------------------------------------------------
gfx::~gfx()
{
	emi->unsubscribe_all(this);
	
	if(renderer != nullptr)
		SDL_DestroyRenderer(renderer);
	if(window != nullptr)
		SDL_DestroyWindow(window);
	
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

//------------------------------------------------------------------------------
void gfx::render()
{
	SDL_RenderClear(renderer);

	frames++;

	SDL_RenderPresent(renderer);

	update_fps();
}

//------------------------------------------------------------------------------
void gfx::resize(int w, int h)
{
	win_w = w;
	win_h = h;
}

//------------------------------------------------------------------------------
void gfx::init(int w, int h, const std::string &data_root)
{
	std::string window_name = "Asteroids 5";
	win_w = w;
	win_h = h;
	this->data_root = data_root;

	fps_counter = new fox::counter();
	frames = 0;
	framerate = 0;
	fps_time = 0.0f;
	
	int ret = SDL_InitSubSystem(SDL_INIT_VIDEO);
	if(ret < 0)
	{
		printf("Unable to init SDL Video: %s\n", SDL_GetError());
		exit(1);
	}
	
	// find some info about the graphics system being used
	int num_drivers = SDL_GetNumRenderDrivers();
	// the driver index to use
	int driver_index = -1;
	std::cout << "Available SDL Rendering back ends:\n";
	for(int i = 0; i < num_drivers; i++)
	{
		SDL_RendererInfo info;
		int ret = SDL_GetRenderDriverInfo(i, &info);
		if(ret != 0)
		{
			const char* error = SDL_GetError();
			std::cerr << "SDL Error: " << error << std::endl;
			continue;
		}

		// some options are: direct3d, direct3d11, opengl, opengles2, software
		#if _WIN32 || _WIN64
		// direct3d11 seems to be the best on Windows 10 and NVidia
		if(std::string(info.name) == "direct3d11")
			driver_index = i;
		#elif __linux__
		if(std::string(info.name) == "opengl")
			driver_index = i;
		#endif
		std::cout << "SDL Renderer[" << i <<"] name: " << info.name << std::endl;
	}

	// WARNING: OpenGL was disabled since this code uses the SDL renderer
#ifdef __ANDROID__
	window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
	window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
	#elif TARGET_IPHONE_SIMULATOR
		// iOS Simulator
	#elif TARGET_OS_MAC
	// Other kinds of Mac OS
	window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
			SDL_WINDOW_RESIZABLE);
#else
		// Unsupported platform
	#endif
#else
	window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, win_w, win_h,
			SDL_WINDOW_RESIZABLE);
#endif

	if(window == nullptr)
	{
		printf("Couldn't create window: %s\n", SDL_GetError());
		exit(-1);
	}
	else
	{
		screen = SDL_GetWindowSurface(window);
		// on Windows 10 at least this returns NULL
		renderer = SDL_GetRenderer(window);
		if(renderer == nullptr) // not created yet?
		{
			// use -1 for driver_index to get the first one that supports what
			// we want
			renderer = SDL_CreateRenderer(window, driver_index,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
			if(renderer == nullptr)
			{
				// just try to create anything if that didn't work
				renderer = SDL_CreateRenderer(window, -1, 0);
				if(renderer == nullptr)
				{
					std::cerr << "Failed to create renderer!\n";
					std::cerr << SDL_GetError() << "\n";
					exit(-1);
				}
			}
		}
	}
	
	SDL_RendererInfo info;
	ret = SDL_GetRendererInfo(renderer, &info);
	if(ret == 0)
	{
		std::cout << "Active SDL_Renderer name: " << info.name << std::endl;
	}
	else
	{
		const char* error = SDL_GetError();
		std::cerr << "Failed to get renderer info!\n";
		std::cerr << "SDL Error: " << error << std::endl;
	}
	
	// 1 is linear, 2 is anisotropic filtering
#if _WIN32 || _WIN64
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
#else
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
#endif
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_ShowWindow(window);
	
	// linear texture interpolation
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
}

//------------------------------------------------------------------------------
void gfx::update_fps()
{
	frames++;
	fps_time += fps_counter->update();

	if(fps_time > 1.0f)
	{
		framerate = (unsigned int)((float)frames / fps_time);
		fps_time = 0.0f;
		frames = 0;

		//printf("FPS: %d\n", framerate);

		//emi->post_event(new events::message_framerate(framerate));
	}
}