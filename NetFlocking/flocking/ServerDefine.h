#pragma once

// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 3
// 11-15-2018
//
// Vedant Chaudhari, 1532077
// John Malvey, 1005854
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

// standard includes
#include <stdio.h>
#include <string.h>
#include <string>

// RakNet includes
#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"

#include"NetworkingInterface.h"

#include "Define.h"
#include "Flock.h"

enum GameMessages {
	DEFAULT_MSG = ID_USER_PACKET_ENUM,
	
	// Sent by Server
	REQUEST_CLIENT_FLOCK_MSG,
	REQUEST_CLIENT_IP_MSG,
	FLOCK_STATE_UPDATE_MSG,
	OTHER_FLOCK_STATE_MSG,

	// Sent by client
	CLIENT_FLOCK_DATA_MSG,
	CLIENT_IP_MSG,
};

#pragma pack(push, 1)
struct BaseMsg {
	unsigned char typeID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ClientFlockDataMsg {
	unsigned char typeID = CLIENT_FLOCK_DATA_MSG;

	Flock flock;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ClientIPMsg {
	unsigned char typeID = CLIENT_IP_MSG;

	RakNet::SystemAddress clientIP;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FlockStateUpdateMsg {
	unsigned char typeID = FLOCK_STATE_UPDATE_MSG;

	RakNet::Time timeStamp;
	Flock enemyFlock;
};
#pragma pack(pop)