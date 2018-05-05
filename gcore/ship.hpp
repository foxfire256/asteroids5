//
// Created by foxfire on 2018-05-05.
//

#ifndef ASTEROIDS5_SHIP_HPP
#define ASTEROIDS5_SHIP_HPP

#include "space_object.hpp"

class ship : public space_object
{
public:
	float speed_max;
	float speed;
};


#endif //ASTEROIDS5_SHIP_HPP
