// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 2 & Lab 4
// 10-30-2018
//
// Vedant Chaudhari, 1532077
// John Malvey
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

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