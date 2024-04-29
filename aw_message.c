#include "projects/automated_warehouse/aw_message.h"

// Message boxes from central control node to each robot
struct messsage_box* boxes_from_central_control_node;
// Message boxes from robots to central control node
struct messsage_box* boxes_from_robots;

void init_message_boxes(int robot_count) {
    // Allocate memory for message boxes
    boxes_from_central_control_node = malloc(sizeof(struct messsage_box) * robot_count);
    boxes_from_robots = malloc(sizeof(struct messsage_box) * robot_count);

    // Initialize message boxes
    for (int i = 0; i < robot_count; i++) {
        boxes_from_central_control_node[i].dirtyBit = 0;
        boxes_from_robots[i].dirtyBit = 0;
    }
}

void send_message_to_cnt(int robot_index, struct message msg) {
    // Write the message to the message box of CNT
    boxes_from_robots[robot_index].msg = msg;
    boxes_from_robots[robot_index].dirtyBit = 1;
    // Block the current thread
    block_thread();
}

struct message receive_message_from_robot(int robot_index) {
    // Read the message from the message box of the robot
    return boxes_from_robots[robot_index].msg;
}

void send_message_to_robot(int robot_index, struct message msg) {
    // Write the message to the message box of the robot
    boxes_from_central_control_node[robot_index].msg = msg;
    boxes_from_central_control_node[robot_index].dirtyBit = 1;
}

struct message receive_message_from_cnt(int robot_index) {
    // Read the message from the message box of CNT
    return boxes_from_central_control_node[robot_index].msg;
}