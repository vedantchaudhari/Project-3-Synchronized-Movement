#pragma once

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


#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"

//
#include "Flock.h"

#include "Thread.h"

class NetworkingInterface {
private:
	static NetworkingInterface* mpInstance;

	// Connection counts for easy access: inbound only and total
	unsigned short mMaxInbound, mMaxConnections;
	// Ports used for inbound and outbound connections
	unsigned short mPortIn, mPortOut;
	// Networking management w/ thread
	bool mNetworkThreadAllowed;
	egpThread mNetworkThread[1];

	RakNet::SystemAddress mServerIP;
	RakNet::SocketDescriptor sd;

	// Thread launch function
	//static int NetworkingThread(NetworkingInterface* networkInterface);

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
		address:	external peer IP Address
		portOffset:	port offset to use
	@return
	*/
	int ConnectPeer(const char address[16], const unsigned short portOffset);

	/*
	Internal utility for disconnecting from an external peer
	*/
	int DisconnectPeer();
	int ProcessAllPackets() const;
	//void ResetValues();

protected:

	// Enumeration of custom packet identifiers
	enum PacketIdentifier {
		DEFAULT_MSG = ID_USER_PACKET_ENUM,

		// Sent by Server
		REQUEST_CLIENT_FLOCK_MSG,
		REQUEST_CLIENT_IP_MSG,
		FLOCK_STATE_UPDATE_MSG,

		// Sent by client
		CLIENT_FLOCK_DATA_MSG,
		CLIENT_IP_MSG,
	};

	/*
	Constructor
	*/
	NetworkingInterface()
	{
		mpPeer = RakNet::RakPeerInterface::GetInstance();
		mPortIn = 60000;

		mNetworkThreadAllowed = false;
	}

	/*
	API for processing individual packet; function should not change manager
	@params
		packet: pointer to packet
		packetIndex: index in sequence of processed packets
	@return
		0	stop processing packets
	*/
	virtual int ProcessPacket(const RakNet::Packet *const packet, const unsigned int packetIndex) const;

public:
	/*
	Constructor
	*/
	RakNet::RakPeerInterface* mpPeer;

	/*
	API for getting current NetworkingInterface instance
	@return
		Static NetworkingInterface instance
	*/
	static NetworkingInterface* getInstance();

	/*
	Virtual Destructor
	*/
	//	virtual ~NetworkingInterface();

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
		const unsigned short portOffsetInbound, const bool threaded = false)
	{
		InitializePeer(doesConnect, maxInbound, portOffsetInbound);
		
		return 1;
	}

	/*
	API for shutting down networking
	@return
	1	success
	0	failure
	*/
	virtual int ShutdownNetworking() {
		return 1;
	};

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
	virtual int Connect(const char address[16], const unsigned short portOffset = 0)
	{
		ConnectPeer(address, portOffset);
		return 1;
	};
	/*
	API for disconnection
	@return
	1	success
	0	failure
	-1	not connected
	*/
	virtual int Disconnect() {
		return 1;
	};

	/*
	API for handling networking
	*/
	virtual int HandleNetworking() const;

	/*
	API for networking update; publicly accessible for non-threaded networking
	@return:
	n	packet count
	0	if no packets proccessed or not set up
	-1	if threaded (fails)
	*/
	//virtual int HandleNetworking() const;
	void SendFlock(Flock toSend);

	/*
	API for getting port with offset from default for this application
	@params
	portOffset: added to default to retrieve port (pass 0 for default)
	@return
	n	port number
	*/
	static unsigned short GetPort(const unsigned short portOffset = 0) { return 60000; };
};