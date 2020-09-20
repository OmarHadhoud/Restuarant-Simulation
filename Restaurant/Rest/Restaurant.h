#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include "..\Generic_DS\SortedList.h"
#include "..\Generic_DS\LinkedList.h"

#include <fstream>
#include <ostream>
#include "Motorcycle.h"
#include "Order.h"
#include "VIPOrder.h"
// it is the maestro of the project
class Restaurant
{
private:
	GUI * pGUI;
	Queue<Event> EventsQueue;	//Queue of all events that will be loaded from file

	ifstream InputFile;
	ofstream OutputFile;
	string FileName;

	int AutoPT;

	SortedList<VIPOrder, false> VipOrders[4]; //List of VIP order lists that is sorted descendingly. Array as every element is a list for one region.
	LinkedList<Order> NormalOrders[4]; //Queue of Normal order lists. Queue as every element is a list for one region.
	Queue<Order> FrozenOrders[4]; //Queue of Frozen order lists. Queue as every element is a list for one region.

	SortedList<Motorcycle> AvVipMotors[4]; //List of available to use VIP motorcycles.
	SortedList<Motorcycle> AvNrmMotors[4]; //List of available to use Normal motorcycles.
	SortedList<Motorcycle> AvFrzMotors[4]; //List of available to use Frozen motorcycles.

	//Lists that we need when orders are actually being served.
	
	SortedList<Motorcycle> nAvVipMotors[4]; //Motorcycles that aren't available, as they are serving orders.
	SortedList<Motorcycle> nAvNrmMotors[4]; //Motorcycles that aren't available, as they are serving orders.
	SortedList<Motorcycle> nAvFrzMotors[4]; //Motorcycles that aren't available, as they are serving orders.

	SortedList<Order> OrdersOut; //List of orders that are out to be served. (for output file purposes)
	string oP[4] = { "Last Time Step REG A:", "| REG B:", "| REG C:", "| REG D:" };

	int NoSV[4] = { 0 }, NoSF[4] = { 0 }, NoSN[4] = { 0 }; //counters for different served order types
	void InteractiveMode();
	void StepByStepMode();
	void SilentMode();
	void ServeOrders(int currentTime);
	void ReturnMotors(int currentTime);

public:

	Restaurant();
	~Restaurant();
	void AddEvent(Event* pE);	//adds a new event to the queue of events
	void ExecuteEvents(int TimeStep);	//executes all events at current timestep
	void RunSimulation();

	void Load(); 
	void SaveOutput();

	void AddToOrders(Order* po); //Adds an order to the appropriate list.
	bool CancelOrder(int id, int Reg=0 ,int idx= 0); //Cancels and order with it's id.
	bool PromoteOrder(int ID, int Ex_MON, int Reg =0, int idx =0);
	bool AutoPromoteOrders(int ct);

};

#endif