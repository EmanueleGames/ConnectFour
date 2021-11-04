#pragma once

#include "constants.h"
#include "globals.h"
#include "clTexture.h"

extern clTexture gDiskSpriteSheetTexture;
extern SDL_Rect gDiskSpriteClips[3];

enum class eSlotStatus {
	SLOT_EMPTY			= 0,
	SLOT_YELLOW_DISK	= 1,
	SLOT_RED_DISK		= 2,
	SLOT_GREEN_DISK		= 3,
	SLOT_STATUS_COUNT	= 4
};

class clDiskSlot
{
	public:
		//Constructor
		clDiskSlot();
		//Destructor
		//~clDiskSlot();

		// operator == overload
		friend bool operator==(const clDiskSlot& s1, const clDiskSlot& s2);

		//Sets top left position
		void setCurrentSprite(const eSlotStatus sprite);
		void setPosition(const int x, const int y);
		void setUsed(const bool isUsed);
		void resetSlot();

		//Get functions
		bool isUsed() const;
		eSlotStatus getCurrentSprite() const;

		//render
		void render();

	private:

		//Currently showed sprite
		eSlotStatus mCurrentSlot;
		//Top left corner
		SDL_Point mSlotTopLeftCorner;
		//Flag indicating if the slot has been claimed by any player
		bool used;
};