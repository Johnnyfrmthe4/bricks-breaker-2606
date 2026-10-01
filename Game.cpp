#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	Reset();
}

void Game::Reset()
{
	Console::SetWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Console::CursorVisible(false);
	paddle.width = 12;
	paddle.height = 2;
	paddle.x_position = 32;
	paddle.y_position = 30;

	ball.visage = 'O';
	ball.color = ConsoleColor::Cyan;
	ResetBall();

	// TODO #2 - Add this brick and 4 more bricks to the vector
	// Add 5 bricks to the vector, evenly spaced across the window
	const int numBricks = 5;
	bricks.clear();
	for (int i = 0; i < numBricks; ++i)
	{
		Box b;
		b.width = 10;
		b.height = 2;
		int totalWidth = numBricks * b.width;
		int gap = (WINDOW_WIDTH - totalWidth) / (numBricks + 1);
		b.x_position = gap + i * (b.width + gap);
		b.y_position = 5;
		b.doubleThick = true;
		b.color = ConsoleColor::DarkGreen;
		b.hits = 0;
		bricks.push_back(b);
	}

	// Reset any paused/win/lose state
	paused = false;
	endMessage.clear();
}

void Game::ResetBall()
{
	ball.x_position = paddle.x_position + paddle.width / 2;
	ball.y_position = paddle.y_position - 1;
	ball.x_velocity = rand() % 2 ? 1 : -1;
	ball.y_velocity = -1;
	ball.moving = false;
}

bool Game::Update()
{
	if (GetAsyncKeyState(VK_ESCAPE) & 0x1)
		return false;

	if (GetAsyncKeyState(VK_RIGHT) && paddle.x_position < WINDOW_WIDTH - paddle.width)
		paddle.x_position += 2;

	if (GetAsyncKeyState(VK_LEFT) && paddle.x_position > 0)
		paddle.x_position -= 2;

	if (!paused && (GetAsyncKeyState(VK_SPACE) & 0x1))
		ball.moving = !ball.moving;

	if (GetAsyncKeyState('R') & 0x1)
		Reset();

	if (!paused && ball.moving)
		ball.Update();

	CheckCollision();
	return true;
}

//  All rendering, including text, should occur in the Render function
void Game::Render() const
{
	Console::Lock(true);
	Console::Clear();
	
	paddle.Draw();
	ball.Draw();

	// TODO #3 - Update render to render all bricks
	// Render all bricks
	for (const auto &b : bricks)
		b.Draw();

	// If paused (win/lose), show centered message
	if (paused && !endMessage.empty())
	{
		int x = (WINDOW_WIDTH - (int)endMessage.size()) / 2;
		int y = WINDOW_HEIGHT / 2;
		Console::SetCursorPosition(x, y);
		std::cout << endMessage;
	}

	Console::Lock(false);
}

void Game::CheckCollision()
{
	// TODO #4 - Update collision to check all bricks
	// Check collision against all bricks
	for (size_t i = 0; i < bricks.size(); ++i)
	{
		if (bricks[i].Contains(ball.x_position + ball.x_velocity, ball.y_position + ball.y_velocity))
		{
			// register hit
			bricks[i].hits++;
			// darken color
			bricks[i].color = ConsoleColor(bricks[i].color - 1);

			// remove brick after 3 hits
			if (bricks[i].hits >= 3)
			{
				bricks.erase(bricks.begin() + i);
			}

			// bounce ball
			ball.y_velocity *= -1;
			// TODO #5 - If the ball hits the same brick 3 times (color == black), remove it from the vector
			// If no bricks remain, player wins
			if (bricks.empty())
			{
				ball.moving = false;
				paused = true;
				endMessage = "You win! Press 'R' to play again.";
			}

			break; // handle one collision per update
		}
	}
	// TODO #6 - If no bricks remain, pause ball and display (render) victory text with R to reset
	// If no bricks remain, handled above in collision


	if (paddle.Contains(ball.x_position + ball.x_velocity, ball.y_velocity + ball.y_position))
	{
		ball.y_velocity *= -1;
	}
	// TODO #7 - If ball touches bottom of window, pause ball and display (render) defeat text with R to reset
	// If ball touches bottom of window, player loses
	if (ball.y_position + ball.y_velocity >= WINDOW_HEIGHT - 1)
	{
		ball.moving = false;
		paused = true;
		endMessage = "You lose. Press 'R' to play again.";
	}
}
