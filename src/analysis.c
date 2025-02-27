#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

// New and safer strcmp
// \param s1 first string to compare
// \param s2 second string to compare
// 0 if strings equal, >0 if s1 greater, <0 if s2 greater
int new_strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return (unsigned char)(*s1) - (unsigned char)(*s2);
}

// Main function for running analysis to grab stats on scheduling algorithms
// \param argc number of arguments in command line
// \param argv pointer to command line arguments
// return EXIT_SUCCESS if successful, EXIT_FAILURE if error
int main(int argc, char **argv) 
{
	// if insufficient amount of arguments, return EXIT_FAILURE
	if (argc < 3 || argc > 4) {
		printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
		return EXIT_FAILURE;
	}

	// grab filename of pcb
	char* fileName = argv[1];

	// grab scheduling algorithm used
	char* algorithm = argv[2];

	// grab quantum for round-robin. Must be valid quantum
	size_t quantum = 0;
	if(argc == 4 && (*argv[3] >= '0' && *argv[3] <= '9')){
		quantum = quantum * 10 + (*argv[3] - '0');
	}

	// load ready queue from pcb file. If NULL, return EXIT_FAILURE
	dyn_array_t* readyQueue = load_process_control_blocks(fileName);
	if (readyQueue == NULL) return EXIT_FAILURE;

	// create schedule result struct. If unsuccessful, return EXIT_FAILURE
	ScheduleResult_t *result = (ScheduleResult_t*) malloc(sizeof(ScheduleResult_t));
	if(result == NULL) return EXIT_FAILURE;

	// change directory to parent folder
	chdir("..");

	// Open file to print out to
	FILE *fptr = fopen("README.md", "a");
	if (fptr == NULL) return EXIT_FAILURE;

	// determine what algorithm was selected and fill result with respective algorithm's stats
	if (new_strcmp(algorithm, FCFS) == 0){
		first_come_first_serve(readyQueue, result);
		fprintf(fptr, "Results for First Come First Serve...................\n");
	}
	else if(new_strcmp(algorithm, P) == 0){
		priority(readyQueue, result);
		fprintf(fptr, "Results for Priority.................................\n");
	}
	else if(new_strcmp(algorithm, RR) == 0){
		round_robin(readyQueue, result, quantum);
		fprintf(fptr, "Results for Round Robin..............................\n");
	}
	else if(new_strcmp(algorithm, SJF) == 0){
		shortest_job_first(readyQueue, result);
		fprintf(fptr, "Results for Shortest Job First.......................\n");
	}

	// print stats for scheduling algorithm
	fprintf(fptr, "Average waiting time: %f\n", result->average_waiting_time);
	fprintf(fptr, "Average turnaround time: %f\n", result->average_turnaround_time);
	fprintf(fptr, "Total run time: %ld\n\n\n", result->total_run_time);

	// close file, free result and array. Return success
	fclose(fptr);
	free(result);
	dyn_array_destroy(readyQueue);
	return EXIT_SUCCESS;
}
