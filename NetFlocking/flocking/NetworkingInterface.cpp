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

#include "NetworkingInterface.h"

// -----------------------------------------------------------------------------------------------------
/* INTERNAL UTILITIES */

int NetworkingInterface::InitializePeer(const bool doesConnect, const unsigned short maxInbound,
	const unsigned short portOffsetInbound) {
	mMaxInbound = maxInbound;
	mMaxConnections = maxInbound + (doesConnect ? 1 : 0);
	mPortIn = maxInbound ? GetPort(portOffsetInbound) : 0;

	mNetworkThreadAllowed = true;

	mpPeer->SetOccasionalPing(true);

	// Open socket if we are accepting connections
	RakNet::SocketDescriptor sd = mMaxInbound ? RakNet::SocketDescriptor(mPortIn, 0) : RakNet::SocketDescriptor();
	mpPeer->Startup(mMaxConnections, &sd, 1);
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

	ResetValues();

	return 1;
}

int NetworkingInterface::ConnectPeer(const char address[16], const unsigned short portOffset) {
	const unsigned short portOut = GetPort(portOffset);

	if (address && *address && portOut != mPortIn) {
		mPortOut = portOut;
		mpPeer->Connect(address, portOut, 0, 0);

		return 1;
	}

	return -1;	// Invalid Parameters
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

}

// -----------------------------------------------------------------------------------------------------