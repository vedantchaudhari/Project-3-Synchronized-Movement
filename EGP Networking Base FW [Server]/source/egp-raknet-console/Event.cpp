#include "../headers/Event.h"

Event::Event(EVENT_ID id)
{
	mID = id;
}
Event::Event()
{
	mID = INVALID_EVENT;
}
Event::~Event()
{
}
