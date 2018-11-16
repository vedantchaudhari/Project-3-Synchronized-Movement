#pragma once

#include <vector>

#include "Boid.h"
#include "RakNet/DS_List.h"
#include "RakNet/BitStream.h"
#include "RakNet/GetTime.h"
#include "RakNet/MessageIdentifiers.h"

class Flock {
private:
	int count;

public:
	//std::vector<Boid> boids;
	DataStructures::List<Boid> boidsList;
	RakNet::Time lastUpdateTimeStamp;

	Flock(int numOfBoids);

	// Steering behaviors  

	float getDistance(Boid boidA, Boid boidB);

	void update();
	void render();

	void setColor(int color);

	// Read & Write to Bitstreams
	void writeToBitstream(RakNet::BitStream& bsOut, unsigned char typeID);
	void readFromBitstream(RakNet::Packet* packet);

	Flock operator=(const Flock& b)
	{
		count = b.count;
		boidsList = b.boidsList;
		return *this;
	}
};