#ifndef GFX_HPP
#define GFX_HPP

#include <string>
#include <SDL2/SDL.h>

#include "events/observer.hpp"

namespace fox
{
class counter;
}

class gfx : public events::observer
{
public:
	gfx(events::manager_interface *emi);
	virtual ~gfx();
	
	void init(int w, int h, const std::string &data_root);
	void render();
	void resize(int w, int h);

	/// this is the message handler from events::observer
	void process_messages(events::message_base *e){}

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
	/**
	* @brief Root directory to load data from
	*/
	std::string data_root;

	unsigned int framerate;
	fox::counter *fps_counter;
	double fps_time;
	int frames;

	void update_fps();
};


#endif //GFX_HPP
