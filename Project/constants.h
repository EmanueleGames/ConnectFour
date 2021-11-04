#pragma once

//Window resolution
const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

//Intro Margins
const int INTRO_TOP_MARGIN     = 600;
const int INTRO_P1_ARROW_MARGIN = 280;
const int INTRO_P2_ARROW_MARGIN = 520;
const int INTRO_P1_LEFT_MARGIN = INTRO_P1_ARROW_MARGIN + 10;
const int INTRO_P2_LEFT_MARGIN = INTRO_P2_ARROW_MARGIN + 10;

//Play Margins
const int LEFT_RIGHT_MARGIN = (WINDOW_WIDTH - 700) / 2;
const int TOP_MARGIN = WINDOW_HEIGHT - 600;
const int TURN_P1_CENTER = (WINDOW_WIDTH - 700) / 4;
const int TURN_P2_CENTER = WINDOW_WIDTH - TURN_P1_CENTER;

//FPS limit
const int MAX_FPS = 60;
const int MAX_TICKS_PER_FRAME = 1000 / MAX_FPS;
const int FPS_DECIMAL_DIGIT = 2;

//Disks and Hints size
const int SIZE = 100;

//DELAY for CPU move and Game Over
const int DELAY = 40;

//Game states
enum class eGameStates
{
    STATE_NULL,
    STATE_INTRO,
    STATE_PLAY,
    STATE_MATCH_OVER,
    STATE_EXIT,
};

//Game Mode
enum class eGameMode
{
    ONE_PLAYER,     // Player vs CPU
    TWO_PLAYER      // Player vs Player
};

//Players Turns
enum class ePlayer
{
    PLAYER_CPU,
    PLAYER_ONE,
    PLAYER_TWO,
    GAME_OVER
};

//Winner
enum class eWinner
{
    WINNER_CPU,
    WINNER_ONE,
    WINNER_TWO,
    NO_WINNER
};