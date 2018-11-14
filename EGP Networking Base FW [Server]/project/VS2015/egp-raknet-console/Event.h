#pragma once

enum EVENT_TYPE {
	INVALID_TYPE,

	SEND_FLOCK_STATE,
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