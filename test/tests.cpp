#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include "gtest/gtest.h"
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function mangling
extern "C"
{
#include <dyn_array.h>
}

#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

/*
unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment
{
	public:
		virtual void SetUp()
		{
			score = 0;
			total = 210;
		}

		virtual void TearDown()
		{
			::testing::Test::RecordProperty("points_given", score);
			::testing::Test::RecordProperty("points_total", total);
			std::cout << "SCORE: " << score << '/' << total << std::endl;
		}
};
*/

/*
* load_process_control_blocks tests. One to check proper param checking,
* one to check functionality of the function
*/
TEST(load_process_control_blocks, InvalidParam){
	//Checking we get null as the output when null param
	EXPECT_EQ(NULL, load_process_control_blocks(NULL));
	//Checking we get null as the output when bad filenames
	EXPECT_EQ(NULL, load_process_control_blocks(""));
	EXPECT_EQ(NULL, load_process_control_blocks("\n"));
	EXPECT_EQ(NULL, load_process_control_blocks("\0"));
	EXPECT_EQ(NULL, load_process_control_blocks("../fakefile.bin"));
}


TEST (load_process_control_blocks, CorrectlyRead){
	//Filename to be passed in
	const char* input_filename = "pcb.bin";
	//create array based on contents of file
	dyn_array_t* arr = load_process_control_blocks(input_filename);

	//Check each individual pcb
	ProcessControlBlock_t *pcb = (ProcessControlBlock_t *)dyn_array_at(arr, 0);
	EXPECT_EQ(20.0, pcb->remaining_burst_time);
	EXPECT_EQ(0.0, pcb->priority);
	EXPECT_EQ(3.0, pcb->arrival);

	pcb = (ProcessControlBlock_t *)dyn_array_at(arr, 1);
	EXPECT_EQ(5.0, pcb->remaining_burst_time);
	EXPECT_EQ(0.0, pcb->priority);
	EXPECT_EQ(2.0, pcb->arrival);

	pcb = (ProcessControlBlock_t *)dyn_array_at(arr, 2);
	EXPECT_EQ(10.0, pcb->remaining_burst_time);
	EXPECT_EQ(0.0, pcb->priority);
	EXPECT_EQ(1.0, pcb->arrival);

	pcb = (ProcessControlBlock_t *)dyn_array_at(arr,3);
	EXPECT_EQ(15.0, pcb->remaining_burst_time);
	EXPECT_EQ(0.0, pcb->priority);
	EXPECT_EQ(0.0, pcb->arrival);

	//destroy the array to free memory
	dyn_array_destroy(arr);
}

/*
* First-come, First-serve tests. One to check proper param checking,
* one to check functionality of the program
*/

TEST (first_come_first_serve, InvalidParam){
	//Checking we return false when we pass in invalid params
	EXPECT_EQ(false, first_come_first_serve(NULL, NULL));
}

TEST (first_come_first_serve, CorrectProcess){
	//creating an array
	dyn_array_t *queue = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);

	//creating each pcb for the array
	ProcessControlBlock_t *p1 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p1->priority = 0;
	p1->arrival = 0;
	p1->remaining_burst_time = 10;
	p1->started = false;
	dyn_array_push_back(queue, p1); //pushing it to the back of the queue

	ProcessControlBlock_t *p2 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p2->priority = 1;
	p2->arrival = 1;
	p2->remaining_burst_time = 5;
	p2->started = false;
	dyn_array_push_back(queue, p2);
	
	ProcessControlBlock_t *p3 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p3->priority = 2;
	p3->arrival = 2;
	p3->remaining_burst_time = 8;
	p3->started = false;
	dyn_array_push_back(queue, p3);

	//create a result
	ScheduleResult_t *output = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));

	//run scheduling algorithm
	bool result = first_come_first_serve(queue, output);

	float expected_turnaround_time = 15.0;
	float expected_waiting_time = 22.0/3.0;

	EXPECT_EQ(true, result);
	EXPECT_EQ(expected_turnaround_time, output->average_turnaround_time);
	EXPECT_EQ(expected_waiting_time, output->average_waiting_time);

	//free all variables
	dyn_array_destroy(queue);
	free(output);
}


TEST(shortest_job_first, InvalidParam){
	//checking that the output is false when we pass in null parameters
	EXPECT_EQ(false, shortest_job_first(NULL,NULL));
}

TEST(shortest_job_first, CorrectProcess){
	//creating an array
	dyn_array_t *queue = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);

	//creating PCB variables to test with
	ProcessControlBlock_t *p1 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p1->priority = 0;
	p1->arrival = 2;
	p1->remaining_burst_time = 6;
	p1->started = false;
	dyn_array_push_back(queue, p1); //pushing variable onto the queue

	ProcessControlBlock_t *p2 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p2->priority = 0;
	p2->arrival = 0;
	p2->remaining_burst_time = 3;
	p2->started = false;
	dyn_array_push_back(queue, p2);
	
	ProcessControlBlock_t *p3 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p3->priority = 0;
	p3->arrival = 1;
	p3->remaining_burst_time = 8;
	p3->started = false;
	dyn_array_push_back(queue, p3);

	//create result output
	ScheduleResult_t *output = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));

	//running scheduling algorithm
	bool result = shortest_remaining_time_first(queue, output);

	float expected_waiting_time = (0.0 + 1.0 + 8.0) / 3.0;
	float expected_turnaround_time = (3.0 + 16.0 + 7.0) / 3.0;

	//checking the waiting time vs expected
	EXPECT_EQ(true, result);
	EXPECT_FLOAT_EQ(expected_waiting_time, output->average_waiting_time);
	EXPECT_FLOAT_EQ(expected_turnaround_time, output->average_turnaround_time);

	dyn_array_destroy(queue);
	free(output);
}

TEST(priority, InvalidParam){
	//checking that we output false for incorrect params
	EXPECT_EQ(false, priority(NULL,NULL));
}

TEST(priority, CorrectProcess){
	//creating an array
	dyn_array_t *queue = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);

	//creating PCB variables to test with
	ProcessControlBlock_t *p1 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p1->priority = 2;
	p1->arrival = 0;
	p1->remaining_burst_time = 10;
	p1->started = false;
	dyn_array_push_back(queue, p1); //pushing variable onto the queue

	ProcessControlBlock_t *p2 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p2->priority = 0;
	p2->arrival = 0;
	p2->remaining_burst_time = 5;
	p2->started = false;
	dyn_array_push_back(queue, p2);
	
	ProcessControlBlock_t *p3 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p3->priority = 1;
	p3->arrival = 0;
	p3->remaining_burst_time = 8;
	p3->started = false;
	dyn_array_push_back(queue, p3);

	//create result output
	ScheduleResult_t *output = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));

	//running scheduling algorithm
	bool result = shortest_remaining_time_first(queue, output);

	float expected_turnaround_time = (5.0 + 13.0 + 23.0)/3.0;
	float expected_waiting_time = (0.0 + 13.0 + 5.0)/3.0;

	EXPECT_EQ(true, result);
	EXPECT_FLOAT_EQ(expected_turnaround_time, output->average_turnaround_time);
	EXPECT_FLOAT_EQ(expected_waiting_time, output->average_waiting_time);

	dyn_array_destroy(queue);
	free(output);
}



TEST(round_robin, InvalidParam){
	//checking that we output false for incorrect params
	EXPECT_EQ(false, round_robin(NULL,NULL,0));
}

TEST(round_robin, CorrectProcess){
	//creating an array
	dyn_array_t *queue = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);

	//creating PCB variables to test with
	ProcessControlBlock_t *p1 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p1->priority = 0;
	p1->arrival = 0;
	p1->remaining_burst_time = 10;
	p1->started = false;
	dyn_array_push_back(queue, p1); //pushing variable onto the queue

	ProcessControlBlock_t *p2 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p2->priority = 0;
	p2->arrival = 0;
	p2->remaining_burst_time = 5;
	p2->started = false;
	dyn_array_push_back(queue, p2);
	
	ProcessControlBlock_t *p3 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p3->priority = 0;
	p3->arrival = 0;
	p3->remaining_burst_time = 8;
	p3->started = false;
	dyn_array_push_back(queue, p3);

	//create result output
	ScheduleResult_t *output = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));

	//running scheduling algorithm
	bool result = round_robin(queue, output, 4);

	float expected_turnaround_time = (23.0 + 17.0 + 21.0)/3.0;
	float expected_waiting_time = (13.0 + 12.0 + 13.0)/3.0;

	EXPECT_EQ(true, result);
	EXPECT_EQ(expected_turnaround_time, output->average_turnaround_time);
	EXPECT_EQ(expected_waiting_time, output->average_waiting_time);

	dyn_array_destroy(queue);
	free(output);
}

TEST(shortest_remaining_time_first, InvalidParam){
	//checking that we output false for incorrect params
	EXPECT_EQ(false, shortest_remaining_time_first(NULL,NULL));
}

TEST(shortest_remaining_time_first, CorrectProcess){
	//creating an array
	dyn_array_t *queue = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);

	//creating PCB variables to test with
	ProcessControlBlock_t *p1 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p1->priority = 0;
	p1->arrival = 0;
	p1->remaining_burst_time = 6;
	p1->started = false;
	dyn_array_push_back(queue, p1); //pushing variable onto the queue

	ProcessControlBlock_t *p2 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p2->priority = 0;
	p2->arrival = 0;
	p2->remaining_burst_time = 8;
	p2->started = false;
	dyn_array_push_back(queue, p2);
	
	ProcessControlBlock_t *p3 = (ProcessControlBlock_t *)malloc(sizeof(ProcessControlBlock_t));
	p3->priority = 0;
	p3->arrival = 0;
	p3->remaining_burst_time = 5;
	p3->started = false;
	dyn_array_push_back(queue, p3);

	ScheduleResult_t *output = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));

	bool result = shortest_remaining_time_first(queue, output);

	float expected_turnaround_time = (11.0 + 19.0 + 5.0)/3.0;
	float expected_waiting_time = (5.0 + 11.0)/3.0;

	EXPECT_EQ(true,result);
	EXPECT_EQ(expected_turnaround_time, output->average_turnaround_time);
	EXPECT_EQ(expected_waiting_time, output->average_waiting_time);

	dyn_array_destroy(queue);
	free(output);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	// ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
	return RUN_ALL_TESTS();
}
