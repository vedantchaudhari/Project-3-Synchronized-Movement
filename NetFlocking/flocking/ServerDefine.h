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

#include "Define.h"
#include "Flock.h"

enum GameMessages
{
	DEFAULT_MESSAGE = ID_USER_PACKET_ENUM,
	REQUEST_CLIENT_DATA_MSG,
	FLOCK_STATE_UPDATE_MSG,
	REQUEST_CLIENT_IP_MSG,
	SEND_CLIENT_IP_MSG,

	SEND_CLIENTDATA,
	RECIEVE_FLOCK_DATA,
	RECIEVE_FLOCK2_DATA,
	RECIEVE_COUPLEDFLOCK_DATA,
	GET_ORDER,
};

#pragma pack(push, 1)
struct BaseMessage {
	unsigned char typeID;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ClientData
{
	unsigned char useTimeStamp;
	RakNet::Time timeStamp;

	int ID = SEND_CLIENTDATA;
	Flock clientFlock;
	RakNet::SystemAddress clientIP;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct FlockStateUpdate {
	unsigned char useTimeStamp;
	RakNet::Time timeStamp;

	unsigned char typeID = FLOCK_STATE_UPDATE_MSG;

	Flock enemyFlock;
	RakNet::SystemAddress clientIP;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct ClientIPData {
	unsigned char typeID = SEND_CLIENT_IP_MSG;

	RakNet::SystemAddress clientIP;
};
#pragma pack(pop)