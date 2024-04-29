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
    int req_row;
    int req_col;
    int current_payload; 
    int tar_row;
    int tar_col;
};
void set_req(struct robot* _robot, int req) {
    switch (req) 
        case 1:
            _robot->req_row = 1;
            _robot->req_col = 1;
            break;
        case 2:
            _robot->req_row = 1;
            _robot->req_col = 3;
            break;
        case 3:
            _robot->req_row = 1;
            _robot->req_col = 4;
            break;
        case 4:
            _robot->req_row = 1;
            _robot->req_col = 5;
            break;
        case 5:
            _robot->req_row = 4;
            _robot->req_col = 1;
            break;
        case 6:
            _robot->req_row = 4;
            _robot->req_col = 3;
            break;
        case 7:
            _robot->req_row = 4;
            _robot->req_col = 4;
            break;

}
void setRobot(struct robot* _robot, const char* name, int row, int col, int required_payload, int current_payload, int tar_row, int tar_col);
void move_robot_to_tar(struct robot* _robot);
void move_robot_to_req(struct robot* _robot);
#endif