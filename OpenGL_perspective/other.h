#pragma once
#include "structs.h"
#include "glut.h"

#define TURN_DURATION 0.1

extern pCamera pCam;
extern oCamera oCam;

extern int view_mode;			 //Mode: 0 - Perspective, 1 = Ortho2d
extern bool show_solution;

// Handle for wall display list
extern unsigned int g_wall;

extern std::vector<plane> plane_vec;
extern gridCombo grids;
extern std::vector<vec3> solution_vec;

//Compare the current position of the player (using perspective camera) to the top - 1 element in the stack and resolve.
void resolve_stack(pCamera&);

//Initialiser of vector (stack) for solution
void setup_solution_stack();

//Add a pre-defined set of planes to the plane_vec vector to be drawn. - currently defining maze level here.
void preset_planes(gridCombo&);

//Update player pointer transformations
void update_pointer();

//Create display list entry handle for plane
unsigned int make_wall();

//Draw all walls in the global vector
void draw_all_walls();

//Draw solution line using solution_vec vector.
void draw_solution();

//Idle function, used for animation
void animatedTurn();

