#include "world.hpp"

//------------------------------------------------------------------------------
world::world(events::manager_interface *emi, const std::string &data_root)
{
	this->emi = emi;
	this->data_root = data_root;
}

//------------------------------------------------------------------------------
world::~world()
{
}

