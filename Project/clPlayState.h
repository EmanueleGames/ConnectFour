#pragma once
#include <time.h>

#include "clGameState.h"
#include "clDiskSlot.h"
#include "clColumnHint.h"

//Derived Class: Play State
class clPlayState : public clGameState
{
    private:

        //Playing screen resources and variables
        clTexture    mGrid;
        clDiskSlot   mSlotGrid[6][7];
        clColumnHint mColumnHint[7];

        //Turn indicators
        TTF_Font*  mFontTurn;
        clTexture  mTextTurn;
        int mTurnOffsetX;
        int mTurnSpriteIndex;
        Uint32 ticksCounter;

        //Sounds
        Mix_Chunk* mDiskSound;
        Mix_Chunk* mErrorSound;
        Mix_Chunk* mVictorySound;

        //Play functions
        bool checkGameOver(eWinner &winner,int winningSlotsXY[]);
        bool lookForMove(eSlotStatus diskColor, int& column_selected, int& preemptive_column);
        bool last_slot_available(const int row, const int col);
        bool check_preemptive_move(const int row, const int col);

    public:

        //Load State assets
        clPlayState();
        //Free State assets
        ~clPlayState();

        //Main loop functions
        void handle_events();
        void logic();
        void render();
};
