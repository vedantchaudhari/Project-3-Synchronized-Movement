#pragma once

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

//boids includes
#include "Define.h"
#include "Flock.h"

enum GameMessages
{
	SETMODE_PUSH = ID_USER_PACKET_ENUM,
	SETMODE_SHARE,
	SETMODE_COUPLED,
	DATA_PUSH,
	DATA_SHARE,
	DATA_COUPLED,
	INCOMING_CLIENTDATA,
	RECIEVE_FLOCK_DATA,
	RECIEVE_FLOCK2_DATA,
	RECIEVE_COUPLEDFLOCK_DATA,
	GET_CLIENT_IP,
	GET_ORDER,
};

#pragma pack(push, 1)
struct GameMessageData
{
	int ID = DATA_PUSH;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ClientData
{
	int ID = INCOMING_CLIENTDATA;
	Flock clientFlock;
	RakNet::SystemAddress clientIP;
	bool instantiated = false;
};
#pragma pack(pop)
