#include "clColumnHint.h"

clTexture gHintSpriteSheetTexture;
SDL_Rect gHintSpriteClips[2];

clColumnHint::clColumnHint()
{
	mHintTopLeftCorner.x = 0;
	mHintTopLeftCorner.y = 0;

	mCurrentHint = eHint::NO_INDICATOR;
}

void clColumnHint::setPosition(int x, int y)
{
	mHintTopLeftCorner.x = x;
	mHintTopLeftCorner.y = y;
}

void clColumnHint::setCurrentHint(eHint hint)
{
	mCurrentHint = hint;
}

void clColumnHint::reset()
{
	mHintTopLeftCorner.x = 0;
	mHintTopLeftCorner.y = 0;

	mCurrentHint = eHint::NO_INDICATOR;
}

void clColumnHint::render()
{
	if (mCurrentHint != eHint::NO_INDICATOR)
	{
		gHintSpriteSheetTexture.render(gRenderer, mHintTopLeftCorner.x, mHintTopLeftCorner.y, &gHintSpriteClips[(int)mCurrentHint - 1]);
	}
}