//
// Created by foxfire on 2018-05-05.
//

#ifndef ASTEROIDS5_SPACE_OBJECT_HPP
#define ASTEROIDS5_SPACE_OBJECT_HPP

#define _USE_MATH_DEFINES
#include <Eigen/Core>

/**
 * @class space_object
 * @breif The base class of all space objects
 */
class space_object
{
public:
	space_object(){}
	virtual ~space_object(){}

	/**
	 * Object position
	 */
	Eigen::Vector2f pos;
	/**
	 * Object velocity
	 */
	Eigen::Vector2f vel;
	/**
	 * The circle radius used for collision detection
	 */
	float radius;
};


#endif //ASTEROIDS5_SPACE_OBJECT_HPP
