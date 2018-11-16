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
	DEFAULT_MSG = ID_USER_PACKET_ENUM,
	//SENT BY SERVER MESSAGES
	REQUEST_CLIENT_FLOCK_MSG,
	REQUEST_CLIENT_IP_MSG,
	FLOCK_STATE_UPDATE_MSG,
	OTHER_FLOCK_STATE_MSG,

	//SENT BY CLIENT MESSAGES
	CLIENT_FLOCK_DATA_MSG,
	CLIENT_IP_MSG,
};


#pragma pack(push, 1)
struct BaseMsg
{
	unsigned char typeID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ClientFlockDataMsg
{
	
	unsigned char typeID;
	Flock flock;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FlockStateUpdateMsg 
{
	FlockStateUpdateMsg():enemyFlock(NUM_BOIDS) {};

	unsigned char typeID;
	
	RakNet::Time timeStamp;
	Flock enemyFlock;

};
#pragma pack(pop)

#pragma pack(push, 1)
struct ClientIPMsg
{
	unsigned char typeID;

	RakNet::SystemAddress clientIP;
};
#pragma pack(pop)



#pragma pack(push, 1)
struct Client
{
	Client():flock(NUM_BOIDS) {};

	Flock flock;
	RakNet::SystemAddress clientIP;
};
#pragma pack(pop)