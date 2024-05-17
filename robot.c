#include "projects/automated_warehouse/robot.h"


const char map[6][7] = {
    {'X', 'X', 'A', 'X', 'X', 'X', 'X' },
    {'X', '1', ' ', '2', '3', '4', 'X' },
    {'B', ' ', ' ', ' ', ' ', ' ', 'X' },
    {'X', ' ', ' ', ' ', ' ', ' ', 'X' },
    {'X', '5', ' ', '6', '7', 'S', 'X' },
    {'X', 'X', 'C', 'X', 'X', 'W', 'X' }
};

void set_req(struct robot* _robot, int req) {
    switch (req) {
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
}

/**
 * A function setting up robot structure
 */
void setRobot(struct robot* _robot, const char* name, int row, int col, int required_payload, int current_payload, int tar_row, int tar_col, char target_position){
	_robot->name = malloc(strlen(name) + 1);
    strlcpy(_robot->name, name, strlen(name) + 1);
    _robot->row = row;
    _robot->col = col;
    _robot->required_payload = required_payload; // required position of payload, [1 2 3 4 5 6 7]
    _robot->current_payload = current_payload; // current payload which indicates robot have passed or not the payload position. shown as req value.
    set_req(_robot, required_payload); // set the req_pos which need to visit
    _robot->tar_row = tar_row; // value of target [A B C] row
    _robot->tar_col = tar_col; // value of target [A B C] col
    _robot->target_position=target_position;
}


void move_robot_to_req(struct robot* _robot) { // move toward the req payload
	char req_load[2];
	snprintf(req_load, sizeof(req_load), "%d", _robot->required_payload);
    if ((_robot->col > _robot->req_col) && (map[_robot->row][_robot->col - 1] == ' ' || 
map[_robot->row][_robot->col - 1] == 'S' || map[_robot->row][_robot->col - 1] == req_load[0] ||
map[_robot->row][_robot->col - 1] == _robot->target_position )) {
        _robot->col--; // Move leftwards
        return;
    }
    if ((_robot->row < _robot->req_row) && (map[_robot->row+1][_robot->col] == ' ' || 
map[_robot->row+1][_robot->col] == 'S' || map[_robot->row+1][_robot->col] == req_load[0] ||
map[_robot->row+1][_robot->col] == _robot->target_position)) {
        _robot->row++; // Move downwards
        return;
    }
    if ((_robot->col < _robot->req_col) && (map[_robot->row][_robot->col + 1] == ' ' || 
map[_robot->row][_robot->col + 1] == 'S' || map[_robot->row][_robot->col + 1] == req_load[0] ||
map[_robot->row][_robot->col + 1] == _robot->target_position)) {
        _robot->col++; // Move rightwards
        return;
    }
    if ((_robot->row > _robot->req_row) && (map[_robot->row-1][_robot->col] == ' ' || 
map[_robot->row-1][_robot->col] == 'S' || map[_robot->row-1][_robot->col] == req_load[0] ||
map[_robot->row-1][_robot->col] == _robot->target_position)) {
        _robot->row--; // Move upwards
        return;
    }

    //if robot gets to req pos
    if((_robot->row == _robot->req_row) && (_robot->col == _robot->req_col)){ 
	    _robot->current_payload = _robot->required_payload;
	    return;
    }

    //if can't move, move 1 step up or down
    if(map[_robot->row+1][_robot->col] == ' '){
	    _robot->row++;
	    return;
    }
    if(map[_robot->row-1][_robot->col] == ' '){
	    _robot->row--;
	    return;
    }
}

void move_robot_to_tar(struct robot* _robot) { // after get the payload, move to desitnation
	char req_load[2];
	snprintf(req_load, sizeof(req_load), "%d", _robot->required_payload);

    // Determine the direction of movement based on the current position and the target position
    if ((_robot->col > _robot->tar_col) && (map[_robot->row][_robot->col - 1] == ' ' || 
map[_robot->row][_robot->col - 1] == 'S' || map[_robot->row][_robot->col - 1] == req_load[0] ||
map[_robot->row][_robot->col - 1] == _robot->target_position )) {
        _robot->col--; // Move leftwards
        return;
    }
    if ((_robot->row < _robot->tar_row) && (map[_robot->row+1][_robot->col] == ' ' || 
map[_robot->row+1][_robot->col] == 'S' || map[_robot->row+1][_robot->col] == req_load[0] ||
map[_robot->row+1][_robot->col] == _robot->target_position)) {
        _robot->row++; // Move downwards
        return;
    }
    if ((_robot->col < _robot->tar_col) && (map[_robot->row][_robot->col + 1] == ' ' || 
map[_robot->row][_robot->col + 1] == 'S' || map[_robot->row][_robot->col + 1] == req_load[0] ||
map[_robot->row][_robot->col + 1] == _robot->target_position)) {
        _robot->col++; // Move rightwards
        return;
    }
    if ((_robot->row > _robot->tar_row) && (map[_robot->row-1][_robot->col] == ' ' || 
map[_robot->row-1][_robot->col] == 'S' || map[_robot->row-1][_robot->col] == req_load[0] ||
map[_robot->row-1][_robot->col] == _robot->target_position)) {
        _robot->row--; // Move upwards
        return;
    }

    //if can't move, move 1 step up or down
    if(map[_robot->row+1][_robot->col] == ' '){
	    _robot->row++;
	    return;
    }
    if(map[_robot->row-1][_robot->col] == ' '){
	    _robot->row--;
	    return;
    }
}
