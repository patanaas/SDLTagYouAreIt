// Week 9 Code for Input Core
#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "StandardIncludes.h"

class Keyboard;

//class Mouse;
//class Controller;

class InputController : public Singleton<InputController>
{
public:
	// Contructors/Destructors
	InputController();
	virtual ~InputController();

	// Accessors
	Keyboard* KB() { return m_keyboard; }

private:
	Keyboard* m_keyboard;

};

#endif // !INPUTCONTROLLER_H
