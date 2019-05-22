#pragma once

//Project Includes
#include "GridObject.h"

class Exit : public GridObject
{
public:

	enum exittypes
	{
		TOP,
		TOPRIGHT,
		LEFT,
		RIGHT,
		BOTTOMLEFT,
		BOTTOMRIGHT,
		BOTTOM
	};

public:

	Exit();

	void setExitType(char _ch);
	exittypes getExitType();


private:

	exittypes exitType;

};