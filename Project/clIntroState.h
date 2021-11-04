#pragma once

#include "clGameState.h"

//Derived Class: Intro State
class clIntroState : public clGameState
{
    private:

        //Title screen resources and variables
        TTF_Font* mFontTitle;
        clTexture mTextTitle;
        TTF_Font* mFontSmaller;
        clTexture mTextAuthor;
        clTexture mTextOption;
        clTexture mTextArrow;

        bool mFadeOut;
        int mOffset;
        Uint8 mTextAlpha;

    public:

        //Load State assets
        clIntroState();
        //Free State assets
        ~clIntroState();

        //Game loop functions
        void handle_events();
        void logic();
        void render();
};