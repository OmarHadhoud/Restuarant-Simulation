#pragma once
#include "Event.h"
class CancellationEvent :
	public Event
{
public:
	CancellationEvent(int eTime, int ordID);
	~CancellationEvent();
	virtual void Execute(Restaurant* pRest);
};
