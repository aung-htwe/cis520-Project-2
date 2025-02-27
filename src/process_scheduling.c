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


// this function will schedule the tasks based on which has arrived first, and it does NOT exhibit preemptive behavior
// \param ready queue a dyn_array of type ProcessControlBlock_t that contains up to N elements
// \param result used for the first come first serve stat tracking \ref ScheduleResult_t
// \return true if function ran successful else false for an error
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

// this function will schedule the tasks based on which has the shortest job, and it does NOT exhibit preemptive behavior
// \param ready queue a dyn_array of type ProcessControlBlock_t that contains up to N elements
// \param result used for the shortest job first stat tracking \ref ScheduleResult_t
// \return true if function ran successful, else false for an error
bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
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
    
    		//update current PCB if walker has arrived and has a shorter burst (or if the cur is NULL, so we can initialize a PCB)
    		if((cur == NULL || walker->remaining_burst_time < cur->remaining_burst_time) && walker->arrival <= clockTime)
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

// this function will schedule the tasks based on which has the lowest priority, and it does NOT exhibit preemptive behavior
// \param ready queue a dyn_array of type ProcessControlBlock_t that contains up to N elements
// \param result used for shortest job first stat tracking \ref ScheduleResult_t
// \return true if function ran successfully, false if error
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
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
    
    	//store next most prioritized and valid job in cur and update fin;
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
    
    		//update current PCB if walker has arrived and has a better priority (or if the cur is NULL, so we can initialize a PCB)
    		if((cur == NULL || walker->priority < cur->priority) && walker->arrival <= clockTime)
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

// Runs the Round Robin Process Scheduling algorithm over the incoming ready_queue
// \param ready queue a dyn_array of type ProcessControlBlock_t that contain be up to N elements
// \param result used for round robin stat tracking \ref ScheduleResult_t
// \param the quantum
// \return true if function ran successful else false for an error
bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
	//check for valid input
	if(ready_queue == NULL || result == NULL || ready_queue->data_size != sizeof(ProcessControlBlock_t) || quantum == 0){
		return false;
	}

	uint32_t clockTime = 0; //total time
	uint32_t busyTime = 0; //sum of turnaround times of all processes
	uint32_t waitTime = 0; //sum of waiting times of all processes
	size_t num_processes = dyn_array_size(ready_queue); //total number of processes in the ready queue
	if(num_processes == 0){
		return false;
	}

	//Create new array to hold modified queue
	dyn_array_t *queue = dyn_array_create(dyn_array_size(ready_queue), sizeof(ProcessControlBlock_t), NULL);
	//Check that queue was created correctly
	if(queue == NULL){
		return false;
	}
	//Iterrate through the original queue and copy elements to the new queue
	for(size_t i = 0; i < num_processes; i++){
		ProcessControlBlock_t process;
		dyn_array_extract_front(ready_queue, &process);
		dyn_array_push_back(queue, &process);
	}

	//Main algorithm loop
	while(dyn_array_empty(queue) == false){ //runs until queue is empty
		ProcessControlBlock_t process;
		dyn_array_extract_front(queue, &process);

		//check if a process has started yet. If not, start it and save it's burst time for waiting time calcs
		if(process.started == false){
			process.started = true;
			process.initial_burst_time = process.remaining_burst_time;
		}

		//check if the remaining burst time is smaller than the quantum, so we can make sure the process doesn't get scheduled for more time than it needs
		uint32_t time_slice;
		if(process.remaining_burst_time > quantum){
			time_slice = quantum;
		}
		else{
			time_slice = process.remaining_burst_time;
		}
		while(time_slice != 0){
			virtual_cpu(&process);
			clockTime++;
			time_slice--;
		}

		//If the process hasn't finished, put it back in the queue
		if(process.remaining_burst_time > 0){
			dyn_array_push_back(queue, &process);
		}
		//otherwise calculate turnaround time and waiting time, and accumulate them.
		else{
			uint32_t turnaround_time = clockTime - process.arrival;
			uint32_t waiting_time = turnaround_time - process.initial_burst_time;

			busyTime += turnaround_time;
			waitTime += waiting_time;
		}
	}
	
	//destroy the queue we created
	dyn_array_destroy(queue);

	//calculate values
	result->total_run_time = clockTime;
	result->average_turnaround_time = (float)busyTime / num_processes;
	result->average_waiting_time = (float)waitTime / num_processes;

	return true;
}


// Reads the PCB properties from the binary file into a ProcessControlBlock_t struct and its respective members.
// for N number of PCBs stored in the file.
// \param input_file the binary file contaiing the number of PCBs and contents of PCBs
// \return a populated dyn_array of ProcessControlBlocks if function ran successful else NULL for an error
dyn_array_t *load_process_control_blocks(const char *input_file)
{
	// check if file is NULL or bad file name
	if (input_file == NULL) return NULL;
	if (strncmp(input_file, "\0", 1) == 0 || strncmp(input_file, "", 1) == 0 || strncmp(input_file, "\n", 1) == 0) return NULL;

	// attempt to open input file. If error, return NULL
	FILE* file = fopen(input_file, "rb");
	if (file == NULL) return NULL;

	uint32_t n = 0;
	int count = 0;
	size_t read;

	// attempt to allocate buffer used for reading file. If error, return NULL
	unsigned char* buffer = (unsigned char*) malloc(sizeof(uint32_t));
	if (buffer == NULL) return NULL;

	// grab n = number of PCBs. If can't read from file, return NULL
	if ((read = fread(buffer, 4, 1, file)) == 1)
		memcpy(&n, buffer, sizeof(uint32_t));
	else{
		free(buffer);
		fclose(file);
		return NULL;
	}

	// create array with room for n PCBs. If error, return NULL
	dyn_array_t* arr = dyn_array_create((size_t)n, sizeof(ProcessControlBlock_t), NULL);
	if (arr == NULL){
		free(buffer);
		fclose(file);
		return NULL;
	}

	// declare pcb struct to store pcb values in file
	ProcessControlBlock_t* pcb;

	// read PCB contents
	while ((read = fread(buffer, 4, 1, file)) == 1){
		// if count is 0, next int read is remaining_burst_time. Also allocate pcb to store next 3 values
		if (count == 0){
			pcb = (ProcessControlBlock_t*) malloc (sizeof(ProcessControlBlock_t));
			memcpy(&(pcb->remaining_burst_time), buffer, 4);
		}
		// if count is 1, next int read is priority. Store in the same pcb allocated
		else if (count == 1)
			memcpy(&(pcb->priority), buffer, 4);
		// if count is 2, next int read is arrival. Store in the same pcb allocated and push onto array since this is the last value to be stored
		else if (count == 2){
			memcpy(&(pcb->arrival), buffer, 4);
			if (!dyn_array_push_front(arr, pcb)){	// if can't push onto array, free allocated memory and return NULL
				free(buffer);
				free(pcb);
				fclose(file);
				dyn_array_destroy(arr);
				return NULL;
			}
		}

		// if count is 2, next value is the remaining_burst_time for the next pcb. Restart back to 0 and allocate a new pcb. Else, increment count
		if (count == 2) count = 0;
		else count++;
	}

	// free buffer, close file, and return array
	free(buffer);
	fclose(file);
	return arr;
}

// Runs the Shortest Remaining Time First algorithm over the incoming ready_queue
// \param ready queue for a dyn_array of type ProcessControlBlock_t that contains up to N elements
// \param result used for the shortest job first stat tracking \ref ScheduleResult_t
// \return true if function ran successful, else false for an error
bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
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
    
    		//update current PCB if walker has arrived and has a shorter burst (or if the cur is NULL, so we can initialize a PCB)
    		if((cur == NULL || walker->remaining_burst_time < cur->remaining_burst_time) && walker->arrival <= clockTime)
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
			virtual_cpu(cur);
			clockTime++;
    		cur->arrival = clockTime;
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
