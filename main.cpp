#include <string>
#include <iostream>

#include <boost/version.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/token_functions.hpp>

#include "gfx/gfx.hpp"
#include "events/manager.hpp"
#include "main_observer.hpp"
#include "events/console_writer.hpp"
#include "sdl_message_handler.hpp"

int main(int argc, char ** argv)
{
	events::manager *em = nullptr;
	main_observer *mo = nullptr;
	events::console_writer *ecw = nullptr;
	sdl_message_handler *smh = nullptr;
	gfx *g = nullptr;

	int done;
	int win_w, win_h;

	// set some defaults
	win_w = 1024;
	win_h = 1024;

#if defined(__ANDROID__)
	std::string data_root = "/sdcard/asteroids5";
#elif defined(__APPLE__)
	std::string data_root = "/Users/foxfire/dev/asteroids5";
#elif defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64)
	std::string data_root = "C:/dev/asteroids5";
#else // Linux
	std::string data_root = "/home/foxfire/dev/asteroids5";
#endif

	std::cout << "Using Boost "
		// major version
		<< BOOST_VERSION / 100000
		<< "."
		// minor version
		<< BOOST_VERSION / 100 % 1000
		<< "."
		// patch level
		<< BOOST_VERSION % 100
		<< std::endl;

	namespace po = boost::program_options;

	// parse command line
	po::options_description desc("Command line options");
	po::variables_map vm;

	desc.add_options()
		("help,h", "produce help message")
		("width", po::value<int>(), "set window width")
		("height", po::value<int>(), "set window height")
		("data-root", po::value<std::string>(), "set data directory")
		;

	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help"))
	{
		std::cout << desc << "\n";
		return 1;
	}
	if(vm.count("width"))
	{
		win_w = vm["width"].as<int>();
	}
	if(vm.count("height"))
	{
		win_h = vm["height"].as<int>();
	}
	if(vm.count("data-root"))
	{
		data_root = vm["data-root"].as<std::string>();
	}

	// create an events manager first
	em = new events::manager();

	// write info to the command line
	ecw = new events::console_writer(em);

	// get the observer going for main
	done = 0;
	mo = new main_observer(em, &done);

	// start SDL
	SDL_Init(0);

	std::cout << "Running on platform: " << SDL_GetPlatform() << std::endl;
	std::cout << "Number of logical CPU cores: " << SDL_GetCPUCount() << std::endl;
	int ram_mb = SDL_GetSystemRAM();
	char buffer[8];
	snprintf(buffer, 8, "%.1f", ram_mb / 1024.0f);
	std::cout << "System RAM " << ram_mb << "MB (" << buffer << " GB)\n";
	
	g = new gfx(em);
	g->init(win_w, win_h, data_root);
	smh = new sdl_message_handler(em);
	fflush(stdout);

	// main loop
	while(!done)
	{
		smh->pump_messages();

		em->pump_events();

		g->render();
	}

	delete smh;
	delete g;
	//delete sp;
	delete mo;
	delete ecw;
	delete em;

	SDL_Quit();

	return 0;
}
