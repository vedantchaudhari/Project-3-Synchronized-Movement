#pragma once

#include <iostream>
#include <math.h>

#include "Define.h"

class Boid {
private:

public:
	Boid();
	Boid(float x, float y);

	int numOfNeighbors = 0;

	// Vector 2 Data
	float pos[2] = { 0, 0 };
	float alignmentTargetPos[2] = { 0, 0 };
	float cohesionTargetPos[2] = { 0, 0 };
	float separationTargetPos[2] = { 0, 0 };

	float targetPosBuffer[2] = { 0, 0 };
	float velocity[2] = { 0, 0 };
	float theta = 0;

	float sumofNeighborsTheta = 0;

	void update();

	void addToTargetPosBuffer(float x, float y);
	void clearBuffers();
	void steerTo(float x, float y);

	/*Boid operator=(const Boid b)
	{
	numOfNeighbors = b.numOfNeighbors;
	pos[0] = b.pos[0];
	pos[1] = b.pos[1];
	alignmentTargetPos[0] = b.alignmentTargetPos[0];
	alignmentTargetPos[1] = b.alignmentTargetPos[1];
	cohesionTargetPos[0] = b.cohesionTargetPos[0];
	cohesionTargetPos[1] = b.cohesionTargetPos[1];
	separationTargetPos[0] = b.separationTargetPos[0];
	separationTargetPos[1] = b.separationTargetPos[1];

	targetPosBuffer[0] = b.targetPosBuffer[0];
	targetPosBuffer[1] = b.targetPosBuffer[1];
	velocity[0] = b.velocity[0];
	velocity[1] = b.velocity[1];
	theta = b.theta;
	return *this;
	}*/
};