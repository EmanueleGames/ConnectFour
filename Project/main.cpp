#include <iostream>

#include "constants.h"
#include "globals.h"
#include "clTexture.h"
#include "clTimer.h"
#include "clGameState.h"
#include "clIntroState.h"
#include "clPlayState.h"
#include "clMatchOverState.h"

using namespace std;

int main(int argc, char* argv[])
{
    //SLD components initialization
    if (!SDL_initialize())
    {
        cout << "SDL initialization Failed!" << endl;
        return -1;
    }

    //Load game assets
    if (!load_Game_Data())
    {
        cout << "Failed to load media!" << endl;
        return -1;
    }

    //Game starts in Intro State
    gActualState = eGameStates::STATE_INTRO;
    //Set the current game state object
    ptrCurrentState = new clIntroState();

    clTimer capFPStimer;

    //Start the FPS calculator timer
    gCountedFrames = 0;
    gCalcFPStimer.start();

    //While the user hasn't quit
    while (gActualState != eGameStates::STATE_EXIT)
    {
        //Start timer to limit FPS
        capFPStimer.start();

        //Handle events on queue
        ptrCurrentState->handle_events();

        //Handle the state logic
        ptrCurrentState->logic();

        //Change state if needed
        change_state();

        //Render current state
        ptrCurrentState->render();

        //Switch front and back buffer
        SDL_RenderPresent(gRenderer);

        //Limit the FPS
        if (capFPStimer.getTicks() < MAX_TICKS_PER_FRAME) //the frame took less ticks than expected
        {
            SDL_Delay(MAX_TICKS_PER_FRAME - capFPStimer.getTicks()); //wait
        }
        gCountedFrames++;
    }

    //Free memory and close SDL
    SDL_close();

    return 0;

}