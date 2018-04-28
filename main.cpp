#include "gfx/gfx.hpp"

int main(int argc, char ** argv)
{
	gfx *g = new gfx();
	g->init();

	for(;;)
		g->render();
	
	delete g;
	return 0;
}
