#ifndef WORLD_HPP
#define WORLD_HPP

#define _USE_MATH_DEFINES

#include "events/observer.hpp"

#include <random>
#include <list>

class space_object;

class world : public events::observer
{
public:
	/**
	* @brief The only constructor for this class
	* @param emi The events manager interface
	* @param data_root The location where all of the data lives
	*/
	world(events::manager_interface *emi, const std::string &data_root, int width, int height);
	/**
	* @brief Destructor
	*/
	virtual ~world();

	/**
	* @brief No default constructor
	*/
	world() = delete;

	/// this is the message handler from events::observer
	void process_messages(events::message_base *e){}

	void init();
	void deinit();

private:
	std::string data_root;
	int width, height;
	/**
	* @brief A random generator that is initialized in the constructor
	*/
	std::mt19937_64 generator;

	std::list<space_object *> asteroids;
	std::list<space_object *> bullets;
	space_object *player_ship;
};

#endif
