#ifndef WORLD_HPP
#define WORLD_HPP

#include "events/observer.hpp"

class world : public events::observer
{
public:
	/**
	* @brief The only constructor for this class
	* @param emi The events manager interface
	* @param data_root The location where all of the data lives
	*/
	world(events::manager_interface *emi, const std::string &data_root);
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

private:
	std::string data_root;
};

#endif
