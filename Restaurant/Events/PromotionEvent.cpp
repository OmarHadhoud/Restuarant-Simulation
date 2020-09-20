#include "PromotionEvent.h"
#include "../Rest/Restaurant.h"
#include "../Rest/VIPOrder.h"


PromotionEvent::PromotionEvent(int eTime, int oID, int ExM) :Event(eTime, oID)
{
	ExMoney = ExM;
}
int PromotionEvent::getExMoney() const {
	return ExMoney;
}

PromotionEvent::~PromotionEvent()
{
}
void PromotionEvent::Execute(Restaurant* pRest) {
	pRest->PromoteOrder(OrderID,ExMoney);
	//Call promotion function (in phase 2) with the id and extra money as parameters.
}
