#include "clPlayState.h"

using namespace std;

int getPointedColumn(int myX, int myY);

clPlayState::clPlayState()
{
    if (!mGrid.createFromFile(gRenderer, "data/c4_grid.png"))
    {
        cout << "Grid image loading failed!" << endl;
    }
	else
	{
		//grid color set to blue
		mGrid.setColorModulation(140, 140, 255);
	}

	//Set up disks and hints sprites
	if (!gDiskSpriteSheetTexture.createFromFile(gRenderer, "data/disks.png"))
	{
		cout << "Disks sprites loading failed!" << endl;
	}
	if (!gHintSpriteSheetTexture.createFromFile(gRenderer, "data/hints.png"))
	{
		cout << "Hints sprites loading failed!" << endl;
	}

	//Set sprite clips for DISKS
	for (int i = 0; i < 3; i++)
	{
		gDiskSpriteClips[i].x = i * SIZE;
		gDiskSpriteClips[i].y = 0;
		gDiskSpriteClips[i].w = SIZE;
		gDiskSpriteClips[i].h = SIZE;
	}
	//Set sprite clips for HINTS
	for (int i = 0; i < 2; i++)
	{
		gHintSpriteClips[i].x = i * SIZE;
		gHintSpriteClips[i].y = 0;
		gHintSpriteClips[i].w = SIZE;
		gHintSpriteClips[i].h = SIZE;
	}

	//Slots position initialization
	int topLeftX, topLeftY;
	for (int i = 0; i < 6; i++)
	{
		topLeftY = TOP_MARGIN + i * 100;
		for (int j = 0; j < 7; j++)
		{
			topLeftX = LEFT_RIGHT_MARGIN + j * 100;
			mSlotGrid[i][j].setPosition(topLeftX, topLeftY);
		}
	}
	//Hints position initialization
	for (int i = 0; i < 7; i++)
	{
		mColumnHint[i].setPosition(LEFT_RIGHT_MARGIN + i * 100, TOP_MARGIN - 100);
	}

	//Turn indicator font and variables
	mFontTurn = TTF_OpenFont("data/jabjai_light.ttf", 35);
	if (mFontTurn == NULL)
	{
		cout << "Failed to load jabjai_light font for MatchOver screen! SDL_ttf Error: " << TTF_GetError() << endl;
	}

	string messageTurn = "Player 1";
	if (!mTextTurn.createFromText(gRenderer, mFontTurn, messageTurn.c_str(), textColorBlack))
	{
		cout << "Unable to create MatchOver message texture!" << endl;
	}
	mTurnOffsetX = TURN_P1_CENTER;
	mTurnSpriteIndex = 0;

	//Load sound effects
	mDiskSound = Mix_LoadWAV("data/plastic_pop_1.wav");
	if (mDiskSound == NULL)
	{
		cout << "Failed to load Disk sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
	}
	else
	{
		Mix_VolumeChunk(mDiskSound, 40);
	}
	
	mErrorSound = Mix_LoadWAV("data/generic_error.wav");
	if (mErrorSound == NULL)
	{
		cout << "Failed to load Error sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
	}
	else
	{
		Mix_VolumeChunk(mErrorSound, 10);
	}

	mVictorySound = Mix_LoadWAV("data/victory.wav");
	if (mVictorySound == NULL)
	{
		cout << "Failed to load Victory sound effect! SDL_mixer Error: " << Mix_GetError() << endl;
	}
	else
	{
		Mix_VolumeChunk(mVictorySound, 5);
	}
	
	ticksCounter = 0;
}

clPlayState::~clPlayState()
{
    //Free the resources
    mGrid.freeTexture();

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			mSlotGrid[i][j].resetSlot();
		}
	}

	//Hints position initialization
	for (int i = 0; i < 7; i++)
	{
		mColumnHint[i].reset();
	}

	mTextTurn.freeTexture();
	TTF_CloseFont(mFontTurn);
	mFontTurn = NULL;

	Mix_FreeChunk(mDiskSound);
	Mix_FreeChunk(mErrorSound);
	Mix_FreeChunk(mVictorySound);
	mDiskSound = NULL;
	mErrorSound = NULL;
	mVictorySound = NULL;
}

void clPlayState::handle_events()
{
    //Polling for a new event
    while (SDL_PollEvent(&gEvent))
    {
        //User requests to exit
        if (gEvent.type == SDL_QUIT)
        {
            set_next_state(eGameStates::STATE_EXIT);
        }

		// reset all hints
		for (int i = 0; i < 7; i++)
		{
			mColumnHint[i].setCurrentHint(eHint::NO_INDICATOR);
		}

		//Catch Mouse Click and Mouse Over events
		if (gEvent.type == SDL_MOUSEMOTION || gEvent.type == SDL_MOUSEBUTTONDOWN || gEvent.type == SDL_MOUSEBUTTONUP)
		{
			//Get mouse position
			int xMouse, yMouse;
			SDL_GetMouseState(&xMouse, &yMouse);

			//columnPointed is the index of the selected column
			int columnPointed = getPointedColumn(xMouse,yMouse);

			if (columnPointed != -1)
			{
				switch (gEvent.type)
				{

				//Mouse Over
				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONUP:
					//column not full
					if (!(mSlotGrid[0][columnPointed].isUsed()))
					{
						mColumnHint[columnPointed].setCurrentHint(eHint::ICON_ARROW);
					}
					//column full
					else
					{
						mColumnHint[columnPointed].setCurrentHint(eHint::ICON_X);
					}
					break;

				//Mouse Click
				case SDL_MOUSEBUTTONDOWN:
					//column full
					if ((mSlotGrid[0][columnPointed].isUsed()))
					{
						Mix_PlayChannel(-1, mErrorSound, 0);
					}
					else
					{
						// find lowest free slot
						int i = 5;
						while (i >= 0 && mSlotGrid[i][columnPointed].isUsed())
							{ i--; }
						
						if (gPlayerTurn == ePlayer::PLAYER_ONE)
						{
							mSlotGrid[i][columnPointed].setCurrentSprite(eSlotStatus::SLOT_YELLOW_DISK);
							mSlotGrid[i][columnPointed].setUsed(true);
							Mix_PlayChannel(-1, mDiskSound, 0);
							
							mTurnOffsetX = TURN_P2_CENTER;
							mTurnSpriteIndex = 1;

							if (gPlayerNumber == eGameMode::ONE_PLAYER)
							{
								string messageTurn = "CPU";
								mTextTurn.createFromText(gRenderer, mFontTurn, messageTurn.c_str(), textColorBlack);
								gPlayerTurn = ePlayer::PLAYER_CPU;

								ticksCounter = 0;	// reset delay counter
							}
							else if (gPlayerNumber == eGameMode::TWO_PLAYER)
							{
								string messageTurn = "Player 2";
								mTextTurn.createFromText(gRenderer, mFontTurn, messageTurn.c_str(), textColorBlack);
								gPlayerTurn = ePlayer::PLAYER_TWO;
							}
						}

						else if (gPlayerTurn == ePlayer::PLAYER_TWO)
						{
							mSlotGrid[i][columnPointed].setCurrentSprite(eSlotStatus::SLOT_RED_DISK);
							mSlotGrid[i][columnPointed].setUsed(true);
							Mix_PlayChannel(-1, mDiskSound, 0);
							mTurnOffsetX = TURN_P1_CENTER;
							mTurnSpriteIndex = 0;
							string messageTurn = "Player 1";
							mTextTurn.createFromText(gRenderer, mFontTurn, messageTurn.c_str(), textColorBlack);
							gPlayerTurn = ePlayer::PLAYER_ONE;
						}
					}
					break;
				}
			}
		}
    }
}

void clPlayState::logic()
{
	//Calculate create texture for FPS
	calc_and_texture_FPS();

	// Game already knows that match is over
	if (gPlayerTurn == ePlayer::GAME_OVER)
	{
		if (ticksCounter > DELAY*2)
		{
			set_next_state(eGameStates::STATE_MATCH_OVER);
		}
		else
		{
			ticksCounter++;
		}
	}
	else	// check if the match is over
	{
		int winningSlotsXY[8];
		//check game over
		if (checkGameOver(gPlayerWinner, winningSlotsXY))
		{
			if (gPlayerWinner != eWinner::NO_WINNER)
			{
				mSlotGrid[winningSlotsXY[0]][winningSlotsXY[1]].setCurrentSprite(eSlotStatus::SLOT_GREEN_DISK);
				mSlotGrid[winningSlotsXY[2]][winningSlotsXY[3]].setCurrentSprite(eSlotStatus::SLOT_GREEN_DISK);
				mSlotGrid[winningSlotsXY[4]][winningSlotsXY[5]].setCurrentSprite(eSlotStatus::SLOT_GREEN_DISK);
				mSlotGrid[winningSlotsXY[6]][winningSlotsXY[7]].setCurrentSprite(eSlotStatus::SLOT_GREEN_DISK);
				Mix_PlayChannel(-1, mVictorySound, 0);
			}
			gPlayerTurn = ePlayer::GAME_OVER;
			ticksCounter = 0;
		}
	}

	//CPU turn
	if (gPlayerTurn == ePlayer::PLAYER_CPU)
	{
		if (ticksCounter >= DELAY)	// emulate CPU thinking
		{
			// CPU AI!
			int selected_column;
			int good_column  = -1;
			int avoid_column = -1;
			int attempts_counter;

			// Check if CPU can win (3 aligned red disks)
			if (!lookForMove(eSlotStatus::SLOT_RED_DISK, selected_column, good_column))
			{
				// If there is no win condition for the CPU
				// Check if PLAYER 1 needs to be blocked (3 aligned yellow disks)
				if (!lookForMove(eSlotStatus::SLOT_YELLOW_DISK, selected_column, avoid_column))
				{

					// If we can put a disk to grant us a victory condition,
					// and the same disk does not grant player a win condition, proceed
					if (good_column != -1 && good_column != avoid_column)
					{
						selected_column = good_column;
					}
					// otherwise put a disk in a RANDOM non-full column
					// avoiding the column that gives player victory condition
					else
					{
						srand(time(NULL));
						selected_column = rand() % 7;
						attempts_counter = 0;
						while (mSlotGrid[0][selected_column].isUsed() ||
							(selected_column == avoid_column && attempts_counter < 10))
						{
							// avoid deadlock with 1 or 2 column left
							if (selected_column == avoid_column)
							{
								attempts_counter++;
							}

							selected_column = rand() % 7;
						}
					}
				}
			}

			// find lowest free slot
			int i = 5;
			while (i >= 0 && mSlotGrid[i][selected_column].isUsed())
			{
				i--;
			}

			// put a disk in that slot
			mSlotGrid[i][selected_column].setCurrentSprite(eSlotStatus::SLOT_RED_DISK);
			mSlotGrid[i][selected_column].setUsed(true);
			Mix_PlayChannel(-1, mDiskSound, 0);
			mTurnOffsetX = TURN_P1_CENTER;
			mTurnSpriteIndex = 0;
			string messageTurn = "Player 1";
			mTextTurn.createFromText(gRenderer, mFontTurn, messageTurn.c_str(), textColorBlack);
			gPlayerTurn = ePlayer::PLAYER_ONE;
		}
		else
		{
			ticksCounter++;
		}
	}
}

void clPlayState::render()
{
	//Clear the window (white color)
	SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);  //almost white
	SDL_RenderClear(gRenderer);

    // Disk are rendered first to show them behind the grid
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			mSlotGrid[i][j].render();
		}
	}

	//Render hints
	for (int i = 0; i < 7; i++)
	{
		mColumnHint[i].render();
	}

	//Show the grid
	mGrid.render(gRenderer, LEFT_RIGHT_MARGIN, TOP_MARGIN);

	//Average FPS
	gFPSTextTexture.render(gRenderer, 2, 0);

	//render turn indicator elements
	mTextTurn.render(gRenderer, mTurnOffsetX - mTextTurn.getWidth() / 2, 350);
	gDiskSpriteSheetTexture.render(gRenderer, mTurnOffsetX - SIZE / 2, 400, &gDiskSpriteClips[mTurnSpriteIndex]);

}

bool clPlayState::checkGameOver(eWinner& winner, int winningSlotsXY[])
{
	bool game_over = false;

	//Horizontal - look for 4 aligned disks
	for (int i = 0; i < 6 && !game_over; i++)		// every row
	{
		for (int j = 0; j < 4 && !game_over; j++)	// columns 0-1-2-3
		{
			if (mSlotGrid[i][j] == mSlotGrid[i][j + 1] &&
				mSlotGrid[i][j] == mSlotGrid[i][j + 2] &&
				mSlotGrid[i][j] == mSlotGrid[i][j + 3])
			{
				if (mSlotGrid[i][j].getCurrentSprite() == eSlotStatus::SLOT_YELLOW_DISK)
					winner = eWinner::WINNER_ONE;
				else
				{
					if (gPlayerNumber == eGameMode::TWO_PLAYER)
						winner = eWinner::WINNER_TWO;
					else
						winner = eWinner::WINNER_CPU;
				}
				winningSlotsXY[0] = i; winningSlotsXY[1] = j;
				winningSlotsXY[2] = i; winningSlotsXY[3] = j+1;
				winningSlotsXY[4] = i; winningSlotsXY[5] = j+2;
				winningSlotsXY[6] = i; winningSlotsXY[7] = j+3;
				game_over = true;
			}
		}
	}

	//Vetical - look for 4 aligned disks
	for (int i = 0; i < 3 && !game_over; i++)		// rows 0-1-2
	{
		for (int j = 0; j < 7 && !game_over; j++)	// every column
		{
			if (mSlotGrid[i][j] == mSlotGrid[i + 1][j] &&
				mSlotGrid[i][j] == mSlotGrid[i + 2][j] &&
				mSlotGrid[i][j] == mSlotGrid[i + 3][j])
			{
				if (mSlotGrid[i][j].getCurrentSprite() == eSlotStatus::SLOT_YELLOW_DISK)
					winner = eWinner::WINNER_ONE;
				else
				{
					if (gPlayerNumber == eGameMode::TWO_PLAYER)
						winner = eWinner::WINNER_TWO;
					else
						winner = eWinner::WINNER_CPU;
				}
				winningSlotsXY[0] = i;   winningSlotsXY[1] = j;
				winningSlotsXY[2] = i+1; winningSlotsXY[3] = j;
				winningSlotsXY[4] = i+2; winningSlotsXY[5] = j;
				winningSlotsXY[6] = i+3; winningSlotsXY[7] = j;
				game_over = true;
			}
		}
	}

	//Diagonal \  - look for 4 aligned disks
	for (int i = 0; i < 3 && !game_over; i++)		// rows 0-1-2
	{
		for (int j = 0; j < 4 && !game_over; j++)	// columns 0-1-2-3
		{
			if (mSlotGrid[i][j] == mSlotGrid[i + 1][j + 1] &&
				mSlotGrid[i][j] == mSlotGrid[i + 2][j + 2] &&
				mSlotGrid[i][j] == mSlotGrid[i + 3][j + 3])
			{
				if (mSlotGrid[i][j].getCurrentSprite() == eSlotStatus::SLOT_YELLOW_DISK)
					winner = eWinner::WINNER_ONE;
				else
				{
					if (gPlayerNumber == eGameMode::TWO_PLAYER)
						winner = eWinner::WINNER_TWO;
					else
						winner = eWinner::WINNER_CPU;
				}
				winningSlotsXY[0] = i;   winningSlotsXY[1] = j;
				winningSlotsXY[2] = i+1; winningSlotsXY[3] = j+1;
				winningSlotsXY[4] = i+2; winningSlotsXY[5] = j+2;
				winningSlotsXY[6] = i+3; winningSlotsXY[7] = j+3;
				game_over = true;
			}
		}
	}

	//Diagonal /  - look for 4 aligned disks
	for (int i = 3; i < 6 && !game_over; i++)		// rows 3-4-5
	{
		for (int j = 0; j < 4 && !game_over; j++)	// columns 0-1-2-3
		{
			if (mSlotGrid[i][j] == mSlotGrid[i - 1][j + 1] &&
				mSlotGrid[i][j] == mSlotGrid[i - 2][j + 2] &&
				mSlotGrid[i][j] == mSlotGrid[i - 3][j + 3])
			{
				if (mSlotGrid[i][j].getCurrentSprite() == eSlotStatus::SLOT_YELLOW_DISK)
					winner = eWinner::WINNER_ONE;
				else
				{
					if (gPlayerNumber == eGameMode::TWO_PLAYER)
						winner = eWinner::WINNER_TWO;
					else
						winner = eWinner::WINNER_CPU;
				}
				winningSlotsXY[0] = i;   winningSlotsXY[1] = j;
				winningSlotsXY[2] = i-1; winningSlotsXY[3] = j+1;
				winningSlotsXY[4] = i-2; winningSlotsXY[5] = j+2;
				winningSlotsXY[6] = i-3; winningSlotsXY[7] = j+3;
				game_over = true;
			}
		}
	}

	if (game_over)
	{
		return true;
	}

	//full grid with no winner check
	bool full_grid = true;
	for (int i = 0; i < 7 && full_grid; i++)
	{
		if (!mSlotGrid[0][i].isUsed())
		{
			full_grid = false;
		}
	}

	if (full_grid)
	{
		winner = eWinner::NO_WINNER;
		game_over = true;
	}

	return game_over;
}

bool clPlayState::lookForMove(eSlotStatus diskColor, int& column_selected, int& preemptive_column)
{
	bool move_found = false;

	//Horizontal - look for 3 aligned disks (- O O O) or (O O O -)
	for (int i = 0; i < 6 && !move_found; i++)		// every row
	{
		for (int j = 0; j < 5 && !move_found; j++)	// columns 0-1-2-3-4
		{
			if (mSlotGrid[i][j] == mSlotGrid[i][j + 1] &&
				mSlotGrid[i][j] == mSlotGrid[i][j + 2] &&
				mSlotGrid[i][j].getCurrentSprite() == diskColor)
			{
				if (j > 0)
				{
					if (move_found = last_slot_available(i, j - 1))	//check the LEFT spot
					{
						column_selected = j - 1;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i, j - 1))
						{
							preemptive_column = j - 1;
						}
					}
				}

				if (j < 4 && !move_found)
				{
					if (move_found = last_slot_available(i, j + 3))	//check the RIGHT spot
					{
						column_selected = j + 3;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i, j + 3))
						{
							preemptive_column = j + 3;
						}
					}
				}
			}
		}
	}
	//Horizontal - look (O - O O) or (O O - O) condition
	for (int i = 0; i < 6 && !move_found; i++)		// every row
	{
		for (int j = 0; j < 4 && !move_found; j++)	// columns 0-1-2-3
		{
			if (mSlotGrid[i][j] == mSlotGrid[i][j + 3] &&
				mSlotGrid[i][j].getCurrentSprite() == diskColor)
			{
				if (mSlotGrid[i][j] == mSlotGrid[i][j + 2])			// (O - O O)
				{
					if (move_found = last_slot_available(i, j + 1))	//check the SECOND spot (from left)
					{
						column_selected = j + 1;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i, j + 1))
						{
							preemptive_column = j + 1;
						}
					}
				}
				else if (mSlotGrid[i][j] == mSlotGrid[i][j + 1])	// (O O - O)
				{
					if (move_found = last_slot_available(i, j + 2))	//check the THIRD spot (from left)
					{
						column_selected = j + 2;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i, j + 2))
						{
							preemptive_column = j + 2;
						}
					}
				}
			}
		}
	}

	//Vetical - look for 3 aligned disks
	for (int i = 1; i < 4 && !move_found; i++)		// rows 0-1-2-3
	{
		for (int j = 0; j < 7 && !move_found; j++)	// every column 
		{
			if (mSlotGrid[i][j] == mSlotGrid[i + 1][j] &&
				mSlotGrid[i][j] == mSlotGrid[i + 2][j] &&
				mSlotGrid[i][j].getCurrentSprite() == diskColor)
			{
				if (i > 0)
				{
					move_found = last_slot_available(i - 1, j);	//check the UPPER spot
					column_selected = j;
				}
			}
		}
	}

	//Diagonal \ - look for 3 aligned disks (- O O O) or (O O O -)
	for (int i = 1; i < 4 && !move_found; i++)		// rows 0-1-2-3
	{
		for (int j = 0; j < 5 && !move_found; j++)	// columns 0-1-2-3-4
		{
			if (mSlotGrid[i][j] == mSlotGrid[i + 1][j + 1] &&
				mSlotGrid[i][j] == mSlotGrid[i + 2][j + 2] &&
				mSlotGrid[i][j].getCurrentSprite() == diskColor)
			{
				if (i > 0 && j > 0)
				{
					if (move_found = last_slot_available(i - 1, j - 1))	//check the UPPER-LEFT spot
					{
						column_selected = j - 1;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i - 1, j - 1))
						{
							preemptive_column = j - 1;
						}
					}
				}

				if (i < 3 && j < 4 && !move_found)
				{
					if (move_found = last_slot_available(i + 3, j + 3))	//check the LOWER-RIGHT spot
					{
						column_selected = j + 3;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i + 3, j + 3))
						{
							preemptive_column = j + 3;
						}
					}
				}

			}
		}
	}
	//Diagonal \ - look (O - O O) or (O O - O) condition
	for (int i = 0; i < 3 && !move_found; i++)		// rows 0-1-2
	{
		for (int j = 0; j < 4 && !move_found; j++)	// columns 0-1-2-3
		{
			if (mSlotGrid[i][j] == mSlotGrid[i + 3][j + 3] &&
				mSlotGrid[i][j].getCurrentSprite() == diskColor)
			{
				if (mSlotGrid[i][j] == mSlotGrid[i + 2][j + 2])			// (O - O O)
				{
					if (move_found = last_slot_available(i + 1, j + 1))	//check the SECOND spot (from left)
					{
						column_selected = j + 1;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i + 1, j + 1))
						{
							preemptive_column = j + 1;
						}
					}
				}
				else if (mSlotGrid[i][j] == mSlotGrid[i + 1][j + 1])	// (O O - O)
				{
					if (move_found = last_slot_available(i + 2, j + 2))	//check the THIRD spot (from left)
					{
						column_selected = j + 2;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i + 2, j + 2))
						{
							preemptive_column = j + 2;
						}
					}
				}
			}
		}
	}

	//Diagonal /  - look for 3 aligned disks
	for (int i = 2; i < 6 && !move_found; i++)		// rows 2-3-4-5
	{
		for (int j = 0; j < 5 && !move_found; j++)	// columns 0-1-2-3-4
		{
			if (mSlotGrid[i][j] == mSlotGrid[i - 1][j + 1] &&
				mSlotGrid[i][j] == mSlotGrid[i - 2][j + 2] &&
				mSlotGrid[i][j].getCurrentSprite() == diskColor)
			{
				if (i < 5 && j > 0)
				{
					if (move_found = last_slot_available(i + 1, j - 1))	//check the LOWER-LEFT spot
					{
						column_selected = j - 1;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i + 1, j - 1))
						{
							preemptive_column = j - 1;
						}
					}
				}
				if (i > 2 && j < 4 && !move_found)
				{
					if (move_found = last_slot_available(i - 3, j + 3))	//check the UPPER-RIGHT spot
					{
						column_selected = j + 3;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i - 3, j + 3))
						{
							preemptive_column = j + 3;
						}
					}
				}
			}
		}
	}
	//Diagonal / - look (O - O O) or (O O - O) condition
	for (int i = 3; i < 6 && !move_found; i++)		// rows 3-4-5
	{
		for (int j = 0; j < 4 && !move_found; j++)	// columns 0-1-2-3
		{
			if (mSlotGrid[i][j] == mSlotGrid[i - 3][j + 3] &&
				mSlotGrid[i][j].getCurrentSprite() == diskColor)
			{
				if (mSlotGrid[i][j] == mSlotGrid[i - 2][j + 2])			// (O - O O)
				{
					if (move_found = last_slot_available(i - 1, j + 1))	//check the SECOND spot (from left)
					{
						column_selected = j + 1;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i - 1, j + 1))
						{
							preemptive_column = j + 1;
						}
					}
				}
				else if (mSlotGrid[i][j] == mSlotGrid[i - 1][j + 1])	// (O O - O)
				{
					if (move_found = last_slot_available(i - 2, j + 2))	//check the THIRD spot (from left)
					{
						column_selected = j + 2;
					}
					else
					{	//possibility to gain or deny a victory condition
						if (check_preemptive_move(i - 2, j + 2))
						{
							preemptive_column = j + 2;
						}
					}
				}
			}
		}
	}

	return move_found;
}

bool clPlayState::last_slot_available(const int row, const int col)
{
	// look if 4th position is available (not used)
	if (!mSlotGrid[row][col].isUsed())
	{
		// if we're on the lowest row (6th)
		if (row == 5)
		{
			return true;
		}
		else // check the lower row
		{
			if (mSlotGrid[row + 1][col].isUsed())
			{
				return true;
			}
		}
	}

	return false;
}

bool clPlayState::check_preemptive_move(const int row, const int col)
{
	// look if 4th position is available (not used)
	if (!mSlotGrid[row][col].isUsed() && row < 5)
	{
		// check if a disk can be placed to gain or deny a vicotry condition
		if (!mSlotGrid[row + 1][col].isUsed())
		{
			if (row == 4)	// if we're the 5th row, no need to check for support disk
			{
				return true;
			}
			else if (row < 4)		// we need to check if there is a supporting disk
			{
				if (mSlotGrid[row + 2][col].isUsed())
				{
					return true;
				}
			}
		}
	}

	return false;
}

int getPointedColumn(int myX, int myY)
{
	int minX = 0;
	int maxX = 0;

	// If the mouse is not over the grid
	if (myY > TOP_MARGIN)
	{
		minX = LEFT_RIGHT_MARGIN;
		maxX = LEFT_RIGHT_MARGIN + 100;

		// and is inside one of the seven column
		for (int i = 0; i < 7; i++)
		{
			if (minX < myX && myX < maxX)
			{
				return i;	//return the index of the pointed column
			}
			minX += 100;	// next column
			maxX += 100;	// next column
		}
		return -1;
	}
	return -1;
}