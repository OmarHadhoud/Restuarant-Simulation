#include "ArrivalEvent.h"
#include "..\Rest\Restaurant.h"


ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, REGION reg, double Money, int Distance) :Event(eTime, oID)
{
	ArrTime = eTime;
	OrdType = oType;
	OrdRegion = reg;
	OrdDistance = Distance;
	OrdMoney = Money;
}
int ArrivalEvent::getOrdDistance() const
{
	return OrdDistance;

}
REGION ArrivalEvent::getRegion() const
{
	return OrdRegion;

}
/*
void ArrivalEvent::setOrdMoney(double money)
{
OrdMoney = money;
}
void ArrivalEvent::setOrdDistance(int dist)
{
OrdDistance = dist;
}
*/
double ArrivalEvent::getOrdMoney() const
{
	return OrdMoney;
}
ORD_TYPE ArrivalEvent::getOrdType() const
{
	return OrdType;
}
void ArrivalEvent::Execute(Restaurant* pRest)
{
	//This function should create and order and and fills its data 
	// Then adds it to normal, frozen, or VIP order lists that you will create in phase1
	Order* pOrd;
	if (OrdType == TYPE_VIP)
	{
		VIPOrder* pOrdV = new VIPOrder(OrderID, OrdType, OrdRegion);
		pOrdV->SetMoney(OrdMoney);
		pOrdV->SetDistance(OrdDistance);
		pOrdV->SetArrTime(ArrTime);
		pOrdV->setPriority();
		pOrd = pOrdV;
	}
	else {
		pOrd = new Order(OrderID, OrdType, OrdRegion); 
		pOrd->SetMoney(OrdMoney);
		pOrd->SetDistance(OrdDistance);
		pOrd->SetArrTime(ArrTime);

	}
	pRest->AddToOrders(pOrd);

}
