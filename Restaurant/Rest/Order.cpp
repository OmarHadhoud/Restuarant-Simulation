#include "Order.h"
#include <cmath>
Order::Order(int id, ORD_TYPE r_Type, REGION r_region)
{
	ID = (id>0 && id<1000) ? id : 0;	//1<ID<999
	type = r_Type;
	Region = r_region;
	served = false;
}

Order::~Order()
{
}

int Order::GetID() const
{
	return ID;
}
bool Order:: operator <=(Order Or) {
	if (!served)
	{
		if ((this->ArrTime) <= (Or.ArrTime))
			return true;
		return false;
	}
	else
	{
		if ((this->FinishTime <= Or.FinishTime))
			return true;
		return false;
	}
}
bool Order:: operator <(Order Or) {
	if (!served)
	{
		if ((this->ArrTime) < (Or.ArrTime))
			return true;
		if ((this->ArrTime) == (Or.ArrTime))
			return this->ID < Or.ID;
		return false;
	}
	else
	{
		if ((this->FinishTime) < Or.FinishTime)
			return true;
		else if (this->FinishTime == Or.FinishTime)
			return this->ServTime < Or.ServTime;
		else
			return false;
	}
}
bool Order::operator ==(Order Or) {
	if (!served)
	{
		if (this->ID == Or.ID)
			return true;
		return false;
	}
}

void Order::Serve(int TimeStep, Motorcycle* motor)
{
	served = true;
	int speed = motor->getSpeed();
	int time = ceil((float)Distance / speed);
	ServTime = time;
	WaitTime =  TimeStep-ArrTime;
	FinishTime = ServTime+WaitTime+ArrTime;
	motor->setReturnTimeStep(FinishTime + time);
	motor->setServingStatus(true);
}
ORD_TYPE Order::GetType() const
{
	return type;
}

REGION Order::GetRegion() const
{
	return Region;
}

void Order::SetDistance(int d)
{
	Distance = d>0 ? d : 0;
}

int Order::GetDistance() const
{
	return Distance;
}

void Order::SetMoney(double m)
{
	totalMoney = m;
}

double Order::GetMoney() const
{
	return totalMoney;
}

void  Order::SetArrTime(int t) {
	ArrTime = t > 0 ? t : 0;
}
int  Order::GetArrTime() const {
	return ArrTime;
}

void  Order::SetServTime(int t) {
	ServTime = t > 0 ? t : 0;
}
int  Order::GetServTime() const {
	return ServTime;
}

void  Order::SetFinishTime(int t) {
	FinishTime = t > 0 ? t : 0;
}
int  Order::GetFinishTime() const {
	return FinishTime;
}

void  Order::SetWaitTime(int t) {
	WaitTime = t > 0 ? t : 0;
}
int  Order::GetWaitTime() const {
	return WaitTime;
}