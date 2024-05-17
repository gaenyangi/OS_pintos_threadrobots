#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"

#include "projects/automated_warehouse/aw_message.h"

#include "projects/automated_warehouse/aw_manager.h"

int robot_count = 0; // total number of robots
int robot_current = 1; // current moving robot. just used for cnt thread, not to intercross robots.

struct robot* robots;

void cnt() {
    while (robot_current<robot_count+1) {

        print_map(robots, robot_count);
	    //printf("robot_current is now %d\n\n", robot_current);

        for (int i = 0; i < robot_count; i++) {

            struct message* robot_msg = receive_message_from_robot(i);
            if (i+1 == robot_current) {
                if (robot_msg->current_payload == robot_msg->required_payload
                    && robot_msg->row == robot_msg->tar_row
                    && robot_msg->col == robot_msg->tar_col) {
		    robot_msg->cmd = 0;
                    send_message_to_robot(i+1, *robot_msg);
                    robot_current++;// if robot gets to destination with req, move the next robot.
                    break;
                }
                else if (robot_msg->current_payload == robot_msg->required_payload) { //check payload status
                    robot_msg->cmd = 1; //if payload success, move to target destination
                    send_message_to_robot(i+1, *robot_msg);
                }
                else {
                    robot_msg->cmd = 2; // haven't get payload, move to req
                    send_message_to_robot(i+1, *robot_msg);
                }
            }
            else { //set other robots as waiting
                robot_msg->cmd = 0;
                send_message_to_robot(i+1, *robot_msg);
            }
        }
        unblock_threads();
        increase_step();
	    thread_yield(); // yield so the unblocked robot_threads can run
    }
    block_thread(); // after all the robots moved, block the control thread to stop the program.
}

void robot_thread(void* aux) { // robot_thread implement
    int idx = *((int*)aux);   
    while (1) {
        int cmd=receive_message_from_cnt(idx).cmd;
	    //printf("idx is %d, robot %s got cmd : %d \n", idx,robots[idx-1].name, cmd);
        switch(cmd){
        case 0:
            //wait
            break;
        case 1:
            move_robot_to_tar((robots+idx-1));
            break;
        case 2:
            move_robot_to_req((robots+idx-1));
            break;
        }

        boxes_from_robots[idx-1].msg.row = robots[idx-1].row;
        boxes_from_robots[idx-1].msg.col = robots[idx-1].col;
        boxes_from_robots[idx-1].msg.current_payload = robots[idx-1].current_payload;
        boxes_from_robots[idx-1].msg.required_payload = robots[idx-1].required_payload;
        boxes_from_robots[idx-1].msg.tar_row = robots[idx-1].tar_row;
        boxes_from_robots[idx-1].msg.tar_col = robots[idx-1].tar_col;

        send_message_to_cnt(idx, boxes_from_robots[idx].msg);
        block_thread();
    }
}


// entry point of simulator
void run_automated_warehouse(char **argv)
{
        init_automated_warehouse(argv); // do not remove this

        printf("implement automated warehouse!\n");        
        robot_count = atoi(argv[1]);    
        robots = malloc(sizeof(struct robot) * (robot_count));

	    char* position = argv[2];

	    init_message_boxes(robot_count);
	    init_blocked_threads();

        for (int i = 0; i <robot_count; i++) {
            int target_load = atoi(&position[3 * i]);
            char relase_pos = position[3 * i + 1]; 
            int target_row = 0;int target_col = 0;char target_position='K';

            /*
            #define ROW_A 0
            #define COL_A 2
            #define ROW_B 2
            #define COL_B 0
            #define ROW_C 5
            #define COL_C 2
            #define ROW_S 4
            #define COL_S 5
            #define ROW_W 5
            #define COL_W 5
            */

            if (relase_pos == 'A') { target_row = 0; target_col = 2; target_position='A';}
            else if (relase_pos == 'B') { target_row = 2; target_col = 0; target_position='B';}
            else { target_row = 5; target_col = 2; target_position='C';}

            char robot_name[5];
	        snprintf(robot_name, sizeof(robot_name), "R%d", (i+1));

            setRobot(&robots[i], robot_name, 5, 5, target_load, 0,target_row, target_col, target_position);
        }

        tid_t* threads = malloc(sizeof(tid_t) * (robot_count + 1));
        int* idxs = malloc(sizeof(int) * (robot_count + 1));
        threads[0] = thread_create("CNT", 0, &cnt, NULL);
        for (int i = 1; i < robot_count + 1; i++) {
            char thread_name[5];
	        snprintf(thread_name, sizeof(thread_name), "R%d", i);
            idxs[i] = i;
            threads[i] = thread_create(thread_name, 0, &robot_thread, &idxs[i]);
        }

        // if you want, you can use main thread as a central control node   
}
