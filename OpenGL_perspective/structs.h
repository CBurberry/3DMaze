#pragma once
#include <stdlib.h>
#include <vector>
#include <ctime>
#include "mymath.h"


//(walls) For each row we want to have 15 potential horizontal positions 
//			and 16 potential vertical positions. So, per row we define:
#define MAXH_PER_ROW 15
#define MAXV_PER_ROW 16

//Then we have a floor grid of 15x15 squares, therefore 15 rows of verticals and 16 rows of horizontals.
#define MAXH_ROWS 16
#define MAXV_ROWS 15

//How far off the walls are from the floor - measured from y=0
#define FLOOR_OFFSET 1.01

//Player camera start and end coords.
#define PLAYER_START_POSX 0.0
#define PLAYER_START_POSZ 14.5
#define PLAYER_END_POSX 0.0
#define PLAYER_END_POSZ -15.5

//Simple data structure for 3 float values.
struct vec3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	vec3() {}
	vec3(float a, float b, float c)
	{
		x = a; y = b; z = c;
	}
};

//Simple data structure for 4 float values.
struct vec4
{
	float a = 0.0f;
	float b = 0.0f;
	float c = 0.0f;
	float d = 0.0f;
	vec4() {}
	vec4(float a, float b, float c, float d)
	{
		this->a = a; this->b = b; this->c = c; this->d = d;
	}
};

//Struct to represent plane translations for constructing from vertex list.
struct plane
{
	int id = 0;
	vec3 displacement = vec3();
	//Rotation in terms of (degrees in x, degrees in y, degrees in z)
	vec3 rotation = vec3();
	plane() {}
	plane(int i, vec3 d, vec3 r)
	{
		id = i;
		displacement = d;
		rotation = r;
	}
};

//Define multiple 2D arrays for Hwalls and Vwalls.
struct gridCombo
{
	int gridH[MAXH_ROWS][MAXH_PER_ROW];
	int gridV[MAXV_ROWS][MAXV_PER_ROW];
	std::vector<std::array<int, MAXH_ROWS>> Hcol_vec;
	std::vector<std::array<int, MAXV_PER_ROW>> Vrow_vec;

	//Initialisation methods for hard-coded transformations of instanced walls.
	void intialiseGH();		//Vertical
	void intialiseGV();		//Horizontal
};

//Pointer triangle that is visible from Ortho camera. Follows player/camera transformations.
struct pointer 
{
	int id = 1;
	vec3 color = vec3(0.0, 0.0, 1.0);
	vec3 vert1 = vec3(0.0, 5.0, -0.9);
	vec3 vert2 = vec3(-0.5, 5.0, 0.7);
	vec3 vert3 = vec3(0.5, 5.0, 0.7);
	std::array<double, 4> localRotation = { 0.0, 0.0, 0.0, 1.0};
	std::array<double, 3> absTranslation = { 0.0, 0.0, 0.0 };
};

//Perspective camera object that is the main component, translated/rotated on input.
struct pCamera 
{
	pointer icon;

	//'g' denotes global positions, 'l' denotes global positions.
	//Local values are required as camera is slightly off center in each facing direction (used for rotations).
	std::array<double, 4> g_eye_pos = { PLAYER_START_POSX, 1.5, PLAYER_START_POSZ, 1.0 };
	std::array<double, 4> l_eye_pos = { 0, 1.5, 0.5, 1.0 };
	std::array<double, 4> g_center_pos = { 0, 1.5, 9, 1.0 };
	std::array<double, 4> l_center_pos = { 0, 1.5, -5.0, 1.0 };
	std::array<double, 3> up_vector = { 0, 1, 0 };
	std::array<double, 3> local_origin = { 0, 0, 0 };
	std::array<double, 3> origin_displacement = { 0, 0, 0 };	//This vector represents the local origin position vector - grid position.

	int direction = 0;
	gridCombo* grids = NULL;
	double turn_angle = 0.0;
	bool turning = false;
	bool win = false;

	pCamera(gridCombo* grids) 
	{
		this->grids = grids;
		icon.absTranslation[0] = PLAYER_START_POSX;
		icon.absTranslation[2] = PLAYER_START_POSZ - 0.5;
		origin_displacement[0] = PLAYER_START_POSX;
		origin_displacement[2] = PLAYER_START_POSZ - 0.5;
	}

	//Move forward - increment the global origins & global centers and apply pos'. Bool value represents success.
	bool forward();
	//Move backward - decrement the global origins & global centers and apply pos'. Bool value represents success.
	bool backward();

	/*
		Rotate a given angle - pivot about local origin, calculate the rotated local eye_pos&center_pos
		and apply translation based on origin displacement vector to get new global eye_pos&center_pos.
	*/
	void rotate(double angle);

	//Traunicate floating point zeroes so they cannot be negative.
	double traunicate(double val) 
	{
		if (val > 0.0000000001 || val < -0.0000000001)
		{
			return val;
		}
		return abs(val * 0);
	}
};

//Struct describing the Orthogonal camera object.
struct oCamera
{
	std::array<double, 4> g_eye_pos = { 0, 6.0, 0, 1.0 };
	std::array<double, 4> l_eye_pos = { 0, 0, 0, 1.0 };
	std::array<double, 4> g_center_pos = { 0, -1.0, 0, 1.0 };
	std::array<double, 4> l_center_pos = { 0, 0, 0, 1.0 };
	std::array<double, 3> up_vector = { 0, 0, -1 };
};