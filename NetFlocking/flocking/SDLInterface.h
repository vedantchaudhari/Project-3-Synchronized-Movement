#pragma once

#include <iostream>
#include <math.h>
#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "Define.h"

class SDLInterface {
private:
	static SDLInterface* mpInstance;

	SDL_Window* mpWindow;
	SDL_Renderer* mpRenderer;

	int mWindowWidth;
	int mWindowHeight;

public:
	bool isExit = false;

	SDLInterface();
	~SDLInterface();

	void init(int width, int height, const std::string& name);
	void render();
	void exit();

	void background();

	// Draw Functions
	void setColor(int r, int g, int b, int a);
	void drawTriangle(float x, float y, float width, float height, float angle);
	void drawLine(int x1, int y1, int x2, int y2);
	void drawCircle(int x, int y, int radius);

	// Getters
	static SDLInterface* getInstance();

	inline SDL_Window* getWindow() { return mpWindow; };
	inline SDL_Renderer* getRenderer() { return mpRenderer; };

	inline int getWidth() { return mWindowWidth; };
	inline int getHeight() { return mWindowHeight; };
};