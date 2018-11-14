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

#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"

#include "Thread.h"

class NetworkingInterface abstract {
private:

	// Peer management
	RakNet::RakPeerInterface* mpPeer;
	// Connection counts for easy access: inbound only and total
	unsigned short mMaxInbound, mMaxConnections;
	// Ports used for inbound and outbound connections
	unsigned short mPortIn, mPortOut;
	// Networking management w/ thread
	bool mNetworkThreadAllowed;
	egpThread mNetworkThread[1];

	// Thread launch function
	static int NetworkingThread(NetworkingInterface* networkInterface);

	/* INTERNAL UTILITIES */

	/*
	Internal utility for initializing mpPeer
	@params
		doesConnect:		peer will attempt to connect to someone (client vs server)
		maxInbound:			number of inbound connections allowed
		portOffsetInbound:	port offset to use if allowing inbound connections
	@return
		1	success
	*/
	int InitializePeer(const bool doesConnect, const unsigned short maxInbound, const unsigned short portOffsetInbound);

	/*
	Internal utility for terminating mpPeer
	@return
		1	success
	*/
	int TerminatePeer();

	/*
	Internal utility for connecting to an external peer
	@params
	@return
	*/
	int ConnectPeer(const char address[16], const unsigned short portOffset);

	int DisconnectPeer();
	int ProcessAllPackets() const;
	void ResetValues();

protected:

	// Enumeration of custom packet identifiers
	enum PacketIdentifier {
		DEFAULT_MSG = ID_USER_PACKET_ENUM,
		STATE_UPDATE,
	};

	/*
		Constructor
	*/
	NetworkingInterface();

	/*
	API 
	*/

public:

	/*
		Virtual Destructor
	*/
	virtual ~NetworkingInterface();

	/*
	API for starting up networking
	@params
		doesConnect:		peer will attempt to connect to someone
		maxInbound:			number of inbound connections allowed
		portOffsetInbound:	port offset to use if allowing inbound connections (pass 0 for default)
	@return
		1	success
		0	failure
	*/
	virtual int StartupNetworking(const bool doesConnect, const unsigned short maxInbound,
		const unsigned short portOffsetInbound, const bool threaded = false);

	/*
	API for shutting down networking
	@return
		1	success
		0	failure
	*/
	virtual int ShutdownNetworking();

	/*
	API for connection, *IF ALLOWED* (determined at startup)
	@params
		address:	null-terminated IP address to connect to, formatted as "###.###.###.###"
		portOffset: port offset to get port (pass 0 for default)
	@return
		1	success
		0	failure
		-1	invalid parameters
	*/
	virtual int Connect(const char address[16], const unsigned short portOffset = 0);

	/*
	API for disconnection
	@return
		1	success
		0	failure
		-1	not connected
	*/
	virtual int Disconnect();

	/*
	API for networking update; publicly accessible for non-threaded networking
	@return:
		n	packet count
		0	if no packets proccessed or not set up
		-1	if threaded (fails)
	*/
	virtual int HandleNetworking() const;

	/*
	API for getting port with offset from default for this application
	@params
		portOffset: added to default to retrieve port (pass 0 for default)
	@return
		n	port number
	*/
	static unsigned short GetPort(const unsigned short portOffset = 0);
};