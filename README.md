# Connect Four
<br/>
<p align="center">
  <img src="http://emanuelecarrino.altervista.org/images/portfolio/connectfour_1024x768.png" />
</p>
<br/>

## Development
**IDE:** &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Microsoft Visual Studio  
**Language:** &nbsp;&nbsp; C++  
**Libraries:** &nbsp;&nbsp;&nbsp;&nbsp; SDL2 (Simple DirectMedia Layer)
<br/>
> Connect Four videogame.  
> Multiple scene implemented with Object Oriented State Machine:  
> inheritance, polymorphism, operator overloading.  
> Game has 1 Player mode (player vs AI) and 2 player mode (Player vs Player).  
> AI is tweaked to give player a bit of a challenge.  
> VSync dibabled, FPS are capped manually in the game loop  
> Multiple source files. 1024x768 pixels resolution.

## How AI works  
During his turn the AI will check for the following condition in this order:  
1. If there is a winning move, the AI will win.  
2. If there is a winning move for the player, the AI will block it.  
3. If there is a move that will let the player win on his next turn, the AI will avoid it.  
4. If none of the above condition verifies, the AI will make a random move.
<br/>

## Features
* Triple scene game: title, game, match over
* 1 or 2 players mode
* Mouse control
* Basic sounds
* VSync disabled
* Average FPS counter

<br/>
<br/>

`Code is a little over-commented just to help anyone interested to navigate it better`
