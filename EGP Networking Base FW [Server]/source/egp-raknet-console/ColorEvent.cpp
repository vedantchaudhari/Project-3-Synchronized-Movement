#include "../headers/ColorEvent.h"

ColorEvent::ColorEvent(int color, RakNet::SystemAddress to)
{
	mColor = color;
	mSendTo = to;
};
ColorEvent::ColorEvent()
{
	mColor = 15;
};