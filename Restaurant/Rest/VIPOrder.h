#pragma once
#include "Order.h"
class VIPOrder :
	public Order

{
	int ExMoney;
	double Priority;
public:
	VIPOrder(int ID, ORD_TYPE r_Type, REGION r_region, int ex_money =0);
	VIPOrder(Order ord, int ex_money);
	virtual bool operator <=(VIPOrder Or);
	virtual bool operator <(VIPOrder Or);
	virtual bool operator ==(VIPOrder Or);
	void setPriority();
	void setExMoney(int ex_money);
	int getPriority();
	int getExMoney();
	~VIPOrder();
};

