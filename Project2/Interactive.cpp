#include "framework.h"

Interactive::Interactive(int x_, int y_, int width_, int height_, Function *func_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	func = func_;
}

Interactive::Interactive(Graphical * g, Function * func_)
{
	x = g->x;
	y = g->y;
	width = g->width;
	height = g->height;
	func = func_;
}

void Interactive::setFunction(Function * func_)
{
	Function* old = func;
	func = func_;
	delete old;
}
