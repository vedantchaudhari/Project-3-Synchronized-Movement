// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 2 & Lab 4
// 10-30-2018
//
// Vedant Chaudhari, 1532077
// John Malvey
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

// FLAGS
#define DRAW_RADIUS_FLAG false
#define DRAW_VECTOR_FLAG false

// CONSTANTS
#define WIDTH 640
#define HEIGHT 480
#define TITLE "Networked Boids"

#define NUM_BOIDS 10
#define BOID_RADIUS 120
#define SEPERATION_RADIUS 40

#define BOID_WIDTH 30
#define BOID_HEIGHT 15

#define COHESION_WEIGHT 0.01
#define SEPARATION_WEIGHT 0.09
#define ALIGNMENT_WEIGHT 0.9

#define PI 3.1415
#define TICK 10
#define TURN_INCREMENT PI/180