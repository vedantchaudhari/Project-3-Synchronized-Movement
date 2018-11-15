// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 2 & Lab 4
// 10-30-2018
//
// Vedant Chaudhari, 1532077
// John Malvey 1005854
// 
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

// Based on https://github.com/forsythe/boids

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <vector>

#include "Define.h" 
#include "Flock.h"
#include "SDLInterface.h"
#include "InputInterface.h"
/*
#include "../../DevSDKs/include/RakNet/RakPeerInterface.h"
#include "../../DevSDKs/include/RakNet/MessageIdentifiers.h"
#include "../../DevSDKs/include/RakNet/BitStream.h"
#include "../../DevSDKs/include/RakNet/RakNetTypes.h"
#include "../../DevSDKs/include/RakNet/DS_List.h"*/
#include <stdio.h>

#include "ServerDefine.h"

#include "FlockStateUpdate.h"

void update() {
	SDLInterface::getInstance()->render();
	SDLInterface::getInstance()->background();
	InputInterface::getInstance()->update();
	EventManager::getInstance()->execute();
	SDLInterface::getInstance()->setColor(255, 255, 255, 255);
}

void prediction(Flock oldFlock, Flock flockPrediction, Uint32 deltaTime) {
	// PSEUDO CODE FOR FORMULA
	// predicted_position = old_position + (old_velocity * elapsedSeconds) + (0.5 * old_acceleration * (elapsedSeconds^2))
	// WITHOUT ACCELERATION
	// predicted_position = old_position + (old_velocity * elapsedSeconds)
	for (int iter = 0; iter < oldFlock.getCount(); iter++) {
		flockPrediction.boidsList[iter].pos[0] = oldFlock.boidsList[iter].pos[0] +
			(oldFlock.boidsList[iter].velocity[0] * deltaTime);
		flockPrediction.boidsList[iter].pos[1] = oldFlock.boidsList[iter].pos[1] +
			(oldFlock.boidsList[iter].velocity[1] * deltaTime);
	}

	// TODO
	// Once u calculated predicted positions, blend the old state with the prediction
	// Once u receive a new packet from the server, correct the position of the boids
}

// percentCompleted shoudl be between [0, 1] (want to come up with a value in between the time steps of the server)
void blend(Flock oldFlock, Flock flockPrediction, float percentCompleted) { // old & new
	Flock finalFlock = Flock(NUM_BOIDS);

	for (int iter = 0; iter < oldFlock.getCount(); iter++) {
		float percentageToOld = 1.0 - percentCompleted;

		finalFlock.boidsList[iter].pos[0] = (percentageToOld * oldFlock.boidsList[iter].pos[0]) +
			(percentCompleted * flockPrediction.boidsList[iter].pos[0]);
		finalFlock.boidsList[iter].pos[1] = (percentageToOld * oldFlock.boidsList[iter].pos[1]) +
			(percentCompleted * flockPrediction.boidsList[iter].pos[1]);

		finalFlock.boidsList[iter].velocity[0] = (percentageToOld * oldFlock.boidsList[iter].velocity[0]) +
			(percentCompleted * flockPrediction.boidsList[iter].velocity[0]);
		finalFlock.boidsList[iter].velocity[1] = (percentageToOld * oldFlock.boidsList[iter].velocity[1]) +
			(percentCompleted * flockPrediction.boidsList[iter].velocity[1]);
	}

	for (int iter = 0; iter < oldFlock.getCount(); iter++) {
		oldFlock.boidsList = finalFlock.boidsList;
	}
}

int main(int argc, char *argv[]) {
	SDLInterface::getInstance()->init(WIDTH, HEIGHT, TITLE);

	Flock flock = Flock(NUM_BOIDS);

	Flock flock2 = Flock(NUM_BOIDS);	// Timestamped server update
	Flock flock2Prediction = Flock(NUM_BOIDS);

	Uint32 iTime = SDL_GetTicks();
	Uint32 iTime2 = 0;

	//networking vars
	RakNet::Packet *packet;
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	const short SERVERPORT = 60000;
	char str[512];
	RakNet::SocketDescriptor sd;
	bool twoFlocks = false;
	bool coupleReadyToSend = false;
	RakNet::SystemAddress otherClientAddress = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
	bool isClient1 = false;

	peer->Startup(1, &sd, 1);

	//set up client connection to server
	printf("Enter server IP\n");
	fgets(str, 512, stdin);
	if (str[0] == 10) {
		strcpy(str, "127.0.0.1");
	}

	system("cls");
	peer->Connect(str, SERVERPORT, 0, 0);

	//big timeout timer
	peer->SetTimeoutTime(999999, RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	while (SDLInterface::getInstance()->isExit == false) {

		//recieve network packets
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case REQUEST_CLIENT_DATA_MSG:
			{
				std::cout << "CLIENT: Sending client flocking data to server" << std::endl;

				RakNet::BitStream sendData;
				flock.update();
				flock.writeToBitstream(sendData, SEND_CLIENTDATA);

				peer->Send(&sendData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
			break;
			case FLOCK_STATE_UPDATE_MSG:	// Sent by server, processed by client
			{
				// update other clients flock positions with what u receiver from the server
				// Call te blending function
				std::cout << "CLIENT: Receiving state update from server" << std::endl;
				// Create event and add to queue
				Flock recFlock = Flock(NUM_BOIDS);
				recFlock.readFromBitstream(packet);
				FlockStateUpdate* newMsg = new FlockStateUpdate(recFlock);
				EventManager::getInstance()->add(FLOCK_STATE_UPDATE_EVENT, newMsg);
			}
			break;
			case REQUEST_CLIENT_IP_MSG:
			{
				std::cout << "Client IP Received" << std::endl;

				ClientIPData *temp = ((ClientIPData*)packet->data);
				otherClientAddress = temp->clientIP;
			}
			break;
			}

			iTime2 = SDL_GetTicks();

			update();
		}

		SDLInterface::getInstance()->exit();
		return 0;
	}
}