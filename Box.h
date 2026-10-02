#pragma once
#include "BaseObject.h"

class Box : public BaseObject
{
public:
	bool doubleThick = false;
	int width = 2;
	int height = 2;
	// Number of times this brick has been hit
	int hits = 0;
	void Draw() const override;
	bool Contains(int x, int y);
};