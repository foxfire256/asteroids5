#include "world.hpp"

#define _USE_MATH_DEFINES
#include <cmath>

#include <Eigen/Core>

#include "fox/counter.hpp"
#include "bullet.hpp"
#include "asteroid.hpp"
#include "ship.hpp"

//------------------------------------------------------------------------------
world::world(events::manager_interface *emi, const std::string &data_root,
	int width, int height)
{
	this->emi = emi;
	this->data_root = data_root;
	this->width = width;
	this->height = height;
	this->generator = std::mt19937_64(std::random_device{}());

	player_ship = nullptr;
	main_counter = nullptr;
}

//------------------------------------------------------------------------------
world::~world()
{
	deinit();
}

//------------------------------------------------------------------------------
void world::update()
{
	float dt = main_counter->update();

	for(asteroid *a : asteroids)
	{
		a->pos = a->pos + dt * a->vel;
		if(a->pos[0] > (float)width)
			a->pos[0] -= (float)width;
		if(a->pos[1] > (float)height)
			a->pos[1] -= (float)height;
	}
}

//------------------------------------------------------------------------------
void world::init()
{
	std::uniform_real_distribution<float> dist(asteroid::speed_min, asteroid::speed_max);
	asteroid *a = new asteroid();
	a->speed = dist(generator);
	dist = std::uniform_real_distribution<float>(0.0f, (float)M_PI);
	a->direction = dist(generator);
	a->vel[0] = cosf(a->direction);
	a->vel[1] = sinf(a->direction);
	a->vel = a->vel * a->speed;
	dist = std::uniform_real_distribution<float>(0.0f, (float)width);
	a->pos[0] = dist(generator);
	dist = std::uniform_real_distribution<float>(0.0f, (float)height);
	a->pos[1] = dist(generator);
	a->radius = a->init_radius;
	asteroids.push_back(a);

	player_ship = new ship();
	player_ship->radius = 16.0f;
	for(int i = 0; i < 100; i++)
	{
		dist = std::uniform_real_distribution<float>(0.0f, (float)width);
		player_ship->pos[0] = dist(generator);
		dist = std::uniform_real_distribution<float>(0.0f, (float)height);
		player_ship->pos[1] = dist(generator);

		Eigen::Vector2f d = player_ship->pos - a->pos;
		
		if(d.norm() > 2.0f * a->radius)
			break;
	}

	main_counter = new fox::counter();
}

//------------------------------------------------------------------------------
void world::deinit()
{
	for(asteroid *a : asteroids)
	{
		delete a;
	}
	asteroids.clear();

	for(bullet *b : bullets)
	{
		delete b;
	}
	bullets.clear();

	delete player_ship;
	delete main_counter;
}
