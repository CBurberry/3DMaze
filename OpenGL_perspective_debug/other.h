#pragma once
#include "glut.h"
#include "structs.h"

extern pCamera pCam;
extern oCamera oCam;
  
extern float g_trans_x;
extern float g_trans_y;
extern float g_trans_z;
extern int g_axis_selection;
extern float g_angle;

extern bool update_flag; //set if current plane should update based on trans.
extern int view_mode; //Mode: 0 - Perspective, 1 = Ortho2d
extern bool show_solution;

				   // handle for display list
extern unsigned int g_plane;
extern std::vector<plane> plane_vec;
extern gridCombo grids;

//Add a pre-defined set of planes to the plane_vec vector to be drawn. - currently defining maze level here.
void preset_planes(gridCombo&);

//Draw the player pointer
void draw_pointer();

//Update player pointer transformations
void update_pointer();

//Draw the debug axis centered on 0.0
void draw_axis();

//Create display list entry handle for plane
unsigned int make_plane();

//View controls on console
void control_help();

//For runtime plane control - OBSOLETE DEBUG METHOD
void update_current_plane();

//Draw all planes in the global vector
void draw_all_planes();

//For runtime plane displacement - OBSOLETE DEBUG METHOD
void plane_displaced(vec3 axis_displacement, vec4 rotation);

//Keyboard controls
void keyboard(unsigned char key, int, int);

//Keyboard controls extension
void special(int key, int, int);

//Draw line from start to goal.
void draw_solution();

