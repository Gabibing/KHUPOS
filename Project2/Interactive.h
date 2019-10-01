#pragma once
class Interactive
{
public:
	int x;
	int y;
	int width;
	int height;
	Function *func;
	Interactive(int x_, int y_, int width_, int height_, Function *func_);
	Interactive(Graphical *g, Function *func_);
	void setFunction(Function* func_);
};
