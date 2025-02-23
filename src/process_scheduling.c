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

/*
	result:
	float average_waiting_time;	 // the average waiting time in the ready queue until first schedue on the cpu
	float average_turnaround_time;  // the average completion time of the PCBs
	unsigned long total_run_time;   // the total time to process all the PCBs in the ready queue

	PCB:
	uint32_t remaining_burst_time;  // the remaining burst of the pcb
	uint32_t priority;				// The priority of the task
	uint32_t arrival;				// Time the process arrived in the ready queue
	bool started;			  		// If it has been activated on virtual CPU
	*/


//this function will schedule the tasks based on which has arrived first, and it does NOT exhibit preemptive behavior
bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	//check that input is valid and contains PCB's'
    if(ready_queue == NULL || result == NULL || ready_queue->data_size != sizeof(ProcessControlBlock_t))
    {
    	return false;
    }
    
    uint32_t clockTime = 0;
    uint32_t busyTime = 0;
    uint32_t waitTime = 0;
    
    bool fin = false;
    while(!fin)
    {
    	//initialize a current PCB and an iterator
    	ProcessControlBlock_t* cur = NULL;
    	uint8_t* iter = (uint8_t *) ready_queue->array;
    	
    	fin = true;
    
    	//store next shortest and valid job in cur and update fin;
    	for (size_t idx = 0; idx < ready_queue->size; ++idx, iter += ready_queue->data_size) 
    	{
    		ProcessControlBlock_t* walker = (ProcessControlBlock_t*) iter;
    		
    		//check if current walker is valid for comparison, updating fin or returning false if encountering invalid process
    		if(walker->remaining_burst_time > 0)
    			fin = false;
    		else if(!(walker->started))
    			return false;
    		else
    			continue;
    
    		//update current PCB if walker has arrived and has more recent arrival (or if the cur is NULL, so we can initialize a PCB)
    		if((cur == NULL || walker->arrival < cur->arrival) && walker->arrival <= clockTime)
    			cur = walker;
    	}
    	
    	//if you can't find anything valid, increment time, otherwise operate on the PCB
    	if(cur == NULL)
    	{
    		clockTime++;
    		busyTime++;
    	}
    	else
    	{
    		waitTime += (clockTime - cur->arrival);
    		cur->started = true;
    		while(cur->remaining_burst_time > 0)
    		{
    			virtual_cpu(cur);
    			clockTime++;
    		}
    	}
    }
    
    //decrement time once to account for the final wait after finishing
    clockTime--;
    busyTime--;
    
    //calculate the result values
    result->average_waiting_time = (float)waitTime / ready_queue->size;
    result->total_run_time = (unsigned long) clockTime - busyTime;
    result->average_turnaround_time = (float) result->average_waiting_time + ((float) result->total_run_time / ready_queue->size);
    
    return true;
}

//this function will schedule the tasks based on which has the shortest job, and it does NOT exhibit preemptive behavior
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	//check that input is valid and contains PCB's'
	if(ready_queue == NULL || result == NULL || ready_queue->data_size != sizeof(ProcessControlBlock_t))
	{
		return false;
	}

	uint32_t time = 0;
	uint32_t waitTime = 0;
	uint32_t turnaroundTime = 0;

	bool fin = false;

	while(!fin)
	{
		//initialize a current PCB and an iterator
		ProcessControlBlock_t* cur = NULL;
		uint8_t* iter = (uint8_t *) ready_queue->array;
		
		fin = true;

		//store next shortest and valid job in cur and update fin;
		for (size_t idx = 0; idx < ready_queue->size; ++idx, iter += ready_queue->data_size) 
		{
			ProcessControlBlock_t* walker = (ProcessControlBlock_t*) iter;
			
			//check if current walker is valid for comparison, updating fin or returning false if encountering invalid process
			if(walker->remaining_burst_time > 0)
				fin = false;
			else if(!(walker->started))
				return false;
			else
				continue;

			//update current PCB if walker has arrived and has shorter burst time (or if the cur is NULL, so we can initialize a PCB)
			if((cur == NULL || walker->remaining_burst_time < cur->remaining_burst_time) && walker->arrival <= time)
				cur = walker;
		}
		
		//if you can't find anything valid, increment time, otherwise operate on the PCB
		if(cur == NULL)
			time++;
		else
		{
			waitTime += time;
			cur->started = true;
			while(cur->remaining_burst_time > 0)
			{
				virtual_cpu(cur);
				time++;
			}
			turnaroundTime += time;
		}
	}

	//decrement time once to account for the final wait after finishing
	time--;

	//calculate the result values
	result->average_waiting_time = (float)waitTime / ready_queue->size;
	result->average_turnaround_time = (float)turnaroundTime / ready_queue->size;
	result->total_run_time = (unsigned long) time;
	
	return true;
}

//this function will schedule the tasks based on which has the lowest priority, and it does NOT exhibit preemptive behavior
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
	//check that input is valid and contains PCB's'
	if(ready_queue == NULL || result == NULL || ready_queue->data_size != sizeof(ProcessControlBlock_t))
	{
		return false;
	}

	uint32_t time = 0;
	uint32_t waitTime = 0;
	uint32_t turnaroundTime = 0;

	bool fin = false;

	while(!fin)
	{
		//initialize a current PCB and an iterator
		ProcessControlBlock_t* cur = NULL;
		uint8_t* iter = (uint8_t *) ready_queue->array;
		
		fin = true;

		//store next shortest and valid job in cur and update fin;
		for (size_t idx = 0; idx < ready_queue->size; ++idx, iter += ready_queue->data_size) 
		{
			ProcessControlBlock_t* walker = (ProcessControlBlock_t*) iter;
			
			//check if current walker is valid for comparison, updating fin or returning false if encountering invalid process
			if(walker->remaining_burst_time > 0)
				fin = false;
			else if(!(walker->started))
				return false;
			else
				continue;

			//update current PCB if walker has arrived and has a lower priority value (or if the cur is NULL, so we can initialize a PCB)
			if((cur == NULL || walker->priority < cur->priority) && walker->arrival <= time)
				cur = walker;
		}
		
		//if you can't find anything valid, increment time, otherwise operate on the PCB
		if(cur == NULL)
			time++;
		else
		{
			waitTime += time;
			cur->started = true;
			while(cur->remaining_burst_time > 0)
			{
				virtual_cpu(cur);
				time++;
			}
			turnaroundTime += time;
		}
	}

	//decrement time once to account for the final wait after finishing
	time--;

	//calculate the result values
	result->average_waiting_time = (float)waitTime / ready_queue->size;
	result->average_turnaround_time = (float)turnaroundTime / ready_queue->size;
	result->total_run_time = (unsigned long) time;
	
	return true;
}

// Runs the Round Robin Process Scheduling algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for round robin stat tracking \ref ScheduleResult_t
// \param the quantum
// \return true if function ran successful else false for an error
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
	UNUSED(ready_queue);
	UNUSED(result);
	UNUSED(quantum);
	return true;
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
	size_t read;

	unsigned char* buffer = (unsigned char*) malloc(sizeof(uint32_t)); 
	if (buffer == NULL) return NULL;


	// grab n = number of PCBs
	if ((read = fread(buffer, 4, 1, file)) > 0)
		memcpy(&n, buffer, sizeof(uint32_t)); 

	// create array with room for n PCBs
	dyn_array_t* arr = dyn_array_create((size_t)n, sizeof(ProcessControlBlock_t), NULL);
	ProcessControlBlock_t* pcb;

	// read PCB contents 
	while ((read = fread(buffer, 4, 1, file)) > 0){
		if (count == 0){
			pcb = (ProcessControlBlock_t*) malloc (sizeof(ProcessControlBlock_t));
			memcpy(&(pcb->remaining_burst_time), buffer, 4);
		}
		else if (count == 1)
			memcpy(&(pcb->priority), buffer, 4);
		else if (count == 2){
			memcpy(&(pcb->arrival), buffer, 4);
			dyn_array_push_front(arr, pcb);
		}

		if (count == 2) count = 0;
		else count++;
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
