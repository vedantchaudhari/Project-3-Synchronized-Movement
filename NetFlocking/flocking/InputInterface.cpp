#include "InputInterface.h"
#include "SDLInterface.h"

#include "SDL.h"

InputInterface* InputInterface::mpInstance = NULL;

InputInterface::InputInterface() {
	mNumKeys = NULL;
}

InputInterface* InputInterface::getInstance() {
	if (mpInstance == NULL)
		mpInstance = new InputInterface();

	return mpInstance;
}

void InputInterface::onKeyDown() {
	mpVKeyState = SDL_GetKeyboardState(&mNumKeys);
}

void InputInterface::onKeyUp() {
	mpVKeyState = SDL_GetKeyboardState(&mNumKeys);
}

bool InputInterface::isKeyDown(SDL_Scancode key) {
	if (mpVKeyState == NULL)
		return false;

	if (mpVKeyState[key] == 1)
		return true;
	else
		return false;
}

bool InputInterface::isKeyUp(SDL_Scancode key) {
	if (mpVKeyState == NULL)
		return false;

	if (mpVKeyState[key] == 0)
		return true;
	else
		return false;
}

void InputInterface::update() {
	SDL_Event sdlEvent;

	while (SDL_PollEvent(&sdlEvent)) {

		switch (sdlEvent.type) {
		case SDL_KEYDOWN:
			switch (sdlEvent.key.keysym.sym) {
			case SDLK_ESCAPE:
				SDLInterface::getInstance()->isExit = true;
				break;
			}
			break;
		case SDL_KEYUP:
			onKeyUp();
			break;
		case SDL_QUIT:
			SDLInterface::getInstance()->isExit = true;
			break;
		}
	}
}