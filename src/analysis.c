#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
	if (argc < 3) 
	{
		printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
		return EXIT_FAILURE;
	}

	// grab filename of pcb
	char* fileName = argv[1];

	// grab scheduling algorithm used
	char* algorithm = argv[2];

	// load ready queue from pcb file. If NULL, return error code -1
	dyn_array_t* readyQueue = load_process_control_blocks(fileName);
	if (readyQueue == NULL) return -1;

	ScheduleResult_t *result = (ScheduleResult_t*) malloc(sizeof(ScheduleResult_t));


        if (strcmp(algorithm, "FCFS") == 0)
		first_come_first_serve(readyQueue, result);

	printf("Average waiting time: %f\n", result->average_waiting_time);
	printf("Average turnaround time: %f\n", result->average_turnaround_time);
	printf("Total run time: %ld\n", result->total_run_time);

	return EXIT_SUCCESS;
}
