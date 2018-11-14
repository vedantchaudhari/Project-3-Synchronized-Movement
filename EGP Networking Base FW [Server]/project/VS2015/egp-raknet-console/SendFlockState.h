#pragma once

// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 2 & Lab 4
// 10-30-2018
//
// Vedant Chaudhari, 1532077
// John Malvey, 1005854
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

#include "Event.h"
#include "../headers/Boid.h"
#include "../headers/Flock.h"

class FlockStateUpdate : public Event {
private:
	EVENT_TYPE mType;
	Flock flockOne;
	Flock flockTwo;

public:
	FlockStateUpdate() : mType(FLOCK_STATE_UPDATE) {};

	virtual void execute();
};