#include "Motorcycle.h"


Motorcycle::Motorcycle(int id, ORD_TYPE ty, int sp, REGION	Reg, STATUS	stat)
{
	ID = (id > 0 && id < 1000) ? id : 0;
	type = ty;
	speed = (sp > 0) ? sp : 0;
	region = Reg;
	status = stat;
}
bool Motorcycle::operator <=(Motorcycle motor) {
	if (!serving)
	{
		if (this->speed <= motor.speed)
			return true;
		return false;
	}
	else
	{
		if ((this->returnTimeStep) <= motor.returnTimeStep)
			return true;
		return false;
	}
}
bool Motorcycle::operator <(Motorcycle motor) {
	if (!serving)
	{
		if (this->speed < motor.speed)
			return true;
		return false;
	}
	else
	{
		if ((this->returnTimeStep) <= motor.returnTimeStep)
			return true;
		return false;
	}
}
void Motorcycle::setID(int id) {
	ID = (id < 1000 && id>0) ? id : 0;
}
int Motorcycle::getID() {
	return ID;
}
void Motorcycle::setSpeed(int s) {
	speed = s;
}
int Motorcycle::getSpeed() {
	return speed;
}

bool Motorcycle::getServingStatus() const
{
	return serving;
}

void Motorcycle::setServingStatus(bool s)
{
	serving = s;
}
int Motorcycle::getReturnTimeStep() const
{
	return returnTimeStep;
}
void Motorcycle::setReturnTimeStep(int t)
{
	returnTimeStep = t;
}

void Motorcycle::setORDtype(ORD_TYPE ORDTYPE) {
	type = ORDTYPE;
}
ORD_TYPE Motorcycle::getORDtype() {
	return type;
}
void Motorcycle::setRegion(REGION r) {
	region = r;
}
REGION Motorcycle::getRegion() {
	return region;
}
void Motorcycle::setStatus(STATUS s) {
	status = s;
}
STATUS Motorcycle::getStatus() {
	return status;
}

Motorcycle::~Motorcycle()
{
}
