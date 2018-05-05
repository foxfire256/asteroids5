//
// Created by foxfire on 2018-05-05.
//

#ifndef ASTEROIDS5_ASTEROID_HPP
#define ASTEROIDS5_ASTEROID_HPP

#include "space_object.hpp"

class asteroid : public space_object
{
public:
	asteroid();

	/**
	 * Minimum speed in pixels per second
	 */
	static const float speed_min;
	/**
	* Maximum speed in pixels per second
	*/
	static const float speed_max;
	/**
	* Speed in pixels per second
	*/
	float speed;
	static const float init_radius;
	/**
	 * The direction in radians where this thing is going
	 */
	float direction;
};


#endif //ASTEROIDS5_ASTEROID_HPP
