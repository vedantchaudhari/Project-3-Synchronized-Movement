#pragma once

enum EVENT_TYPE {
	INVALID_TYPE,

	FLOCK_STATE_UPDATE,
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