#ifndef BASICSTRUCTS_H
#define BASICSTRUCTS_H

#ifndef _WIN32
#include <Windows.h>
#else
typedef unsigned char byte;
#endif // !WIN_32

#include "SpriteSheet.h"

struct Warrior {
	float x;
	float y;
	float speed;
	float direction;
	float animationSpeed;
	bool isDead;
	bool isAnimatingDeath;
	float deathTimer;
	SpriteSheet* animation;

	Warrior(float x = 0, float y = 0) : x(x), y(y) {}
};

struct Color
{
	Color(byte _r, byte _g, byte _b, byte _a)
	{
		R = _r;
		G = _g;
		B = _b;
		A = _a;
	}

	byte R;
	byte G;
	byte B;
	byte A;
};

// Code for Rendering Primitives
struct Point
{
	/*Point(unsigned int _x, unsigned int _y)
	{
		X = _x;
		Y = _y;
	}

	unsigned int X;
	unsigned int Y;*/

	Point() : X(0), Y(0) {}  // Default constructor initializing X and Y to 0
	Point(unsigned int _x, unsigned int _y) : X(_x), Y(_y) {}  // Parameterized constructor

	unsigned int X;
	unsigned int Y;

	/*Point() : X(0), Y(0) {}
	Point(float xPos, float yPos) : X(xPos), Y(yPos) {}*/
};

struct Rect
{
	Rect(unsigned int _x1, unsigned int _y1, unsigned int _x2, unsigned int _y2)
	{
		X1 = _x1;
		Y1 = _y1;
		X2 = _x2;
		Y2 = _y2;
	}

	unsigned int X1;
	unsigned int Y1;
	unsigned int X2;
	unsigned int Y2;
};

struct Position
{
	Position()
	{
		X = 0;
		Y = 0;
	}

	Position(int _x, int _y)
	{
		X = _x;
		Y = _y;
	}

	int X;
	int Y;
};

#endif // !BASICSTRUCTS_H

