/*
	RakNet Console Application
	Prepared by Dan Buckstein
	September 5, 2018

	This file was modified by Jack Malvey with permission from author.”
	"I certify that this work is
	entirely my own. The assessor of this project may reproduce this project
	and provide copies to other academic staff, and/or communicate a copy of
	this project to a plagiarism-checking service, which may retain a copy of the
	project on its database"
	-John Malvey 1005854
*/
#include "../headers/CommonInclude.h"
#include "../headers/input.h"

// entry function
int main(int const argc, char const *const *const argv)
{
	//Initializations
	unsigned short serverPort = 60000;
	const unsigned int MAX_CLIENTS = 3;

	//Begin Networking
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();

	Client clientList[2];
	bool client1Set = false;

	bool c1Flock = false, c2Flock = false;

	bool bothClientsConnected = false;

	bool serverPaused = false;

	RakNet::SocketDescriptor sd(serverPort, 0);
	peer->Startup(MAX_CLIENTS, &sd, 1);

	//connect client and server
	printf("Starting the server @ %s\n", peer->GetLocalIP(2));
	peer->SetMaximumIncomingConnections(MAX_CLIENTS);

	peer->SetTimeoutTime(999999, RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	std::string placeHolder = "";
	RakNet::Packet *packet;
	while (1)
	{
		if (!serverPaused) {
			for (
				packet = peer->Receive();
				packet;
				peer->DeallocatePacket(packet), packet = peer->Receive()
				)
			{
				switch (packet->data[0])
				{
				case ID_REMOTE_DISCONNECTION_NOTIFICATION:
					printf("Another client has disconnected.\n");
					break;
				case ID_REMOTE_CONNECTION_LOST:
					printf("Another client has lost the connection.\n");
					break;
				case ID_NEW_INCOMING_CONNECTION:
				{
					printf("A client has connected.\n");
					BaseMsg gameMessage;

					printf("Request client IP Address\n");
					gameMessage.typeID = REQUEST_CLIENT_IP_MSG;
					peer->Send((char*)&gameMessage, sizeof(gameMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				}
				break;
				case ID_REMOTE_NEW_INCOMING_CONNECTION:
					printf("Another client has connected.\n");
					break;

				case ID_DISCONNECTION_NOTIFICATION:
				{
					printf("A client has disconnected.\n");
					bothClientsConnected = false;
				}
				case ID_TIMESTAMP:
				{
					printf("Client has sent timestamp\n");
				}
				break;

				case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
					break;

					break;

					//Jack
				case CLIENT_IP_MSG:
				{
					//add client data to client list
					printf("incoming client data: \n");
					ClientIPMsg* gameMessage = (ClientIPMsg*)packet->data;

					printf("Client IP Address is %s\n",packet->systemAddress);

					// Request Flock Data

					if (!client1Set)
					{
						client1Set = true;
						clientList[0].clientIP = packet->systemAddress;
						printf("Client 1 IP Received!\nRequesting flock data...\n");

						BaseMsg getFlock;
						getFlock.typeID = REQUEST_CLIENT_FLOCK_MSG;
						peer->Send((char*)&getFlock, sizeof(getFlock), HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[0].clientIP, false);

					}
					else if (client1Set)
					{
						clientList[1].clientIP = packet->systemAddress;
						printf("Client 2 IP Received!\nRequesting flock data...\n");

						BaseMsg getFlock;
						getFlock.typeID = REQUEST_CLIENT_FLOCK_MSG;
						peer->Send((char*)&getFlock, sizeof(getFlock), HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[1].clientIP, false);

					}

				}

				break;
				//Jack
				case CLIENT_FLOCK_DATA_MSG:
				{

					//send oposing flocks
					if (packet->systemAddress == clientList[0].clientIP)
					{
						printf("Client 0 Flock Recieved!\n");
						clientList[0].flock.readFromBitstream(packet);
						c1Flock = true;
					}
					else if (packet->systemAddress == clientList[1].clientIP)
					{
						printf("Client 1 Flock Recieved!\n");

						clientList[1].flock.readFromBitstream(packet);
						c2Flock = true;
					}
				}
				break;
				case FLOCK_STATE_UPDATE_MSG:
				{

				}
				break;
				default:
					printf("Message with identifier %i has arrived.\n", packet->data[0]);
					break;
				}
			}

			if (c1Flock && c2Flock && !bothClientsConnected)
				bothClientsConnected = true;

			if (bothClientsConnected)
			{
				
				
					clientList[0].flock.update();
					clientList[1].flock.update();	


					RakNet::BitStream client0Flock;
					clientList[0].flock.writeToBitstream(client0Flock, FLOCK_STATE_UPDATE_MSG);
					peer->Send(&client0Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[0].clientIP, false);

					RakNet::BitStream client1Flock;
					clientList[1].flock.writeToBitstream(client1Flock, OTHER_FLOCK_STATE_MSG);
					peer->Send(&client1Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[0].clientIP, false);
					
					clientList[1].flock.writeToBitstream(client0Flock, FLOCK_STATE_UPDATE_MSG);
					peer->Send(&client0Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[1].clientIP, false);

					clientList[0].flock.writeToBitstream(client1Flock, OTHER_FLOCK_STATE_MSG);
					peer->Send(&client1Flock, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[1].clientIP, false);
					
				
				/*
				clientList[0].flock.update();
				clientList[1].flock.update();

				FlockStateUpdateMsg* sendFlock;
				sendFlock = new FlockStateUpdateMsg;
				sendFlock->typeID = FLOCK_STATE_UPDATE_MSG;

				RakNet::BitStream bsOutFlockData;
				RakNet::BitStream bsOutFlockData2;

				//send Client 0 the Client 1 Flock
				clientList[0].flock.writeToBitstream(bsOutFlockData, OTHER_FLOCK_STATE_MSG);
				peer->Send(&bsOutFlockData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[1].clientIP, false);


				//send Client 1 the Client 0 Flock
				//sendFlock->enemyFlock = clientList[0].flock;

				clientList[1].flock.writeToBitstream(bsOutFlockData2, OTHER_FLOCK_STATE_MSG);
				peer->Send(&bsOutFlockData2, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[0].clientIP, false);

				//send flocks to owners
				clientList[0].flock.writeToBitstream(bsOutFlockData, FLOCK_STATE_UPDATE_MSG);
				peer->Send(&bsOutFlockData, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[0].clientIP, false);


				//send Client 1 the Client 0 Flock
				//sendFlock->enemyFlock = clientList[0].flock;

				clientList[1].flock.writeToBitstream(bsOutFlockData2, FLOCK_STATE_UPDATE_MSG);
				peer->Send(&bsOutFlockData2, HIGH_PRIORITY, RELIABLE_ORDERED, 0, clientList[1].clientIP, false);
				*/

			}
		}
		switch (getInput(placeHolder))
		{
		case PAUSE:
		{
			serverPaused = !serverPaused;
			if (serverPaused)
				std::cout << "Server Paused!\n";
			else
				std::cout << "Server Running!\n";

		}
		break;
		}
	}
		// shut down networking by destroying peer interface instance
		RakNet::RakPeerInterface::DestroyInstance(peer);

		// exit
		printf("\n\n");
		system("pause");
		return 0;
	}
	
