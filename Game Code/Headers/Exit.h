#pragma once
#pragma once

//Project Includes
#include "GridObject.h"

class Exit : public GridObject
{
private:

	enum exittypes
	{
		TOP,
		TOPLEFT,
		TOPRIGHT,
		BOTTOMLEFT,
		BOTTOMRIGHT,
		BOTTOM
	};

public:

	Exit();

	void setExitType();

private:

	exittypes exitType;

};