#include "framework.h"

// Image
Graphical::Graphical(int x_, int y_, int width_, int height_, Gdiplus::Image* spriteSet_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	spriteSet = spriteSet_;
	type = type | 1;
}


// Text
Graphical::Graphical(int x_, int y_, int width_, int height_, std::wstring text_, int size_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	text = text_;
	font = new Gdiplus::Font(L"¸¼Àº °íµñ", size_, Gdiplus::FontStyleBold, Gdiplus::Unit::UnitPixel);
	textBrush = new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0));
	textFormat = new Gdiplus::StringFormat();
	type = type | 2;
}

// Text + Color
Graphical::Graphical(int x_, int y_, int width_, int height_, std::wstring text_, Gdiplus::Color color, int size_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	text = text_;
	font = new Gdiplus::Font(L"¸¼Àº °íµñ", size_, Gdiplus::FontStyleBold, Gdiplus::Unit::UnitPixel);
	textBrush = new Gdiplus::SolidBrush(color);
	textFormat = new Gdiplus::StringFormat();
	type = type | 2;
}

// Rectangle
Graphical::Graphical(int x_, int y_, int width_, int height_, Gdiplus::Color color, Gdiplus::REAL thickness)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
	rectPen = new Gdiplus::Pen(color, thickness);
	type = type | 4;
}

void Graphical::setTextColor(Gdiplus::Color color)
{
	Gdiplus::SolidBrush *old = textBrush;
	textBrush = new Gdiplus::SolidBrush(color);
	delete old;
}

void Graphical::setRectColor(Gdiplus::Color color)
{
	Gdiplus::Pen *old = rectPen;
	if (old != nullptr)
	{
		rectPen = new Gdiplus::Pen(color, old->GetWidth());
		delete old;
	}
	else
		rectPen = new Gdiplus::Pen(color);
	type = type | 4;
}

void Graphical::setText(std::wstring text_)
{
	text = text_;
	if (font == nullptr)
		font = new Gdiplus::Font(L"¸¼Àº °íµñ", 20, Gdiplus::FontStyleBold, Gdiplus::Unit::UnitPixel);
	if (textBrush == nullptr)
		textBrush = new Gdiplus::SolidBrush(Gdiplus::Color(0, 0, 0));
	if (textFormat == nullptr)
		textFormat = new Gdiplus::StringFormat();
	type = type | 2;
}

void Graphical::setTextAlign(Gdiplus::StringAlignment alignmnet)
{
	Gdiplus::StringFormat *old = textFormat;
	textFormat = new Gdiplus::StringFormat();
	textFormat->SetAlignment(alignmnet);
	delete old;
}


void Graphical::drawRectangle(Gdiplus::Color color, Gdiplus::REAL thickness)
{
	Gdiplus::Pen *old = rectPen;
	rectPen = new Gdiplus::Pen(color, thickness);
	delete old;
	type = type | 4;
}

void Graphical::fillRectangle(Gdiplus::Color color)
{
	Gdiplus::SolidBrush *old = rectBrush;
	rectBrush = new Gdiplus::SolidBrush(color);
	delete old;
	type = type | 8;
}
