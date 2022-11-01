#include "bsw.h"


#define CNT_RUNNABLE 11
#define CNT_TASK 3

// Runnable Bit Representation
#define RUNNABLE1 0b00000000001;
#define RUNNABLE2 0b00000000010;
#define RUNNABLE3 0b00000000100;
#define RUNNABLE4 0b00000001000;
#define RUNNABLE5 0b00000010000;
#define RUNNABLE6 0b00000100000;
#define RUNNABLE7 0b00001000000;
#define RUNNABLE8 0b00010000000;
#define RUNNABLE9 0b00100000000;
#define RUNNABLE10 0b01000000000;
#define RUNNABLE11 0b10000000000;


void StartupHook(void)
{
	// Runnable's execution time
	static const unsigned long g_kRunnables_execution_times[CNT_RUNNABLE] = { 100, 200, 100, 200, 300, 300, 100, 200, 200, 300, 100 };

	// Task's Period
	static const unsigned long g_kTasks_periods[CNT_TASK] = { 5000, 8000, 100000 };


	static unsigned long* g_Runnables_data[CNT_RUNNABLE];
	for (int i = 0; i < CNT_RUNNABLE; i++) {
		g_Runnables_data = (unsigned long*)malloc(sizeof(unsigned int) * CNT_RUNNABLE);
	}
}

TASK(Task_5s)
{
	printfSerial("Task 1 begins... \n");
	Runnable_5();
	Runnable_1();
	printfSerial("Task 1 ends... \n");
    TerminateTask();
}

TASK(Task_8s)
{
	printfSerial("Task 2 begins... \n");
	Runnable_2();
	Runnable_3();
	printfSerial("Task 2 ends... \n");
	TerminateTask();
}

TASK(Task_10s)
{
	printfSerial("Task 3 begins... \n");
	Runnable_4();
	Runnable_6();
	Runnable_7();
	printfSerial("Task 3 ends... \n");
	TerminateTask();
}

void RunRunnable1(void)
{
	//Execute Flag

	//Execute
	mdelay(R1_exeT);

	//Timing add
	//Flow1 += R1_exeT;
	//Flow2 += R1_exeT;
	//Flow3 += R1_exeT;
	printfSerial("Runnable 1\n");
}

void RunRunnable2(void)
{
	R2 = true;
	//Flow1 += R2_exeT;
	//Flow2 += R2_exeT;
	//Flow3 += R2_exeT;
	mdelay(R2_exeT);
	printfSerial("Runnable 2\n");
}

void RunRunnable3(void)
{
	R3 = true;
	//Flow1 += R3_exeT;
	//Flow2 += R3_exeT;
	//Flow3 += R3_exeT;
	mdelay(R3_exeT);
	printfSerial("Runnable 3\n");
}

void RunRunnable4(void)
{
	R4 = true;
	//Flow1 += R4_exeT;
	//Flow2 += R4_exeT;
	//Flow3 += R4_exeT;
	mdelay(R4_exeT);
	printfSerial("Runnable 4\n");

}

void RunRunnable5(void)
{
	if (R1) R5 = true;
	//Flow1 += R5_exeT;
	//Flow2 += R5_exeT;
	//Flow3 += R5_exeT;
	mdelay(R5_exeT);
	printfSerial("Runnable 5\n");
}

void RunRunnable6(void)
{

	if (R5) R6 = true;
	//Flow1 += R6_exeT;
	//Flow2 += R6_exeT;
	//Flow3 += R6_exeT;
	mdelay(R6_exeT);
	printfSerial("Runnable 6\n");

}

void RunRunnable7(void)
{

}
