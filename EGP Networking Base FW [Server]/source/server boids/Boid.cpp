#include "../headers/Boid.h"

Boid::Boid() {
	// Set boid x & y pos randomly
	pos[0] = (rand() % 100) + WIDTH / 2;
	pos[1] = (rand() % 100) + HEIGHT / 2;

	// Set random x & y velocity
	velocity[0] = (100 - (rand() % 200)) / 100.0;
	velocity[1] = (100 - (rand() % 200)) / 100.0;
}

Boid::Boid(float x, float y) {
	pos[0] = x;
	pos[1] = y;

	// Set random x & y velocity
	velocity[0] = (100 - (rand() % 200)) / 100.0;
	velocity[1] = (100 - (rand() % 200)) / 100.0;
}

void Boid::update() {
	steerTo((cohesionTargetPos[0] * COHESION_WEIGHT) + (separationTargetPos[0] * SEPARATION_WEIGHT)
		+ (alignmentTargetPos[0] * ALIGNMENT_WEIGHT), (cohesionTargetPos[1] * COHESION_WEIGHT) +
		(separationTargetPos[1] * SEPARATION_WEIGHT) + (alignmentTargetPos[1] * ALIGNMENT_WEIGHT));

	pos[0] += velocity[0];
	pos[1] += velocity[1];

	// Wrap Around Baby
	if (pos[0] >= WIDTH)
		pos[0] = 1;
	if (pos[0] <= 0)
		pos[0] = WIDTH;

	if (pos[1] >= HEIGHT)
		pos[1] = 1;
	if (pos[1] <= 0)
		pos[1] = HEIGHT;

	theta = atan2(-velocity[1], velocity[0]);

	if (theta < 0)
		theta += 2 * PI;
}
void Boid::addToTargetPosBuffer(float x, float y) {
	targetPosBuffer[0] += x;
	targetPosBuffer[1] += y;
}

void Boid::clearBuffers() {
	numOfNeighbors = 0;

	targetPosBuffer[0] = 0;
	targetPosBuffer[1] = 0;
}

void Boid::steerTo(float x, float y) {
	if (x == pos[0] && y == pos[1])
		return;

	float faceAngle = atan2(pos[1] - y, x - pos[0]);	// Destination Angle
	float moveAngle = 0;	// Angle update per tick

	if (faceAngle > 0)
		faceAngle += 2 * PI;

	if (faceAngle > theta)
		if (faceAngle - theta <= PI)
			moveAngle = theta + TURN_INCREMENT;
		else
			moveAngle = theta - TURN_INCREMENT;
	else if (theta - faceAngle <= PI)
		moveAngle = theta - TURN_INCREMENT;
	else
		moveAngle = theta + TURN_INCREMENT;

	velocity[0] = cos(moveAngle);
	velocity[1] = -sin(moveAngle);
}