#include "clMatchOverState.h"

using namespace std;

clMatchOverState::clMatchOverState()
{
    string messageWinner;
    SDL_Color textColor = { 0, 0, 0, 255 }; //black

    //Match Over font
    mFontMatchOver = TTF_OpenFont("data/jabjai_light.ttf", 55);
    if (mFontMatchOver == NULL)
    {
        cout << "Failed to load jabjai_light font for MatchOver screen! SDL_ttf Error: " << TTF_GetError() << endl;
    }

    string messageMatchOver = "Match Over!";
    if (!mTextMatchOver.createFromText(gRenderer, mFontMatchOver, messageMatchOver.c_str(), textColor))
    {
        cout << "Unable to create MatchOver message texture!" << endl;
    }

    string messagePlayAgain = "Press any key to play again!";
    if (!mTextPlayAgain.createFromText(gRenderer, mFontMatchOver, messagePlayAgain.c_str(), textColor))
    {
        cout << "Unable to create PlayAgain message texture!" << endl;
    }
    
    if (gPlayerWinner == eWinner::WINNER_ONE)
    {
        messageWinner = "Winner: Player 1 (Yellow)";
        textColor = { 160, 160, 0, 255 };   //yellow
    }
    else if (gPlayerWinner == eWinner::WINNER_TWO)
    {
        messageWinner = "Winner: Player 2 (Red)";
        textColor = { 200, 0, 0, 255 };     //red
    }
    else if (gPlayerWinner == eWinner::WINNER_CPU)
    {
        messageWinner = "Winner: CPU (Red)";
        textColor = { 200, 0, 0, 255 };     //red
    }
    else    //No winner
    {
        messageWinner = "Draw!";
        textColor = { 120, 120, 120, 255 }; //grey
    }

    if (!mTextWinner.createFromText(gRenderer, mFontMatchOver, messageWinner.c_str(), textColor))
    {
        cout << "Unable to create Winner message texture!" << endl;
    }
}

clMatchOverState::~clMatchOverState()
{
    //Free the resources
    TTF_CloseFont(mFontMatchOver);
    mFontMatchOver = NULL;

    mTextMatchOver.freeTexture();
    mTextWinner.freeTexture();
    mTextPlayAgain.freeTexture();
}

void clMatchOverState::handle_events()
{
    //Polling for a new event
    while (SDL_PollEvent(&gEvent))
    {
        //User requests to exit
        if (gEvent.type == SDL_QUIT)
        {
            set_next_state(eGameStates::STATE_EXIT);
        }
        //If the user press any key
        else if ((gEvent.type == SDL_KEYDOWN) || (gEvent.type == SDL_MOUSEBUTTONDOWN))
        {
            //Move to the Play screen
            gPlayerTurn = ePlayer::PLAYER_ONE;
            set_next_state(eGameStates::STATE_INTRO);
        }
    }   
}

void clMatchOverState::logic()
{
    //Calculate create texture for FPS
    calc_and_texture_FPS();
}

void clMatchOverState::render()
{
    //Clear the window (light blue color)
    SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);  //almost white
    SDL_RenderClear(gRenderer);
    //Average FPS
    gFPSTextTexture.render(gRenderer, 2, 0);
    //Show the Match result
    mTextMatchOver.render(gRenderer, (WINDOW_WIDTH - mTextMatchOver.getWidth()) / 2, 200);
    mTextWinner.render(gRenderer, (WINDOW_WIDTH - mTextWinner.getWidth()) / 2, 300);
    mTextPlayAgain.render(gRenderer, (WINDOW_WIDTH - mTextPlayAgain.getWidth()) / 2, 600);
}