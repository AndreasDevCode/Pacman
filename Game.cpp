#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
	:
	mWindow(nullptr),
	mRenderer(nullptr),
	mTicksCount(0),
	mIsRunning(true), 
	mPaddleDir(0)
{
}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)",
								 100,	// Top left x-coordinate
								 100,	// Top left y-coordinates
								1024,	// Widht of window
768,	// height og window
0	// Flags(0 for no flags set)
);
// if mWindow is nullptr it failed to initialize
if (!mWindow)
{
	SDL_Log("Failed to create window: %s", SDL_GetError);
	return false;
}

mRenderer = SDL_CreateRenderer(mWindow,
	-1,	// usually -1( wich graphics to use, relevant if mutuiple windows
	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
);
if (!mRenderer)
{
	SDL_Log("Failed to create renderer: %s", SDL_GetError);
	return false;
}
mPaddlePos.x = 10.0f;
mPaddlePos.y = 768.0f / 2.0f;
mBallPos.x = 1024.0f / 2.0f;
mBallPos.y = 768.0f / 2.0f;
mBallVel.x = -200.0f;
mBallVel.y = 235.0f;

return true;
}


void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}
void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}
}
void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
	{

	}
	float dt = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	if (dt > 0.05f)
	{
		dt = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	if (!mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 300.0f * dt;
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			mPaddlePos.y = 768.0f - paddleH / 2 - thickness;
		}
	}

	mBallPos.x += mBallVel.x * dt;
	mBallPos.y += mBallVel.y * dt;
	float diff = mPaddlePos.y - mBallPos.y;
	diff = (diff > 0.0f) ? diff : -diff;
	if (
		// Our y-difference is small enough
		diff <= paddleH / 2.0f &&
		// We are in the correct x-position
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// The ball is moving to the left
		mBallVel.x < 0.0f)
		
	{
		mBallVel.x = -1.0;
	}
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;
	}
	// Did the ball collide with the right wall?
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f;
	}

	// Did the ball collide with the top wall?
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPos.y >= (768 - thickness) &&
		mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1;
	}

}
void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer,0,0,255,255);
	SDL_RenderClear(mRenderer); 
	SDL_SetRenderDrawColor(mRenderer,255,255,255,255);
	
	SDL_Rect wall{0,0,1024, thickness};
	SDL_RenderFillRect(mRenderer, &wall);
	
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	SDL_Rect paddle{ static_cast<int>(mPaddlePos.x),
			static_cast<int>(mPaddlePos.y - paddleH/2),
			thickness, static_cast<int> (paddleH) };
	SDL_RenderFillRect(mRenderer, &paddle);


	SDL_Rect ball{ static_cast<int>(mBallPos.x - thickness / 2),
					static_cast<int>(mBallPos.y - thickness / 2),
					thickness, thickness };
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_RenderPresent(mRenderer);
}