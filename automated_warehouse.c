#include <stdio.h>
#include <string.h>

#include "threads/init.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"

#include "devices/timer.h"

#include "projects/automated_warehouse/aw_manager.h"

int robot_count = 0; // total number of robots
int robot_current = 1; // current moving robot. just used for cnt thread, not to intercross robots.

struct robot* robots;

void cnt() {
    while (robot_current<robot_count+1) {

        print_map(robots, robot_count);
        for (int i = 1; i < robot_count+1; i++) {
            recieve_message_from_robot(i);
            if (i == robot_current) {
                if (recieve_message_from_robot(i).current_payload == recieve_message_from_robot(i).required_payload
                    && recieve_message_from_robot(i).row == recieve_message_from_robot(i).tar_row
                    && recieve_message_from_robot(i).col == recieve_message_from_robot(i).tar_col) {
                    robot_current++;// if robot gets to destination with req, move the next robot.
                    break;
                }
                if (recieve_message_from_robot(i).current_payload == recieve_message_from_robot(i).required_payload) { //check payload status
                    recieve_message_from_robot(i).cmd = 1; //if payload success, move to target destination
                    send_message_to_robot(i, recieve_message_from_robot(i));
                }
                else {
                    recieve_message_from_robot(i).cmd = 2; // haven't get payload, move to req
                    send_message_to_robot(i, recieve_message_from_robot(i));
                }
            }
            else { //set other robots as waiting
                recieve_message_from_robot(i).cmd = 0;
                send_message_to_robot(i, recieve_message_from_robot(i));
            }
        }
        unblock_thread();
        increase_step();
    }
}

void robot_thread(void* aux) { // robot_thread implement
    int idx = *((int*)aux);   
    printf("thread %d is at\n");
    while (1) {
        int cmd=recieve_message_from_cnt(idx);
        switch(cmd){
        case 0:
            //wait
            break;
        case 1:
            move_robot_to_tar(robots[idx]);
            break;
        case 2:
            move_robot_to_req(robots[idx]);
            break;
        }

        boxes_from_robots[idx]->msg.row = robots[idx].row;
        boxes_from_robots[idx]->msg.col = robots[idx].col;
        boxes_from_robots[idx]->msg.current_payload = robots[idx].current_payload;
        boxes_from_robots[idx]->msg.required_payload = robots[idx].required_payload;
        boxes_from_robots[idx]->msg.tar_row = robots[idx].tar_row;
        boxes_from_robots[idx]->msg.tar_col = robots[idx].tar_col;

        send_message_to_cnt(robot[idx], boxes_from_robots[idx]->msg);
        block_thread();
    }
}


// entry point of simulator
void run_automated_warehouse(char **argv)
{
        init_automated_warehouse(argv); // do not remove this

        printf("implement automated warehouse!\n");       

        robot_count = atoi(argv[1]);
        robots = malloc(sizeof(struct robot) * robot_count);
        for (int i = 1; i <robot_count+1; i++) {
            int target_load = atoi(argv[2][3 * i]);
            char relase_pos = argv[2][3 * i + 1]; 
            int target_row = 0;int target_col = 0;

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

            if (relase_pos == 'A') { target_row = 0; target_col = 2; }
            else if (relase_pos == 'B') { target_row = 2; target_col = 0; }
            else { target_row = 5; target_col = 2; }

            char robot_name[5];
            sprintf(robot_name, "R&d", i);
            setRobot(&robots[i], robot_name, 5, 5, target_load, 0,target_row, target_col);
        }

        tid_t* threads = malloc(sizeof(tid_t) * (robot_count + 1));
        int* idxs = malloc(sizeof(int) * (robot_count + 1));
        threads[0] = thread_create("CNT", 0, &cnt, NULL);
        for (int i = 1; i < robot_count + 1; i++) {
            char thread_name[5];
            sprintf(thread_name, "R&d", i);
            threads[i] = thread_create(thread_name, 0, &robot_thread, &idxs[i]);
        }


        // if you want, you can use main thread as a central control node
        
}