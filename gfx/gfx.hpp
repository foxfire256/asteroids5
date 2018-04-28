#ifndef GFX_HPP
#define GFX_HPP

#include <string>
#include <SDL2/SDL.h>

namespace fox
{
class counter;
}

class gfx
{
public:
	gfx();
	virtual ~gfx();
	
	void init();
	void render();
	void resize(int w, int h);

private:
	/**
	 * @brief Window dimensions
	 */
	int win_w, win_h;
	/**
	 * @brief The SDL window
	 */
	SDL_Window *window;
	/**
	 * @brief The SDL screen
	 */
	SDL_Surface *screen;
	/**
	 * @brief The SDL renderer
	 */
	SDL_Renderer *renderer;

	unsigned int framerate;
	fox::counter *fps_counter;
	double fps_time;
	int frames;

	void update_fps();
};


#endif //GFX_HPP
