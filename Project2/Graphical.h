#pragma once
class Graphical
{
public:
	// bit flag
	int type; // 1: �׸�, 2: text, 3: �׸�+�ؽ�Ʈ, 4: Rectangle
	int x;
	int y;
	int width;
	int height;
	
	// Sprite member
	Gdiplus::Image* spriteSet;

	// Rectangle member
	Gdiplus::Pen *rectPen;
	Gdiplus::SolidBrush *rectBrush;

	// Text member
	std::wstring text;
	Gdiplus::SolidBrush *textBrush;
	Gdiplus::Font *font;
	Gdiplus::StringFormat *textFormat;

	// ������
	Graphical(int x_, int y_, int width_, int height_, Gdiplus::Image* spriteSet_);
	Graphical(int x_, int y_, int width_, int height_, std::wstring text_, int size_ = 20);
	Graphical(int x_, int y_, int width_, int height_, std::wstring text_, Gdiplus::Color color, int size_ = 20);
	Graphical(int x_, int y_, int width_, int height_, Gdiplus::Color color, Gdiplus::REAL thickness = 1.0f);

	// ���� �Լ���
	void setTextColor(Gdiplus::Color color);
	void setText(std::wstring text_);
	void setTextAlign(Gdiplus::StringAlignment alignmnet);

	void setRectColor(Gdiplus::Color color);

	void drawRectangle(Gdiplus::Color color, Gdiplus::REAL thickness = 1.0f);
	void fillRectangle(Gdiplus::Color color);
};