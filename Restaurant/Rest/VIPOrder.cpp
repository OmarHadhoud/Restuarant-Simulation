#include "VIPOrder.h"



VIPOrder::VIPOrder(int ID, ORD_TYPE r_Type, REGION r_region, int ex_money) :Order(ID, r_Type, r_region)
{
	ExMoney = ex_money;
	totalMoney += ExMoney;
}

VIPOrder::VIPOrder(Order ord, int ex_money) : Order(ord.GetID(),TYPE_VIP,ord.GetRegion())
{
	Distance = ord.GetDistance();
	ArrTime = ord.GetArrTime();
	totalMoney = ord.GetMoney();
	ExMoney = ex_money;
	totalMoney += ExMoney;
	setPriority();
}
bool VIPOrder:: operator <=(VIPOrder Or) {
	if ((this->Priority) <= (Or.Priority))
		return true;
	return false;
}
bool VIPOrder:: operator <(VIPOrder Or) {
	if ((this->Priority) < (Or.Priority))
		return true;
	return false;
}
bool VIPOrder::operator ==(VIPOrder Or) {
	if (this->ID == Or.ID)
		return true;
	return false;
}
void VIPOrder::setPriority() {
	Priority = (totalMoney) / (Distance*ArrTime);
}
void VIPOrder::setExMoney(int ex_money) {
	ExMoney = ex_money;
}
int VIPOrder::getPriority() {
	return Priority;
}
int VIPOrder::getExMoney() {
	return ExMoney;
}
VIPOrder::~VIPOrder()
{
}
