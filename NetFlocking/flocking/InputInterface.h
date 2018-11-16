#pragma once

#include "SDL.h"

class InputInterface {
private:
	static InputInterface* mpInstance;

	InputInterface();

	SDL_Event mPrevEvent;
	SDL_Event mCurrEvent;

	const Uint8* mpVKeyState;
	int mNumKeys;

	void onKeyDown();
	void onKeyUp();

public:
	static InputInterface* getInstance();

	bool isKeyDown(SDL_Scancode key);
	bool isKeyUp(SDL_Scancode key);

	void update();
};