#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <vector>

#include "Define.h"
#include "ServerDefine.h"
#include "Flock.h"
#include "SDLInterface.h"
#include "InputInterface.h"
#include "NetworkingInterface.h"

#include "../../DevSDKs/include/RakNet/RakPeerInterface.h"
#include "../../DevSDKs/include/RakNet/MessageIdentifiers.h"
#include "../../DevSDKs/include/RakNet/BitStream.h"
#include "../../DevSDKs/include/RakNet/RakNetTypes.h"
#include <stdio.h>

#include "Event.h"
#include "EventManager.h"
#include "FlockStateUpdate.h"

void update() {
	SDLInterface::getInstance()->render();
	SDLInterface::getInstance()->background();
	InputInterface::getInstance()->update();
	EventManager::getInstance()->execute();
	SDLInterface::getInstance()->setColor(255, 255, 255, 255);
}

Flock predict(Flock oldFlock, Uint32 deltaTime) {
	Flock flockPrediction = Flock(NUM_BOIDS);
	// PSEUDO CODE FOR FORMULA
	// predicted_position = old_position + (old_velocity * elapsedSeconds) + (0.5 * old_acceleration * (elapsedSeconds^2))
	// WITHOUT ACCELERATION
	// predicted_position = old_position + (old_velocity * elapsedSeconds)
	for (int iter = 0; iter < NUM_BOIDS; iter++) {
		flockPrediction.boidsList[iter].pos[0] = oldFlock.boidsList[iter].pos[0] +
			(oldFlock.boidsList[iter].velocity[0] * deltaTime);
		flockPrediction.boidsList[iter].pos[1] = oldFlock.boidsList[iter].pos[1] +
			(oldFlock.boidsList[iter].velocity[1] * deltaTime);
	}

	return flockPrediction;
}

Flock interpolate(Flock oldFlock, Flock flockPrediction, float percentCompleted) {
	Flock finalFlock = Flock(NUM_BOIDS);

	for (int iter = 0; iter < NUM_BOIDS; iter++) {
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

	for (int iter = 0; iter < NUM_BOIDS; iter++) {
		oldFlock.boidsList = finalFlock.boidsList;
	}

	return finalFlock;
}

int main(int argc, char *argv[]) {
	SDLInterface::getInstance()->init(WIDTH, HEIGHT, TITLE);

	Flock myFlock = Flock(NUM_BOIDS);

	Flock enemyFlock = Flock(NUM_BOIDS);
	Flock enemyFlockPrediction = Flock(NUM_BOIDS);

	Uint32 iTime = SDL_GetTicks();
	Uint32 iTime2 = 0;

	bool isLocal = true;
	char response;

	std::string serverIP;

	std::cout << "[L]ocal or [N]etworked Boid Simulation? ";
	std::cin >> response;

	if (response == 'N' || response == 'n')
		isLocal = false;

	if (!isLocal) {
		std::cout << "Enter Server IP Address formatted: ###.###.###.### ";
		std::cin >> serverIP;
		NetworkingInterface::getInstance()->StartupNetworking(true, 1, 0, false);
		NetworkingInterface::getInstance()->Connect(serverIP.c_str(), 0);
	}
	
	while (SDLInterface::getInstance()->isExit == false) {
		if (isLocal) {	// Local Game
			iTime2 = SDL_GetTicks();

			if (iTime2 - iTime >= TICK) {
				iTime = SDL_GetTicks();
				//myFlock.update();
			}

			myFlock.render();
			update();
		}
		else {	// Networked Game
			iTime2 = SDL_GetTicks();

			if (iTime2 - iTime >= TICK) {
				iTime = SDL_GetTicks();

				myFlock.update();
				enemyFlock = predict(enemyFlock, iTime2 - iTime);
			}

			for (
				RakNet::Packet* packet = NetworkingInterface::getInstance()->mpPeer->Receive();
				packet; NetworkingInterface::getInstance()->mpPeer->DeallocatePacket(packet),
				packet = NetworkingInterface::getInstance()->mpPeer->Receive()) {
				switch (packet->data[0]) {
				case ID_CONNECTION_REQUEST_ACCEPTED:
					std::cout << "Connected to server" << std::endl;
					break;
				case REQUEST_CLIENT_IP_MSG:
				{
					std::cout << "Server has requested client IP" << std::endl;

					ClientIPMsg msg;
					msg.typeID = CLIENT_IP_MSG;
					msg.clientIP = NetworkingInterface::getInstance()->mpPeer->GetSystemAddressFromIndex(0);

					NetworkingInterface::getInstance()->mpPeer->Send((char*)&msg, sizeof(msg), HIGH_PRIORITY, RELIABLE_ORDERED, 0,
						packet->systemAddress, false);
				}
				break;
				case REQUEST_CLIENT_FLOCK_MSG:
				{
					std::cout << "Server has requested client flock" << std::endl;
					// Send initial flock data using bitstream

					RakNet::BitStream bsOutFlockData;
					myFlock.writeToBitstream(bsOutFlockData, CLIENT_FLOCK_DATA_MSG);

					NetworkingInterface::getInstance()->mpPeer->Send(&bsOutFlockData, HIGH_PRIORITY, RELIABLE_ORDERED, 0,
						packet->systemAddress, false);
				}
				break;
				case FLOCK_STATE_UPDATE_MSG:
				{	
					//std::cout << "Flock state update recieved from server" << std::endl;
					myFlock.readFromBitstream(packet);
					// Receive bitstream and add to event
					// RakNet::BitStream bsFlockingData(packet->data, packet->length, false);
				}
				break;
				case OTHER_FLOCK_STATE_MSG:
				{
					//std::cout << "Other Flock state update recieved from server" << std::endl;

					const RakNet::BitStream bsFlockingData(packet->data, packet->length, false);

					FlockStateUpdate* updateEvent = new FlockStateUpdate(bsFlockingData);
					EventManager::getInstance()->add(updateEvent->getType(), updateEvent);

					enemyFlock.readFromBitstream(packet);
					enemyFlock.setColor(1);
					enemyFlockPrediction.readFromBitstream(packet);
					enemyFlock = interpolate(enemyFlock, enemyFlockPrediction, 0.5);
				}
				break;
				}

			}
		}

		enemyFlock.render();
		myFlock.render();
		update();
	}

	SDLInterface::getInstance()->exit();
	return 0;
}