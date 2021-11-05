#include <iostream>

#include "globals.h"
#include "clIntroState.h"
#include "clPlayState.h"
#include "clMatchOverState.h"

using namespace std;

//Gaming window and window renderer
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Game status variables
eGameStates  gActualState  = eGameStates::STATE_NULL;
eGameStates  gNextState    = eGameStates::STATE_NULL;
clGameState* ptrCurrentState = NULL;
//Game parameters
eGameMode gPlayerNumber = eGameMode::ONE_PLAYER;
ePlayer   gPlayerTurn   = ePlayer::PLAYER_ONE;
eWinner   gPlayerWinner = eWinner::NO_WINNER;

//The event structure
SDL_Event gEvent;

// FPS timer and text
clTimer   gCalcFPStimer;
clTexture gFPSTextTexture;
TTF_Font* gFontFPS;
float gAvgFPS;
int   gCountedFrames;
int   gDecimalDigitValue;
stringstream gTextFPS;

const SDL_Color textColorBlack = { 0, 0, 0, 255 };

/*************** FUNCTIONS ***************/

bool SDL_initialize()
{

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		cout << "SDL failed to initialize! SDL Error: " << SDL_GetError() << endl;
		return false;
	}

	//Set texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		cout << "Warning: Linear texture filtering not enabled!" << endl;
	}

	//Create Game Window
	gWindow = SDL_CreateWindow("Connect Four", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		cout << "Window creation failed! SDL Error: " << SDL_GetError() << endl;
		return false;
	}

	//Create Renderer for the window (vsync OFF), FPS capped manually
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		cout << "Renderer creation failed! SDL Error: " << SDL_GetError() << endl;
		return false;
	}

	//PNG loading initialization (SDL_image)
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		cout << "SDL_image failed to initialize! SDL_image Error: " << IMG_GetError() << endl;
		return false;
	}

	//Font loading initialization (SDL_ttf)
	if (TTF_Init() == -1)
	{
		cout << "SDL_ttf failed to initialize! SDL_ttf Error: " << TTF_GetError() << endl;
		return false;
	}

	//Audio loading initialization (SDL_mixer)
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "SDL_mixer failed to initialize! SDL_mixer Error: " << Mix_GetError() << endl;
		return false;
	}

	//Everything was fine
	return true;
}

void SDL_close()
{
	/*
	//Free loaded images
	gBoxSpriteSheetTexture.freeTexture();

	//Free loaded text textures
	gFPSTextTexture.freeTexture();
	gPlayerTextTexture.freeTexture();

	//Free global font
	TTF_CloseFont(gFontFPS);
	TTF_CloseFont(gFontPlayer);
	gFontFPS = NULL;
	gFontPlayer = NULL;

	//Free the sound effects
	Mix_FreeChunk(gSymbolSound);
	Mix_FreeChunk(gVictorySound);
	gSymbolSound = NULL;
	gVictorySound = NULL;
	*/

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
	
}

bool load_Game_Data()
{
	//Success flag
	bool fl_success = true;

	//Open the fonts
	gFontFPS = TTF_OpenFont("data/Quicksand.ttf", 12);
	if (gFontFPS == NULL)
	{
		cout << "Failed to load Quicksand font! SDL_ttf Error: " << TTF_GetError() << endl;
		fl_success = false;
	}

	//FPS counter initialization
	gDecimalDigitValue = 1;
	for (int i = 0; i < FPS_DECIMAL_DIGIT; i++)
	{
		gDecimalDigitValue *= 10;
	}
	float gAvgFPS = 0;
	int gCountedFrames = 0;

	return fl_success;
	
}

void set_next_state(eGameStates newState)
{
	//Only set next state if the user hasn't requested to exit
	if (gNextState != eGameStates::STATE_EXIT)
	{
		gNextState = newState;
	}
}

void change_state()
{
	//If next state is a valid one
	if (gNextState != eGameStates::STATE_NULL)
	{
		//If we need to exit, we don't delete the current state
		if (gNextState != eGameStates::STATE_EXIT)
		{
			delete ptrCurrentState;
		}

		//Change the state
		switch (gNextState)
		{
		case eGameStates::STATE_INTRO:
			ptrCurrentState = new clIntroState();
			break;
		case eGameStates::STATE_PLAY:
			ptrCurrentState = new clPlayState();
			break;
		case eGameStates::STATE_MATCH_OVER:
			ptrCurrentState = new clMatchOverState();
			break;
		}

		//update the actual anx next state
		gActualState = gNextState;
		gNextState = eGameStates::STATE_NULL;
	}
}

float roundDown(float number, const int value)
{
	// EXAMPLE with 2 digits:
	// 12.3456 * 100 = 1234.56 
	// 1234.56 + .5 = 1235.06		for rounding off value 
	// then type cast to int so value is 1235
	// then divided by 100 so the value converted into 12.35 

	number = (int)(number * value + .5);
	return (number / value);
}

void calc_and_texture_FPS()
{
	gAvgFPS = gCountedFrames / (gCalcFPStimer.getTicks() / 1000.f);
	if (gAvgFPS > 2000000)
	{
		gAvgFPS = 0;
	}
	else
	{
		gAvgFPS = roundDown(gAvgFPS, gDecimalDigitValue);
	}

	//FPS text
	gTextFPS.str("");
	gTextFPS << "Average FPS (vsync OFF):  " << gAvgFPS;
	if (!gFPSTextTexture.createFromText(gRenderer, gFontFPS, gTextFPS.str().c_str(), textColorBlack))
	{
		cout << "Unable to render FPS texture!" << endl;
	}
}