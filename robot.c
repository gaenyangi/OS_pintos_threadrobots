#include "projects/automated_warehouse/robot.h"

/**
 * A function setting up robot structure
 */
void setRobot(struct robot* _robot, const char* name, int row, int col, int required_payload, int current_payload, int tar_row, int tar_col){
    _robot->name = name;
    _robot->row = row;
    _robot->col = col;
    _robot->required_payload = required_payload; // required position of payload, [1 2 3 4 5 6 7]
    _robot->current_payload = current_payload; // current payload which indicates robot have passed or not the payload position. shown as req value.
    set_req(_robot, required_payload);
    _robot->tar_row = tar_row; // value of target [A B C] row
    _robot->tar_col = tar_col; // value of target [A B C] col
    
}


void move_robot_to_req(struct robot* _robot) { // move toward the req payload

    if ((_robot->row < _robot->req_row )&& (map_draw_default[_robot->row+1][_robot->col]!='X')) {
        _robot->row++;// Move downwards
    }
    else if ((_robot->col < _robot->req_col) && (map_draw_default[_robot->row][_robot->col+1] != 'X')) {
        _robot->col++; // Move rightwards
    }
    else if ((_robot->row > _robot->req_row) && (map_draw_default[_robot->row-1][_robot->col] != 'X')) {
        _robot->row--; // Move upwards
    }
    else if ((_robot->row > _robot->req_col) && (map_draw_default[_robot->row][_robot->col-1] != 'X')) {
        _robot->col--; // Move leftwards
    }
    //if robot gets to req pos
    else { _robot->current_payload = _robot->required_payload; }
}

void move_robot_to_tar(struct robot* _robot) { // after get the payload, move to desitnation
    // Determine the direction of movement based on the current position and the target position
    if ((_robot->row < _robot->tar_row) && (map_draw_default[_robot->row+1][_robot->col] != 'X')) {
        _robot->row++; // Move downwards
    }
    else if ((_robot->col < _robot->tar_col) && (map_draw_default[_robot->row][_robot->col+1] != 'X')) {
        _robot->col++; // Move rightwards
    }
    else if ((_robot->row > _robot->tar_row) && (map_draw_default[_robot->row-1][_robot->col] != 'X')) {
        _robot->row--; // Move upwards
    }
    else if ((_robot->row > _robot->tar_col) && (map_draw_default[_robot->row][_robot->col-1] != 'X')) {
        _robot->col--; // Move leftwards
    }
    //if robot gets to target destination
    //else {}
}