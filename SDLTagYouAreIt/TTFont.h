#ifndef TTFONT_H
#define TTFONT_H

#include "StandardIncludes.h"

class Renderer;

class TTFont
{
public:
	// COnstructors/Destructors
	TTFont();
	// Week 9 Code for Input Core
	virtual ~TTFont();

	// Methods
	void Initialize(int pointSize);
	void Write(SDL_Renderer* _renderer, const char* _text, SDL_Color _color, SDL_Point _pos);
	void Shutdown();

private:
	// Methods
	TTF_Font* m_font;
	SDL_Rect destRect;
};

#endif // !TTFONT_H
