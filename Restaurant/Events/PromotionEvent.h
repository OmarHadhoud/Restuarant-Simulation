#pragma once
#include "Event.h"
class PromotionEvent :
	public Event
{
	int ExMoney;
public:

	PromotionEvent(int eTime, int oID, int ExM);
	int getExMoney() const;
	~PromotionEvent();
	virtual void Execute(Restaurant* pRest);
};

