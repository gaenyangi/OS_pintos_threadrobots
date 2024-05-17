#ifndef _PROJECTS_PROJECT1_ROBOT_H__
#define _PROJECTS_PROJECT1_ROBOT_H__

/**
 * A Structure representing robot
 */
struct robot {
    const char* name;
    int row;
    int col;
    int required_payload;
    int current_payload; 
    int req_row;
    int req_col;
    char target_position;	
    int tar_row;
    int tar_col;
};

void set_req(struct robot* _robot, int req);

void setRobot(struct robot* _robot, const char* name, int row, int col, int required_payload, int current_payload, int tar_row, int tar_col, char target_position);
void move_robot_to_tar(struct robot* _robot);
void move_robot_to_req(struct robot* _robot);
#endif
