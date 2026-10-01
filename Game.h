#pragma once
#include "Box.h"
#include "Ball.h"
#include <vector>
#include <string>

class Game
{
	Ball ball;
	Box paddle;

	// Store multiple bricks
	std::vector<Box> bricks;

	// Game paused (win/lose) state and message
	bool paused = false;
	std::string endMessage;

public:
	Game();
	bool Update();
	void Render() const;
	void Reset();
	void ResetBall();
	void CheckCollision();
};