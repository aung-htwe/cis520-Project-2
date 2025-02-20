#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

int new_strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

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

	// grab quantum for round-robin
	size_t quantum = 0;
	if(argc == 4 && (*argv[3] >= '0' && *argv[3] <= '9')){
		quantum = quantum * 10 + (*argv[3] - '0');
	}

	// load ready queue from pcb file. If NULL, return error code -1
	dyn_array_t* readyQueue = load_process_control_blocks(fileName);
	if (readyQueue == NULL) return EXIT_FAILURE;

	ScheduleResult_t *result = (ScheduleResult_t*) malloc(sizeof(ScheduleResult_t));
	if(result == NULL) return EXIT_FAILURE;
    	if (new_strcmp(algorithm, FCFS) == 0)
		first_come_first_serve(readyQueue, result);
		else if(new_strcmp(algorithm, P) == 0)
		priority(readyQueue, result);
		else if(new_strcmp(algorithm, RR) == 0)
		round_robin(readyQueue, result, quantum);
		else if(new_strcmp(algorithm, SJF) == 0)
		shortest_job_first(readyQueue, result);

	FILE *fptr = fopen("README.md", "w");
	if(fptr == NULL)
	return EXIT_FAILURE;

	fprintf(fptr, "Average waiting time: %f\n", result->average_waiting_time);
	fprintf(fptr, "Average turnaround time: %f\n", result->average_turnaround_time);
	fprintf(fptr, "Total run time: %ld\n", result->total_run_time);

	fclose(fptr);
	free(result);
	dyn_array_destroy(readyQueue);
	return EXIT_SUCCESS;
}
