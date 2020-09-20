#ifndef __MOTORCYCLE_H_
#define __MOTORCYCLE_H_


#include "..\Defs.h"

#pragma once
class Motorcycle
{
	int ID;
	ORD_TYPE type;	//for each order type there is a corresponding motorcycle type 
	int speed;		//meters it can move in one clock tick (in one timestep)
	REGION	region;	//region of the motorcycle
	STATUS	status;	//idle or in-service
	bool serving;
	int returnTimeStep; //Time step the motorcycle will return to restaurant in.
public:
	Motorcycle(int id, ORD_TYPE ty, int sp, REGION	Reg, STATUS	stat);
	bool operator <=(Motorcycle motor);
	bool operator <(Motorcycle motor);
	void setID(int id);
	int getID();
	void setSpeed(int s);
	int getSpeed();
	bool getServingStatus() const;
	void setServingStatus(bool s);
	void setORDtype(ORD_TYPE ORDTYPE);
	ORD_TYPE getORDtype();
	void setRegion(REGION r);
	REGION getRegion();
	void setStatus(STATUS s);
	STATUS getStatus();
	int getReturnTimeStep() const;
	void setReturnTimeStep(int t);
	virtual ~Motorcycle();
};

#endif