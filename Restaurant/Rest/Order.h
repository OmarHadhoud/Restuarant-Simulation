#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"
#include "Motorcycle.h"
class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, Frozen, VIP
	REGION Region;  //Region of this order
	int Distance;	//The distance (in meters) between the order location and the resturant 
	bool served;
	double totalMoney;	//Total order money

	int ArrTime, ServTime, WaitTime, FinishTime;	//arrival, service start, and finish times

public:
	Order(int ID, ORD_TYPE r_Type, REGION r_region);
	virtual ~Order();

	int GetID() const;

	ORD_TYPE GetType() const;
	REGION GetRegion() const;

	void SetDistance(int d);
	int GetDistance() const;

	void SetMoney(double m);
	double GetMoney() const;

	void SetArrTime(int t);
	int GetArrTime() const;

	void SetWaitTime(int t);
	int GetWaitTime() const;

	void SetFinishTime(int t);
	int GetFinishTime() const;	

	void SetServTime(int t);
	int GetServTime() const;

	void Serve(int timeStep,Motorcycle* motor);
	virtual bool operator <=(Order Or);
	virtual bool operator <(Order Or);
	virtual bool operator ==(Order Or);
};

#endif