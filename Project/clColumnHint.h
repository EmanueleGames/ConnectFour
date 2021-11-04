#pragma once

#include "constants.h"
#include "globals.h"
#include "clTexture.h"

enum class eHint {
    NO_INDICATOR = 0,
    ICON_ARROW   = 1,
    ICON_X       = 2,
};

extern clTexture gHintSpriteSheetTexture;
extern SDL_Rect gHintSpriteClips[2];

class clColumnHint
{
    public:
        //Constructor
        clColumnHint();
        //Destructor
        //~clColumnHint();
    
        //Set functions
        void setCurrentHint(eHint);
        void setPosition(int x, int y);
        void reset();

        //render
        void render();

    private:

        //Currently showed sprite
        eHint mCurrentHint;
        //Top left corner
        SDL_Point mHintTopLeftCorner;

};

