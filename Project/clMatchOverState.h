#pragma once

#include "clGameState.h"

class clMatchOverState : public clGameState
{
    private:

        //Match Over screen resources and variables
        TTF_Font* mFontMatchOver;
        clTexture mTextMatchOver;
        clTexture mTextWinner;
        clTexture mTextPlayAgain;

    public:

        //Load State assets
        clMatchOverState();
        //Free State assets
        ~clMatchOverState();

        //Game loop functions
        void handle_events();
        void logic();
        void render();
};

