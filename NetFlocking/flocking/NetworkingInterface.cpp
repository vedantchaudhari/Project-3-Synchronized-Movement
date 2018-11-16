// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 3
// 8-27-2018
//
// Vedant Chaudhari, 1532077
// John Malvey, 1005854
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

#include "NetworkingInterface.h"
#include "ServerDefine.h"

NetworkingInterface* NetworkingInterface::mpInstance = NULL;

// -----------------------------------------------------------------------------------------------------
/* INTERNAL UTILITIES */

int NetworkingInterface::InitializePeer(const bool doesConnect, const unsigned short maxInbound,
	const unsigned short portOffsetInbound) {
	mMaxInbound = maxInbound;
	mMaxConnections = maxInbound + (doesConnect ? 1 : 0);
	mPortIn = maxInbound ? GetPort(portOffsetInbound) : 0;

	mNetworkThreadAllowed = true;

	// Open socket if we are accepting connections
	RakNet::SocketDescriptor sd;// = mMaxInbound ? RakNet::SocketDescriptor(mPortIn, 0) : RakNet::SocketDescriptor();
	mpPeer->Startup(1, &sd, 1);
	mpPeer->SetMaximumIncomingConnections(mMaxInbound);

	return 1;
}

int NetworkingInterface::TerminatePeer() {
	// Stop and reset thread if being used
	mNetworkThreadAllowed = false;
	while (mNetworkThread->isRunning);
	memset(mNetworkThread, 0, sizeof(mNetworkThread));

	// Close connections manually with notification
	for (unsigned short i = 0; i < mMaxConnections; ++i)
		mpPeer->CloseConnection(mpPeer->GetSystemAddressFromIndex(i), true);

	// Shutdown peer and close all connections
	mpPeer->Shutdown(0);

	RakNet::RakPeerInterface::DestroyInstance(mpPeer);

	//ResetValues();

	return 1;
}

int NetworkingInterface::ConnectPeer(const char address[16], const unsigned short portOffset) {
	//const unsigned short portOut = GetPort(portOffset);

	//if (address && *address && portOut != mPortIn) {
		//mPortOut = portOut;
		mpPeer->Connect(address, 60000, 0, 0);

		return 1;
	//}

	//return -1;	// Invalid Parameters
}

int NetworkingInterface::DisconnectPeer() {
	if (mPortOut) {
		mPortOut = 0;
		mpPeer->CloseConnection(mpPeer->GetSystemAddressFromIndex(0), true);

		return 1;
	}

	return -1;
}

int NetworkingInterface::ProcessAllPackets() const {
	RakNet::Packet* packet;
	unsigned int i;

	for (packet = mpPeer->Receive(), i = 0; packet && ProcessPacket(packet, i); 
		mpPeer->DeallocatePacket(packet), packet = mpPeer->Receive(), ++i);

	return 1;
}

void NetworkingInterface::SendFlock(Flock sendFlock)
{
	RakNet::BitStream toSend;
	// sendFlock.writeToBitstream(toSend, SEND_CLIENTDATA);
	mpPeer->Send(&toSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, mServerIP, false);
}

// -----------------------------------------------------------------------------------------------------
/* PROTECTED FUNCTIONS */

int NetworkingInterface::ProcessPacket(const RakNet::Packet *const packet, const unsigned int packetIndex) const
{
	// ****TO-DO: implement base class packet processor
	//	-> base class should process only peer-type-agnostic packets, perhaps 
	//		even including RakNet ones such as ID_CONNECTION_REQUEST_ACCEPTED
	//	-> base class should have first dibs at packet

	switch (packet->data[0]) {
	case ID_CONNECTION_REQUEST_ACCEPTED:
		std::cout << "DEBUG: Connected to server" << std::endl;
		break;
	case REQUEST_CLIENT_IP_MSG:
		std::cout << "DEBUG: Connected to server" << std::endl;
		break;
	case REQUEST_CLIENT_FLOCK_MSG:
		// Send Flock
		break;
	}

	// done
	return (packetIndex + 1);
}

// -----------------------------------------------------------------------------------------------------
/* PUBLIC FUNCTIONS */

NetworkingInterface* NetworkingInterface::getInstance() {
	if (mpInstance == NULL)
		mpInstance = new NetworkingInterface();

	return mpInstance;
}

int NetworkingInterface::HandleNetworking() const {
	if (mpPeer) {
		const int result = !mNetworkThread->handle ? ProcessAllPackets() : -1;

		return result;
	}

	return 0;
}

// -----------------------------------------------------------------------------------------------------