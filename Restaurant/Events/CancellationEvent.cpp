#include "CancellationEvent.h"
#include "../Rest/Restaurant.h"


CancellationEvent::CancellationEvent(int eTime, int ordID) :Event(eTime, ordID)
{
}


CancellationEvent::~CancellationEvent()
{
}
void CancellationEvent::Execute(Restaurant* pRest) {
	pRest->CancelOrder(OrderID);
}
