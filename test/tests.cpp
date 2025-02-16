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
* one to check functionality of the program
*/
TEST(load_process_control_blocks, InvalidParam){
	EXPECT_EQ(NULL, load_process_control_blocks(NULL));
}

TEST (load_process_control_blocks, CorrectlyRead){
	const char* input_filename = "../pcb.bin";
	dyn_array_t* arr = load_process_control_blocks(input_filename);
	ASSERT_NE((dyn_array_t*)NULL, arr);
}

/*
* First-come, First-serve tests. One to check proper param checking,
* one to check functionality of the program
*/

TEST (first_come_first_serve, InvalidParam){
	EXPECT_EQ(false, first_come_first_serve(NULL, NULL));
}

TEST (first_come_first_serve, CorrectProcess){
	dyn_array_t *queue = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);

	assert(queue != NULL);

	ProcessControlBlock_t *p1;
	p1->priority = 0;
	p1->arrival = 0;
	p1->remaining_burst_time = 10;
	p1->started = false;
	dyn_array_push_back(queue, &p1);

	ProcessControlBlock_t *p2;
	p2->priority = 1;
	p2->arrival = 1;
	p2->remaining_burst_time = 5;
	p2->started = false;
	dyn_array_push_back(queue, &p2);
	
	ProcessControlBlock_t *p3;
	p3->priority = 2;
	p3->arrival = 2;
	p3->remaining_burst_time = 8;
	p3->started = false;
	dyn_array_push_back(queue, &p3);

	ScheduleResult_t *output = (ScheduleResult_t *)malloc(sizeof(ScheduleResult_t));

	bool result = first_come_first_serve(queue, output);
	assert(result == true);
	assert(output->total_run_time == 23);
	assert(output->average_turnaround_time == 16);
	assert(output->average_waiting_time == ((10.0 + 5.0 + 8.0)/3.0));
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	// ::testing::AddGlobalTestEnvironment(new GradeEnvironment);
	return RUN_ALL_TESTS();
}
