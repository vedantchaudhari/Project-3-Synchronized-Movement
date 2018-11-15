#pragma once

// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 3
// 10-30-2018
//
// Vedant Chaudhari, 1532077
// John Malvey, 1005854
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

#include "Event.h"

#include "../../DevSDKs/include/RakNet/RakPeerInterface.h"
#include "../../DevSDKs/include/RakNet/MessageIdentifiers.h"
#include "../../DevSDKs/include/RakNet/BitStream.h"
#include "../../DevSDKs/include/RakNet/RakNetTypes.h"
#include "../../DevSDKs/include/RakNet/DS_List.h"

class FlockStateUpdate : public Event {
private:
	EVENT_TYPE mType;
	Flock enemyFlock;

public:
	FlockStateUpdate(Flock flock) : mType(FLOCK_STATE_UPDATE_EVENT), enemyFlock(flock) {};

	virtual void execute() override;
};