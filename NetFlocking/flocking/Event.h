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

enum EVENT_TYPE {
	INVALID_TYPE,

	FLOCK_STATE_UPDATE_EVENT,
};

class Event {
private:
	EVENT_TYPE mType;

public:
	Event() { mType = INVALID_TYPE; };
	Event(EVENT_TYPE _type) : mType(_type) {};
	virtual ~Event() {};

	virtual void execute() = 0;

	EVENT_TYPE getType() const { return mType; };
};