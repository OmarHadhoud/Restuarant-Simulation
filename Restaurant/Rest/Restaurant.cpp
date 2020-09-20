#include <cstdlib>
#include <time.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include "..\Events\CancellationEvent.h"
#include "..\Events\PromotionEvent.h"


Restaurant::Restaurant()
{
	pGUI = NULL;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		InteractiveMode();
		break;
	case MODE_STEP:
		StepByStepMode();
		break;
	case MODE_SLNT:
		SilentMode();
		break;
	};

}



//////////////////////////////////  Event handling functions   /////////////////////////////
void Restaurant::AddEvent(Event* pE)	//adds a new event to the queue of events
{
	EventsQueue.enqueue(*pE);
}

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current time
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}



Restaurant::~Restaurant()
{
	//Deleting all orders, motorcycles when program terminates.
	for (int i = 0; i < 4; i++)
	{
		const VIPOrder ** vipOrders = VipOrders[i].toArray();
		for (int j = 0; j < VipOrders[i].getCount(); j++)
		{
			delete vipOrders[j];
		}
		const Order ** nOrders = NormalOrders[i].toArray();
		for (int j = 0; j < NormalOrders[i].getCount(); j++)
		{
			delete nOrders[j];
		}
		const Order ** fOrders = FrozenOrders[i].toArray();
		for (int j = 0; j < FrozenOrders[i].getCount(); j++)
		{
			delete fOrders[j];
		}
		const Motorcycle ** NrmMotors = AvNrmMotors[i].toArray();
		for (int j = 0; j < AvNrmMotors[i].getCount(); j++)
		{
			delete NrmMotors[j];
		}
		const Motorcycle ** VipMotors = AvVipMotors[i].toArray();
		for (int j = 0; j < AvVipMotors[i].getCount(); j++)
		{
			delete VipMotors[j];
		}
		const Motorcycle ** FrzMotors = AvFrzMotors[i].toArray();
		for (int j = 0; j < AvFrzMotors[i].getCount(); j++)
		{
			delete FrzMotors[j];
		}

	}
	delete pGUI;
}

void Restaurant::ReturnMotors(int CurrentTimeStep)
{
	if (CurrentTimeStep == -1)
		CurrentTimeStep = INT_MAX;
	//Checking if there's a motor that has returned
	Motorcycle* TmpMotor;
	for (int i = 0; i < 4; i++)
	{
		//Vip Motors returning
		while (nAvVipMotors[i].getEntry(1))
		{
			if (nAvVipMotors[i].getEntry(1)->getReturnTimeStep() > CurrentTimeStep)
				break;
			TmpMotor = nAvVipMotors[i].getEntry(1);
			TmpMotor->setServingStatus(false);
			AvVipMotors[i].insertSorted(*TmpMotor);
			nAvVipMotors[i].remove(1);
		}
		//Frozen Motors
		while (nAvFrzMotors[i].getEntry(1))
		{
			if (nAvFrzMotors[i].getEntry(1)->getReturnTimeStep() > CurrentTimeStep)
				break;
			TmpMotor = nAvFrzMotors[i].getEntry(1);
			TmpMotor->setServingStatus(false);
			AvFrzMotors[i].insertSorted(*TmpMotor);
			nAvFrzMotors[i].remove(1);
		}
		//Normal motors
		while (nAvNrmMotors[i].getEntry(1))
		{
			if (nAvNrmMotors[i].getEntry(1)->getReturnTimeStep() > CurrentTimeStep)
				break;
			TmpMotor = nAvNrmMotors[i].getEntry(1);
			TmpMotor->setServingStatus(false);
			AvNrmMotors[i].insertSorted(*TmpMotor);
			nAvNrmMotors[i].remove(1);
		}
	}

}

void Restaurant::ServeOrders(int CurrentTimeStep)
{

	//LOGIC TO SERVE ORDERS
	Motorcycle *TmpMotor;
	Order *TmpPtr;
	for (int i = 0; i < 4; i++) // for different regions
	{
		//Serve VIP orders first
		while (VipOrders[i].getCount())
		{

			if (AvVipMotors[i].getCount() > 0)
			{
				//Getting order to be served
				TmpPtr = VipOrders[i].getEntry(1);
				VipOrders[i].remove(1);
				//Getting motorcycle that will serve order
				TmpMotor = AvVipMotors[i].getEntry(1);
				oP[i] += " V" + to_string(TmpMotor->getID()) + "(V" + to_string(TmpPtr->GetID()) + string(") |"); // LastTimeStep actions string
				AvVipMotors[i].remove(1);
				//Moving motorcycle to list of motors out, and calling serve function.
				nAvVipMotors[i].insertSorted(*TmpMotor);
				TmpPtr->Serve(CurrentTimeStep, TmpMotor);
				OrdersOut.insertSorted(*TmpPtr);
				NoSV[i]++; // Increases the count of served orders
			}
			else if (AvNrmMotors[i].getCount() > 0)
			{
				//Getting order to be served
				TmpPtr = VipOrders[i].getEntry(1);
				VipOrders[i].remove(1);
				//Getting motorcycle that will serve order
				TmpMotor = AvNrmMotors[i].getEntry(1);
				oP[i] += " N" + to_string(TmpMotor->getID()) + "(V" + to_string(TmpPtr->GetID()) + string(") |"); // LastTimeStep actions string
				AvNrmMotors[i].remove(1);
				//Moving motorcycle to list of motors out, and calling serve function.
				nAvNrmMotors[i].insertSorted(*TmpMotor);
				TmpPtr->Serve(CurrentTimeStep, TmpMotor);
				OrdersOut.insertSorted(*TmpPtr);
				NoSV[i]++; // Increases the count of served orders
			}
			else if (AvFrzMotors[i].getCount() > 0)
			{

				//Getting order to be served
				TmpPtr = VipOrders[i].getEntry(1);
				VipOrders[i].remove(1);
				//Getting motorcycle that will serve order
				TmpMotor = AvFrzMotors[i].getEntry(1);
				oP[i] += " F" + to_string(TmpMotor->getID()) + "(V" + to_string(TmpPtr->GetID()) + string(") |"); // LastTimeStep actions string
				AvFrzMotors[i].remove(1);
				//Moving motorcycle to list of motors out, and calling serve function.
				nAvFrzMotors[i].insertSorted(*TmpMotor);
				TmpPtr->Serve(CurrentTimeStep, TmpMotor);
				OrdersOut.insertSorted(*TmpPtr);
				NoSV[i]++; // Increases the count of served orders
			}
			else
			{
				break; //No available motorcycles are found
			}
		}

		//Serve Frozen orders secondly
		while (FrozenOrders[i].getCount())
		{
			//Serve only if frozen motors are found.
			if (AvFrzMotors[i].getCount() > 0)
			{
				//Getting order to be served
				FrozenOrders[i].dequeue(TmpPtr);
				//Getting motorcycle that will serve order
				TmpMotor = AvFrzMotors[i].getEntry(1);
				oP[i] += " F" + to_string(TmpMotor->getID()) + "(F" + to_string(TmpPtr->GetID()) + string(") |"); // LastTimeStep actions string
				AvFrzMotors[i].remove(1);
				//Moving motorcycle to list of motors out, and calling serve function.
				nAvFrzMotors[i].insertSorted(*TmpMotor);
				TmpPtr->Serve(CurrentTimeStep, TmpMotor);
				OrdersOut.insertSorted(*TmpPtr);
				NoSF[i]++; // Increases the count of served orders

			}
			else
			{
				break; //No available motorcycles are found
			}
		}

		//Serve Normal orders at last
		while (NormalOrders[i].getCount())
		{

			if (AvNrmMotors[i].getCount() > 0)
			{
				//Getting order to be served
				TmpPtr = NormalOrders[i].getEntry(1);
				NormalOrders[i].remove(1);
				//Getting motorcycle that will serve order
				TmpMotor = AvNrmMotors[i].getEntry(1);
				oP[i] += " N" + to_string(TmpMotor->getID()) + "(N" + to_string(TmpPtr->GetID()) + string(") |"); // LastTimeStep actions string
				AvNrmMotors[i].remove(1);
				//Moving motorcycle to list of motors out, and calling serve function.
				nAvNrmMotors[i].insertSorted(*TmpMotor);
				TmpPtr->Serve(CurrentTimeStep, TmpMotor);
				OrdersOut.insertSorted(*TmpPtr);
				NoSN[i]++; // Increases the count of served orders

			}
			else if (AvVipMotors[i].getCount() > 0)
			{
				//Getting order to be served
				TmpPtr = NormalOrders[i].getEntry(1);
				NormalOrders[i].remove(1);
				//Getting motorcycle that will serve order
				TmpMotor = AvVipMotors[i].getEntry(1);
				oP[i] += " V" + to_string(TmpMotor->getID()) + "(N" + to_string(TmpPtr->GetID()) + string(") |"); // LastTimeStep actions string
				AvVipMotors[i].remove(1);
				//Moving motorcycle to list of motors out, and calling serve function.
				nAvVipMotors[i].insertSorted(*TmpMotor);
				TmpPtr->Serve(CurrentTimeStep, TmpMotor);
				OrdersOut.insertSorted(*TmpPtr);
				NoSN[i]++; // Increases the count of served orders

			}
			else
			{
				break; //No available motorcycles are found
			}
		}
	}
}

void Restaurant::InteractiveMode()
{
	bool done = false;

	Load();

	pGUI->UpdateInterface();

	char timestep[10];
	int CurrentTimeStep = 1;
	int NoV[4] = { 0 }, NoF[4] = { 0 }, NoN[4] = { 0 }; //counters for different order types
	int NoVM[4] = { 0 }, NoFM[4] = { 0 }, NoNM[4] = { 0 }; //counters for different motor types
														   //as long as events queue is not empty yet
	while (!EventsQueue.isEmpty() || !done)
	{

		//print current timestep
		itoa(CurrentTimeStep, timestep, 10);

		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		AutoPromoteOrders(CurrentTimeStep);
		for (int i = 0; i < 4; i++)
		{
			NoV[i] = 0;
			NoF[i] = 0;
			NoN[i] = 0;
		}
		//Let's draw all arrived orders by passing them to the GUI to draw
		for (int i = 0; i < 4; i++) //for different regions
		{
			for (int j = 1; j <= VipOrders[i].getCount(); j++)
			{
				pGUI->AddOrderForDrawing(VipOrders[i].getEntry(j));
				NoV[i]++;
			}
			const Order ** ListArr = FrozenOrders[i].toArray();
			for (int j = 0; j < FrozenOrders[i].getCount(); j++)
			{
				pGUI->AddOrderForDrawing(ListArr[j]);
				NoF[i]++;
			}
			ListArr = NormalOrders[i].toArray();
			for (int j = 0; j < NormalOrders[i].getCount(); j++)
			{

				pGUI->AddOrderForDrawing(ListArr[j]);
				NoN[i]++;
			}
			NoVM[i] = AvVipMotors[i].getCount();
			NoNM[i] = AvNrmMotors[i].getCount();
			NoFM[i] = AvFrzMotors[i].getCount();
		}

		pGUI->PrintMessage(timestep + string(":: #of waiting orders per region|  V:") + to_string(NoV[0]) + " " + to_string(NoV[1]) + " " + to_string(NoV[2]) + " " + to_string(NoV[3]) + "  | F:" + to_string(NoF[0]) + " " + to_string(NoF[1]) + " " + to_string(NoF[2]) +
			" " + to_string(NoF[3]) + "  | N:" + to_string(NoN[0]) + " " + to_string(NoN[1]) + " " + to_string(NoN[2]) + " " + to_string(NoN[3]) + "    || #of available motors per region|  V:" + to_string(NoVM[0]) + " " + to_string(NoVM[1]) + " " + to_string(NoVM[2]) + " "
			+ to_string(NoVM[3]) + "  | F:" + to_string(NoFM[0]) + " " + to_string(NoFM[1]) + " " + to_string(NoFM[2]) + " " + to_string(NoFM[3]) + "  | N:" + to_string(NoNM[0]) + " " + to_string(NoNM[1]) + " " + to_string(NoNM[2]) + " " + to_string(NoNM[3]));
		pGUI->PrintMessage(string("Total # of orders served for each region| V: ") + to_string(NoSV[0]) + " " + to_string(NoSV[1]) + " " + to_string(NoSV[2]) + " " + to_string(NoSV[3]) + "  | F:" + to_string(NoSF[0]) + " " + to_string(NoSF[1]) + " " + to_string(NoSF[2]) +
			" " + to_string(NoSF[3]) + "  | N:" + to_string(NoSN[0]) + " " + to_string(NoSN[1]) + " " + to_string(NoSN[2]) + " " + to_string(NoSN[3]), 1);
		pGUI->PrintMessage(oP[0] + oP[1] + oP[2] + oP[3], 2);
		oP[0] = "Last Time Step REG A:"; oP[1] = "| REG B:"; oP[2] = "| REG C:"; oP[3] = "| REG D:";
		pGUI->UpdateInterface();
		pGUI->ResetDrawingList(); //for next time step

		ReturnMotors(CurrentTimeStep);
		ServeOrders(CurrentTimeStep);

		done = true;
		for (int i = 0; i < 4; i++) //for different regions
		{
			if (VipOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
			else if (FrozenOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
			else if (NormalOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
		}

		CurrentTimeStep++;	//advance timestep
		pGUI->waitForClick();
	}
	ReturnMotors(-1);
	pGUI->PrintMessage(timestep + string(":: #of waiting orders per region|  V:") + to_string(NoV[0]) + " " + to_string(NoV[1]) + " " + to_string(NoV[2]) + " " + to_string(NoV[3]) + "  | F:" + to_string(NoF[0]) + " " + to_string(NoF[1]) + " " + to_string(NoF[2]) +
		" " + to_string(NoF[3]) + "  | N:" + to_string(NoN[0]) + " " + to_string(NoN[1]) + " " + to_string(NoN[2]) + " " + to_string(NoN[3]) + "    || #of available motors per region|  V:" + to_string(NoVM[0]) + " " + to_string(NoVM[1]) + " " + to_string(NoVM[2]) + " "
		+ to_string(NoVM[3]) + "  | F:" + to_string(NoFM[0]) + " " + to_string(NoFM[1]) + " " + to_string(NoFM[2]) + " " + to_string(NoFM[3]) + "  | N:" + to_string(NoNM[0]) + " " + to_string(NoNM[1]) + " " + to_string(NoNM[2]) + " " + to_string(NoNM[3]));
	pGUI->PrintMessage(string("Total # of orders served for each region| V: ") + to_string(NoSV[0]) + " " + to_string(NoSV[1]) + " " + to_string(NoSV[2]) + " " + to_string(NoSV[3]) + "  | F:" + to_string(NoSF[0]) + " " + to_string(NoSF[1]) + " " + to_string(NoSF[2]) +
		" " + to_string(NoSF[3]) + "  | N:" + to_string(NoSN[0]) + " " + to_string(NoSN[1]) + " " + to_string(NoSN[2]) + " " + to_string(NoSN[3]), 1);
	pGUI->PrintMessage(oP[0] + oP[1] + oP[2] + oP[3], 2);
	oP[0] = "Last Time Step REG A:"; oP[1] = "| REG B:"; oP[2] = "| REG C:"; oP[3] = "| REG D:";
	pGUI->UpdateInterface();
	pGUI->waitForClick();
	SaveOutput();
	pGUI->PrintMessage("Simulation done, click to END program");
	pGUI->UpdateInterface();
	pGUI->waitForClick();

}
void Restaurant::StepByStepMode()
{
	bool done = false;

	Load();

	pGUI->UpdateInterface();

	char timestep[10];
	int NoV[4] = { 0 }, NoF[4] = { 0 }, NoN[4] = { 0 }; //counters for different order types
	int NoVM[4] = { 0 }, NoFM[4] = { 0 }, NoNM[4] = { 0 }; //counters for different motor types
	int CurrentTimeStep = 1;
	//as long as events queue is not empty yet
	while (!EventsQueue.isEmpty() || !done)
	{
		//print current timestep
		itoa(CurrentTimeStep, timestep, 10);
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		AutoPromoteOrders(CurrentTimeStep);
		for (int i = 0; i < 4; i++)
		{
			NoV[i] = 0;
			NoF[i] = 0;
			NoN[i] = 0;
		}
		for (int i = 0; i < 4; i++) //for different regions
		{
			for (int j = 1; j <= VipOrders[i].getCount(); j++)
			{
				pGUI->AddOrderForDrawing(VipOrders[i].getEntry(j));
				NoV[i]++;
			}
			const Order ** ListArr = FrozenOrders[i].toArray();
			for (int j = 0; j < FrozenOrders[i].getCount(); j++)
			{
				pGUI->AddOrderForDrawing(ListArr[j]);
				NoF[i]++;
			}
			ListArr = NormalOrders[i].toArray();
			for (int j = 0; j < NormalOrders[i].getCount(); j++)
			{

				pGUI->AddOrderForDrawing(ListArr[j]);
				NoN[i]++;
			}
			NoVM[i] = AvVipMotors[i].getCount();
			NoNM[i] = AvNrmMotors[i].getCount();
			NoFM[i] = AvFrzMotors[i].getCount();
		}

		pGUI->PrintMessage(timestep + string(":: #of waiting orders per region|  V:") + to_string(NoV[0]) + " " + to_string(NoV[1]) + " " + to_string(NoV[2]) + " " + to_string(NoV[3]) + "  | F:" + to_string(NoF[0]) + " " + to_string(NoF[1]) + " " + to_string(NoF[2]) +
			" " + to_string(NoF[3]) + "  | N:" + to_string(NoN[0]) + " " + to_string(NoN[1]) + " " + to_string(NoN[2]) + " " + to_string(NoN[3]) + "    || #of available motors per region|  V:" + to_string(NoVM[0]) + " " + to_string(NoVM[1]) + " " + to_string(NoVM[2]) + " "
			+ to_string(NoVM[3]) + "  | F:" + to_string(NoFM[0]) + " " + to_string(NoFM[1]) + " " + to_string(NoFM[2]) + " " + to_string(NoFM[3]) + "  | N:" + to_string(NoNM[0]) + " " + to_string(NoNM[1]) + " " + to_string(NoNM[2]) + " " + to_string(NoNM[3]));
		pGUI->PrintMessage(string("Total # of orders served for each region| V: ") + to_string(NoSV[0]) + " " + to_string(NoSV[1]) + " " + to_string(NoSV[2]) + " " + to_string(NoSV[3]) + "  | F:" + to_string(NoSF[0]) + " " + to_string(NoSF[1]) + " " + to_string(NoSF[2]) +
			" " + to_string(NoSF[3]) + "  | N:" + to_string(NoSN[0]) + " " + to_string(NoSN[1]) + " " + to_string(NoSN[2]) + " " + to_string(NoSN[3]), 1);
		pGUI->PrintMessage(oP[0] + oP[1] + oP[2] + oP[3], 2);
		oP[0] = "Last Time Step REG A:"; oP[1] = "| REG B:"; oP[2] = "| REG C:"; oP[3] = "| REG D:";
		pGUI->UpdateInterface();
		pGUI->ResetDrawingList(); //for next time step

		ReturnMotors(CurrentTimeStep);
		ServeOrders(CurrentTimeStep);

		done = true;
		for (int i = 0; i < 4; i++) //for different regions
		{
			if (VipOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
			else if (FrozenOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
			else if (NormalOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
		}

		CurrentTimeStep++;	//advance timestep
		Sleep(1000);
	}
	ReturnMotors(-1);
	pGUI->PrintMessage(timestep + string(":: #of waiting orders per region|  V:") + to_string(NoV[0]) + " " + to_string(NoV[1]) + " " + to_string(NoV[2]) + " " + to_string(NoV[3]) + "  | F:" + to_string(NoF[0]) + " " + to_string(NoF[1]) + " " + to_string(NoF[2]) +
		" " + to_string(NoF[3]) + "  | N:" + to_string(NoN[0]) + " " + to_string(NoN[1]) + " " + to_string(NoN[2]) + " " + to_string(NoN[3]) + "    || #of available motors per region|  V:" + to_string(NoVM[0]) + " " + to_string(NoVM[1]) + " " + to_string(NoVM[2]) + " "
		+ to_string(NoVM[3]) + "  | F:" + to_string(NoFM[0]) + " " + to_string(NoFM[1]) + " " + to_string(NoFM[2]) + " " + to_string(NoFM[3]) + "  | N:" + to_string(NoNM[0]) + " " + to_string(NoNM[1]) + " " + to_string(NoNM[2]) + " " + to_string(NoNM[3]));
	pGUI->PrintMessage(string("Total # of orders served for each region| V: ") + to_string(NoSV[0]) + " " + to_string(NoSV[1]) + " " + to_string(NoSV[2]) + " " + to_string(NoSV[3]) + "  | F:" + to_string(NoSF[0]) + " " + to_string(NoSF[1]) + " " + to_string(NoSF[2]) +
		" " + to_string(NoSF[3]) + "  | N:" + to_string(NoSN[0]) + " " + to_string(NoSN[1]) + " " + to_string(NoSN[2]) + " " + to_string(NoSN[3]), 1);
	pGUI->PrintMessage(oP[0] + oP[1] + oP[2] + oP[3], 2);
	oP[0] = "Last Time Step REG A:"; oP[1] = "| REG B:"; oP[2] = "| REG C:"; oP[3] = "| REG D:";
	pGUI->UpdateInterface();
	Sleep(1000);
	SaveOutput();
	pGUI->PrintMessage("Simulation done, click to END program");
	pGUI->waitForClick();

}
void Restaurant::SilentMode()
{
	bool done = false;
	Load();

	int CurrentTimeStep = 1;
	//as long as events queue is not empty yet
	while (!EventsQueue.isEmpty() || !done)
	{
		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step
		AutoPromoteOrders(CurrentTimeStep);
		ReturnMotors(CurrentTimeStep);
		ServeOrders(CurrentTimeStep);
		done = true;
		for (int i = 0; i < 4; i++) //for different regions
		{
			if (VipOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
			else if (FrozenOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
			else if (NormalOrders[i].getCount() > 0)
			{
				done = false;
				break;
			}
		}
		CurrentTimeStep++;	//advance timestep
	}
	ReturnMotors(-1);
	SaveOutput();
	pGUI->PrintMessage("Simulation done, click to END program");
	pGUI->waitForClick();

}

void Restaurant::Load()
{

	Event* pEv;



	pGUI->PrintMessage("Reading I/P Filename, Please insert file name");

	FileName = pGUI->GetString() + ".txt";
	pGUI->UpdateInterface();

	InputFile.open(FileName);

	//check the file
	while (!InputFile.is_open())
	{
		pGUI->PrintMessage("No file found with name: " + FileName + "| Please re-enter: ");
		FileName = pGUI->GetString() + ".txt";
		pGUI->UpdateInterface();
		InputFile.open(FileName);
	}

	pGUI->PrintMessage("Your file has been successfully loaded");
	if (InputFile.is_open())
	{


		int NumberOfMotorCycles;
		InputFile >> NumberOfMotorCycles;

		for (int i = 0; i < NumberOfMotorCycles; i++)
		{
			char MCType;
			char FileRegion;
			int ID_Motorcycle;
			ORD_TYPE OrderType;
			int SpeedOfMotorCycle;
			REGION MCRegion;

			InputFile >> MCType;

			if (MCType == 'V')
			{
				OrderType = TYPE_VIP;
			}
			else if (MCType == 'F')
			{
				OrderType = TYPE_FROZ;
			}
			else if (MCType == 'N')
			{
				OrderType = TYPE_NRM;
			}

			InputFile >> ID_Motorcycle;
			InputFile >> SpeedOfMotorCycle;
			InputFile >> FileRegion;

			if (FileRegion == 'A')
			{
				MCRegion = A_REG;
			}
			else if (FileRegion == 'B')
			{
				MCRegion = B_REG;
			}
			else if (FileRegion == 'C')
			{
				MCRegion = C_REG;
			}
			else if (FileRegion == 'D')
			{
				MCRegion = D_REG;
			}

			Motorcycle* ToBeAdded = new Motorcycle(ID_Motorcycle, OrderType, SpeedOfMotorCycle, MCRegion, IDLE);

			if (OrderType == TYPE_VIP)
			{
				AvVipMotors[MCRegion].insertSorted(*ToBeAdded);
			}
			else if (OrderType == TYPE_FROZ)
			{
				AvFrzMotors[MCRegion].insertSorted(*ToBeAdded);
			}
			else if (OrderType == TYPE_NRM)
			{
				AvNrmMotors[MCRegion].insertSorted(*ToBeAdded);
			}
		}

		int AutoS; // Time taken to be promoted after waiting 
		InputFile >> AutoS;
		AutoPT = AutoS;
		int EventCount;
		InputFile >> EventCount;

		for (int i = 0; i < EventCount; i++)
		{
			char EventType;
			InputFile >> EventType;
			if (EventType == 'R')
			{
				int ArrivalTimeStep;
				InputFile >> ArrivalTimeStep;
				char TypeOfOrder;
				ORD_TYPE OType;
				InputFile >> TypeOfOrder;

				if (TypeOfOrder == 'N')
				{
					OType = TYPE_NRM;
				}
				if (TypeOfOrder == 'V')
				{
					OType = TYPE_VIP;
				}
				if (TypeOfOrder == 'F')
				{
					OType = TYPE_FROZ;
				}

				int O_id;
				InputFile >> O_id;
				int Distance;
				InputFile >> Distance;
				double MoneyPaidForOrder;
				InputFile >> MoneyPaidForOrder;
				char Region;
				REGION reg;
				InputFile >> Region;

				if (Region == 'A')
				{
					reg = A_REG;
				}
				if (Region == 'B')
				{
					reg = B_REG;
				}

				if (Region == 'C')
				{
					reg = C_REG;
				}

				if (Region == 'D')
				{
					reg = D_REG;
				}

				pEv = new ArrivalEvent(ArrivalTimeStep, O_id, (ORD_TYPE)OType, (REGION)reg, MoneyPaidForOrder, Distance);
				AddEvent(pEv);
			}
			if (EventType == 'X')
			{
				int ArrivalTimeStep;
				InputFile >> ArrivalTimeStep;
				int O_id;
				InputFile >> O_id;

				pEv = new CancellationEvent(ArrivalTimeStep, O_id);

				AddEvent(pEv);
			}
			if (EventType == 'P')
			{


				int ArrivalTimeStep;
				InputFile >> ArrivalTimeStep;
				int O_id;
				InputFile >> O_id;
				int ExtraMoney;
				InputFile >> ExtraMoney;


				pEv = new PromotionEvent(ArrivalTimeStep, O_id, ExtraMoney);
				//Extra money is added to the money on the order's already paid money (obvious)

				AddEvent(pEv);
			}
		}
		//Close the file 
		InputFile.close();
	}

}

void Restaurant::SaveOutput()
{
	Order* pOrd;

	//---------------------------------------------------------------------------------------//



	int CountOrders[5] = { 0,0,0,0,0 };
	int CountNormOrders[5] = { 0,0,0,0,0 };
	int CountFrozenOrders[5] = { 0,0,0,0,0 };
	int CountVIPOrders[5] = { 0,0,0,0,0 };

	int CountMC[5] = { 0,0,0,0,0 };
	int CountNormMC[5] = { 0,0,0,0,0 };
	int CountFrozenMC[5] = { 0,0,0,0,0 };
	int CountVIPMC[5] = { 0,0,0,0,0 };

	double AvgServ[5] = { 0,0,0,0,0 };
	double AvgWait[5] = { 0,0,0,0,0 };

	pGUI->PrintMessage("Now Outputting to file, Please insert file name");
	FileName = pGUI->GetString() + ".txt";
	pGUI->UpdateInterface();
	OutputFile.open(FileName);

	if (OutputFile.is_open())
	{
		OutputFile << "FT" << '\t' << "ID" << '\t' << "AT" << '\t' << "WT" << '\t' << "ST" << endl;
		while (!OrdersOut.isEmpty())
		{
			pOrd = OrdersOut.getEntry(1);
			int FT, ID, AT, WT, ST;
			FT = pOrd->GetFinishTime();
			ID = pOrd->GetID();
			AT = pOrd->GetArrTime();
			WT = pOrd->GetWaitTime();
			ST = pOrd->GetServTime();

			AvgServ[4] = AvgServ[4] + ST;
			AvgWait[4] = AvgWait[4] + WT;

			OutputFile << FT << '\t' << ID << '\t' << AT << '\t' << WT << '\t' << ST << endl;

			if (pOrd->GetType() == TYPE_NRM)  // Norm_Orders
			{
				CountOrders[4]++;
				CountNormOrders[4]++;
				if (pOrd->GetRegion() == A_REG)
				{
					CountOrders[0]++;
					CountNormOrders[0]++;

					AvgServ[0] = AvgServ[0] + ST;
					AvgWait[0] = AvgWait[0] + WT;
				}
				else if (pOrd->GetRegion() == B_REG)
				{
					CountOrders[1]++;
					CountNormOrders[1]++;

					AvgServ[1] = AvgServ[1] + ST;
					AvgWait[1] = AvgWait[1] + WT;
				}
				else if (pOrd->GetRegion() == C_REG)
				{
					CountOrders[2]++;
					CountNormOrders[2]++;

					AvgServ[2] = AvgServ[2] + ST;
					AvgWait[2] = AvgWait[2] + WT;
				}
				else if (pOrd->GetRegion() == D_REG)
				{
					CountOrders[3]++;
					CountNormOrders[3]++;

					AvgServ[3] = AvgServ[3] + ST;
					AvgWait[3] = AvgWait[3] + WT;
				}
			}
			else if (pOrd->GetType() == TYPE_FROZ) // Frozen_Orders
			{
				CountOrders[4]++;
				CountFrozenOrders[4]++;
				if (pOrd->GetRegion() == A_REG)
				{
					CountOrders[0]++;
					CountFrozenOrders[0]++;

					AvgServ[0] = AvgServ[0] + ST;
					AvgWait[0] = AvgWait[0] + WT;
				}
				else if (pOrd->GetRegion() == B_REG)
				{
					CountOrders[1]++;
					CountFrozenOrders[1]++;

					AvgServ[1] = AvgServ[1] + ST;
					AvgWait[1] = AvgWait[1] + WT;
				}
				else if (pOrd->GetRegion() == C_REG)
				{
					CountOrders[2]++;
					CountFrozenOrders[2]++;

					AvgServ[2] = AvgServ[2] + ST;
					AvgWait[2] = AvgWait[2] + WT;
				}
				else if (pOrd->GetRegion() == D_REG)
				{
					CountOrders[3]++;
					CountFrozenOrders[3]++;

					AvgServ[3] = AvgServ[3] + ST;
					AvgWait[3] = AvgWait[3] + WT;
				}
			}
			else if (pOrd->GetType() == TYPE_VIP) // Vip_Orders
			{
				CountOrders[4]++;
				CountVIPOrders[4]++;
				if (pOrd->GetRegion() == A_REG)
				{
					CountOrders[0]++;
					CountVIPOrders[0]++;

					AvgServ[0] = AvgServ[0] + ST;
					AvgWait[0] = AvgWait[0] + WT;
				}
				else if (pOrd->GetRegion() == B_REG)
				{
					CountOrders[1]++;
					CountVIPOrders[1]++;

					AvgServ[1] = AvgServ[1] + ST;
					AvgWait[1] = AvgWait[1] + WT;
				}
				else if (pOrd->GetRegion() == C_REG)
				{
					CountOrders[2]++;
					CountVIPOrders[2]++;

					AvgServ[2] = AvgServ[2] + ST;
					AvgWait[2] = AvgWait[2] + WT;
				}
				else if (pOrd->GetRegion() == D_REG)
				{
					CountOrders[3]++;
					CountVIPOrders[3]++;

					AvgServ[3] = AvgServ[3] + ST;
					AvgWait[3] = AvgWait[3] + WT;
				}
			}

			OrdersOut.remove(1);
			pOrd = NULL;
		}
		for (int i = 0; i < 5; i++)
		{
			AvgServ[i] = AvgServ[i] / (double)CountOrders[i];
			if (!(AvgServ[i] == AvgServ[i]))
			{
				AvgServ[i] = 0;
			}
			AvgWait[i] = AvgWait[i] / (double)CountOrders[i];
			if (!(AvgWait[i] == AvgWait[i]))
			{
				AvgWait[i] = 0;
			}

			if (i < 4)
			{
				CountNormMC[i] = AvNrmMotors[i].getCount();
				CountVIPMC[i] = AvVipMotors[i].getCount();
				CountFrozenMC[i] = AvFrzMotors[i].getCount();
				CountMC[i] = CountFrozenMC[i] + CountVIPMC[i] + CountNormMC[i];
			}
			else if (i == 4)
			{
				for (int j = 0; j < 4; j++)
				{
					CountMC[i] = CountMC[i] + CountMC[j];
					CountNormMC[i] = CountNormMC[i] + CountNormMC[j];
					CountVIPMC[i] = CountVIPMC[i] + CountVIPMC[j];
					CountFrozenMC[i] = CountFrozenMC[i] + CountFrozenMC[j];
				}
			}

		}
		OutputFile << "--------------------------------------------------------------------------------------------------" << endl;
		OutputFile << "--------------------------------------------------------------------------------------------------" << endl;
		for (int i = 0; i < 5; i++)
		{
			// Outputting Regions and restaurant
			if (i == 0)
			{
				OutputFile << "Region A:" << endl;
			}
			if (i == 1)
			{
				OutputFile << "Region B:" << endl;
			}
			if (i == 2)
			{
				OutputFile << "Region C:" << endl;
			}
			if (i == 3)
			{
				OutputFile << "Region D:" << endl;
			}
			if (i == 4)
			{
				OutputFile << "Restaurant:" << endl;
			}
			OutputFile << "    " << "Orders: " << CountOrders[i] << "[Norm:" << CountNormOrders[i] << "," << " Froz:" << CountFrozenOrders[i] << ","
				<< " VIP:" << CountVIPOrders[i] << "]" << endl;
			OutputFile << "    " << "MotorC: " << CountMC[i] << "[Norm:" << CountNormMC[i] << "," << " Froz:" << CountFrozenMC[i] << ","
				<< " VIP:" << CountVIPMC[i] << "]" << endl;
			OutputFile << "    " << "Avg Wait = " << AvgWait[i] << ", " << "Avg Serv = " << AvgServ[i] << endl;
		}

		//------------------------------------------------------------------------------------------------------------------------------//

		//Close the file 
		OutputFile.close();
	}
}

//Adds the order to the appropriate list.
void Restaurant::AddToOrders(Order* po)
{
	//Getting the type of the order.
	switch (po->GetType())
	{
	case TYPE_VIP:
	{
		VIPOrder * vpo = dynamic_cast<VIPOrder*>(po);
		VipOrders[po->GetRegion()].insertSorted(*vpo);
	}
	break;
	case TYPE_FROZ:
	{
		FrozenOrders[po->GetRegion()].enqueue(*po);
	}
	break;
	case TYPE_NRM:
	{
		NormalOrders[po->GetRegion()].insert(NormalOrders[po->GetRegion()].getCount() + 1, *po);
	}
	break;
	default:
		break;
	}
}

//A function to cancel an order if found, return true if found and cancelled, false if not found.
bool Restaurant::CancelOrder(int id, int Reg, int idx)
{
	//Region is not known, assume region A just for the constructor to work.
	Order* Srch;
	bool isFound = false;
	for (int i = Reg; i < 4; i++)
	{
		int c = NormalOrders[i].getCount();
		const Order ** ListArr = NormalOrders[i].toArray();
		for (int j = idx; j < c; j++)
		{
			if (ListArr[j]->GetID() == id)
			{
				Order* Del = NormalOrders[i].getEntry(j + 1);
				delete Del;
				return NormalOrders[i].remove(j + 1); //1 based
				isFound = true;
			}

		}
	}
	return isFound;

}
bool Restaurant::PromoteOrder(int ID, int Ex_MON, int Reg, int idx) {

	for (int i = Reg; i < 4; i++)
	{
		int c = NormalOrders[i].getCount();
		const Order ** ListArr = NormalOrders[i].toArray();
		for (int j = idx; j < c; j++)
		{
			if (ListArr[j]->GetID() == ID)
			{
				VIPOrder* Prom_Order = new VIPOrder(*ListArr[j], Ex_MON);
				CancelOrder(ID, i, j); //Given which region and order to look into
				AddToOrders(Prom_Order);
				return true;
			}
		}
	}
	return false;
}


bool Restaurant::AutoPromoteOrders(int ct) {
	bool ispromoted = false;
	for (int i = 0; i < 4; i++) {

		int c = NormalOrders[i].getCount();
		int nC = 0; //Ammout of promoted orders
		const Order ** ListArr = NormalOrders[i].toArray();
		for (int j = 0; j < c; j++)
		{
			if ((ct - ListArr[j]->GetArrTime()) >= AutoPT)
			{
				ispromoted = PromoteOrder(ListArr[j]->GetID(), 0, i, j - nC);
				nC += ispromoted;
			}
		}
	}
	return ispromoted;

}