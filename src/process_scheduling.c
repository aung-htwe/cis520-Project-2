#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
	// decrement the burst time of the pcb
	--process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	UNUSED(quantum);
	return false;
}

dyn_array_t *load_process_control_blocks(const char *input_file) 
{
	// check if file is NULL or bad file name
	if (input_file == NULL) return NULL;
	if (strchr(input_file, '\n') != NULL || strcmp(input_file, "\0") == 0 || strcmp(input_file, "") == 0) return NULL;

	// attempt to open input file. If error, return NULL
	FILE* file = fopen(input_file, "rb");
	if (file == NULL) return NULL;

	uint32_t n = 0;
	int count = 0;

	char* buffer = (char*) malloc(sizeof(uint32_t));
	if (buffer == NULL) return NULL;

	// grab n = number of PCBs
	if (fgets(buffer, sizeof(uint32_t), file) != NULL)
		memcpy(&n, buffer, sizeof(uint32_t));

	// create array with room for n PCBs
	dyn_array_t* arr = dyn_array_create((size_t)n, sizeof(ProcessControlBlock_t), NULL);

	// read burst times 
	while (fgets(buffer, sizeof(uint32_t), file) != NULL){
		if (count % 3 == 0){
			ProcessControlBlock_t pcb = { .remaining_burst_time = *(uint32_t *)&buffer };
			dyn_array_push_front(arr, (const void*)&pcb);
		}
		count++;
	}

	free(buffer);

	return arr;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	return false;
}
