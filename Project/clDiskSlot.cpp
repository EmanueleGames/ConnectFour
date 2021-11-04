#include "clDiskSlot.h"

clTexture gDiskSpriteSheetTexture;
SDL_Rect gDiskSpriteClips[3];

clDiskSlot::clDiskSlot()
{
	mSlotTopLeftCorner.x = 0;
	mSlotTopLeftCorner.y = 0;
	used = false;

	mCurrentSlot = eSlotStatus::SLOT_EMPTY;
}

void clDiskSlot::setCurrentSprite(const eSlotStatus sprite)
{
	mCurrentSlot = sprite;
}

void clDiskSlot::setPosition(const int x, const int y)
{
	mSlotTopLeftCorner.x = x;
	mSlotTopLeftCorner.y = y;
}

void clDiskSlot::setUsed(const bool isUsed)
{
	used = isUsed;
}

bool clDiskSlot::isUsed() const
{
	return used;
}

eSlotStatus clDiskSlot::getCurrentSprite() const
{
	return mCurrentSlot;
}

void clDiskSlot::resetSlot()
{
	used = false;
	mCurrentSlot = eSlotStatus::SLOT_EMPTY;
}

void clDiskSlot::render()
{
	if (used)
	{
		gDiskSpriteSheetTexture.render(gRenderer, mSlotTopLeftCorner.x, mSlotTopLeftCorner.y, &gDiskSpriteClips[(int)mCurrentSlot - 1]);
	}
}

bool operator==(const clDiskSlot& s1, const clDiskSlot& s2)
{
	if (s1.mCurrentSlot == s2.mCurrentSlot && s1.used && s2.used)
	{
		return true;
	}
	return false;
}