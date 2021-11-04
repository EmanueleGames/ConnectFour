#include "clIntroState.h"

using namespace std;

bool getGameMode(int myX, int myY, int& offset);

clIntroState::clIntroState()
{
    mFadeOut = false;
    mOffset = INTRO_P1_ARROW_MARGIN;
    mTextAlpha = 255;

    //Open Title font
    mFontTitle = TTF_OpenFont("data/jabjai_light.ttf", 110);
    if (mFontTitle == NULL)
    {
        cout << "Failed to load jabjai_light font for Title screen! SDL_ttf Error: " << TTF_GetError() << endl;
    }

    string messageIntro = "Connect Four!";
    if (!mTextTitle.createFromText(gRenderer, mFontTitle, messageIntro.c_str(), textColorBlack))
    {
        cout << "Unable to render Intro message texture!" << endl;
    }
    mTextTitle.setBlendMode(SDL_BLENDMODE_BLEND); //ready to fade out

    //Open Author and Option font
    mFontSmaller = TTF_OpenFont("data/jabjai_light.ttf", 40);
    if (mFontSmaller == NULL)
    {
        cout << "Failed to load jabjai_light small font for Title screen! SDL_ttf Error: " << TTF_GetError() << endl;
    }

    string messageAuthor = "by Emanuele Carrino";
    if (!mTextAuthor.createFromText(gRenderer, mFontSmaller, messageAuthor.c_str(), textColorBlack))
    {
        cout << "Unable to render Intro message texture!" << endl;
    }
    mTextAuthor.setBlendMode(SDL_BLENDMODE_BLEND); //ready to fade out

    string messageOption = "1 Player       2 Player";
    if (!mTextOption.createFromText(gRenderer, mFontSmaller, messageOption.c_str(), textColorBlack))
    {
        cout << "Unable to render Option message texture!" << endl;
    }
    mTextOption.setBlendMode(SDL_BLENDMODE_BLEND); //ready to fade out

    string messageArrow = ">";
    if (!mTextArrow.createFromText(gRenderer, mFontSmaller, messageArrow.c_str(), textColorBlack))
    {
        cout << "Unable to render Arrow texture!" << endl;
    }
    mTextArrow.setBlendMode(SDL_BLENDMODE_BLEND); //ready to fade out
}

clIntroState::~clIntroState()
{
    //Free the resources
    TTF_CloseFont(mFontTitle);
    TTF_CloseFont(mFontSmaller);
    mFontTitle = NULL;
    mFontSmaller = NULL;
    mTextTitle.freeTexture();
    mTextAuthor.freeTexture();
    mTextOption.freeTexture();
    mTextArrow.freeTexture();
}

void clIntroState::handle_events()
{
    //Polling for a new event
    while (SDL_PollEvent(&gEvent))
    {
        //User requests to exit
        if (gEvent.type == SDL_QUIT)
        {
            set_next_state(eGameStates::STATE_EXIT);
        }
        //Key press
        else if ((gEvent.type == SDL_KEYDOWN))
        {
            if (gEvent.key.keysym.sym == SDLK_RIGHT || gEvent.key.keysym.sym == SDLK_LEFT)
            {
                if (gPlayerNumber == eGameMode::ONE_PLAYER)
                {
                    gPlayerNumber = eGameMode::TWO_PLAYER;
                    mOffset = INTRO_P2_ARROW_MARGIN;
                }
                else
                {
                    gPlayerNumber = eGameMode::ONE_PLAYER;
                    mOffset = INTRO_P1_ARROW_MARGIN;
                }
            }

            if (gEvent.key.keysym.sym == SDLK_RETURN)    // user confirms option
            {
                //Move to the Play screen
                //set_next_state(eGameStates::STATE_PLAY);  //removed to realize fade out
                if (mFadeOut == false)
                {
                    mFadeOut = true;
                }
            }
        }

        //Mouse Click and Mouse Over
        if (gEvent.type == SDL_MOUSEMOTION || gEvent.type == SDL_MOUSEBUTTONDOWN)
        {
            //Get mouse position
            int xMouse, yMouse;
            SDL_GetMouseState(&xMouse, &yMouse);

            if(getGameMode(xMouse, yMouse, mOffset))
            {
                switch (gEvent.type)
                {
                //Mouse Over
                case SDL_MOUSEMOTION:
                    break;

                //Mouse Click
                case SDL_MOUSEBUTTONDOWN:
                    //Move to the Play screen
                    //set_next_state(eGameStates::STATE_PLAY);  //removed to realize fade out
                    if (mFadeOut == false)
                    {
                        mFadeOut = true;
                    }
                    break;
                }
            }
        }
    }
}

void clIntroState::logic()
{
    //Calculate create texture for FPS
    calc_and_texture_FPS();

    //Fade out
    if (mFadeOut == true)
    {
        mTextAlpha = mTextAlpha - 5;
    }
    if (mTextAlpha == 0)
    {
        //Move to the Play screen
        set_next_state(eGameStates::STATE_PLAY);
    }
}

void clIntroState::render()
{
    //Clear the window
    SDL_SetRenderDrawColor(gRenderer, 200, 200, 200, 255);  //almost white
    SDL_RenderClear(gRenderer);

    //fade out
    mTextTitle.setAlphaModulation(mTextAlpha); 
    mTextAuthor.setAlphaModulation(mTextAlpha);
    mTextOption.setAlphaModulation(mTextAlpha);
    mTextArrow.setAlphaModulation(mTextAlpha);

    //Average FPS
    gFPSTextTexture.render(gRenderer, 2, 0);
    //Show the Title
    mTextTitle.render(gRenderer, (WINDOW_WIDTH - mTextTitle.getWidth()) / 2, 200);
    mTextAuthor.render(gRenderer, (WINDOW_WIDTH - mTextAuthor.getWidth()) / 2, 310);
    //Show Options
    mTextOption.render(gRenderer, (WINDOW_WIDTH - mTextOption.getWidth()) / 2, 600);
    mTextArrow.render(gRenderer, mOffset, 600 );
    

}

bool getGameMode(int myX, int myY, int& offset)
{
    bool valid_position = false;

    // If the mouse is the correct region (Y-axis)
    if (myY > INTRO_TOP_MARGIN &&
        myY < INTRO_TOP_MARGIN + 35)
    {
        // ONE Player region (X-axis)
        if (myX > INTRO_P1_LEFT_MARGIN &&
            myX < INTRO_P1_LEFT_MARGIN + 190)
        {
            gPlayerNumber = eGameMode::ONE_PLAYER;
            offset = INTRO_P1_ARROW_MARGIN;
            valid_position = true;
        }
        // TWO Player region (X-axis)
        else if (myX > INTRO_P2_LEFT_MARGIN &&
                 myX < INTRO_P2_LEFT_MARGIN + 190)
        {
            gPlayerNumber = eGameMode::TWO_PLAYER;
            offset = INTRO_P2_ARROW_MARGIN;
            valid_position = true;
        }
    }

    return valid_position;
}