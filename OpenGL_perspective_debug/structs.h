#pragma once
#include <stdlib.h>
#include <vector>
#include "mymath.h"


//(walls) For each row we want to have 15 potential horizontal positions 
//			and 16 potential vertical positions. So, per row we define:
#define MAXH_PER_ROW 15
#define MAXV_PER_ROW 16
//Then we have a floor grid of 15x15 squares, therefore 15 rows of verticals and 16 rows of horizontals.
#define MAXH_ROWS 16
#define MAXV_ROWS 15

//Floor tile dimensions
#define NUM_OF_SQH 14
#define NUM_OF_SQV 14

//How far off the walls are from the floor - measured from y=0
#define FLOOR_OFFSET 1.01

//Player camera start and end coords.
#define PLAYER_START_POSX 0.0
#define PLAYER_START_POSZ 14.5
#define PLAYER_END_POSX 0.0
#define PLAYER_END_POSZ -15.5



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

//Define multiple 2D arrays for ceiling, floor, Hwalls and Vwalls. - not properly in use yet.
struct gridCombo
{
	int gridH[MAXH_ROWS][MAXH_PER_ROW];
	int gridV[MAXV_ROWS][MAXV_PER_ROW];
	int gridF[NUM_OF_SQV][NUM_OF_SQH];
	int gridT[NUM_OF_SQV][NUM_OF_SQH];
	std::vector<std::array<int, MAXH_ROWS>> Hcol_vec;
	std::vector<std::array<int, MAXV_PER_ROW>> Vrow_vec;
	bool set_floor_colour = false;
	vec3 colourF = vec3(1.0, 1.0, 1.0);	//White
	bool set_wall_colour = false;
	vec3 colourW = vec3(1.0, 1.0, 1.0);	//White
	bool set_top_colour = false;
	vec3 colourT = vec3(1.0, 1.0, 1.0);	//White

	//DEFINE 2D ARRAYS FOR VERTICAL & HORIZONTAL WALL PLACEMENT RESPECTIVELY
	//We define the array in terms of a cartesian placement, origin BL corner.

	//use Hcol_vec to set gridH values. - hard coded.
	void intialiseGH();
	//use Vrow_vec to set gridV values. - hard coded.
	void intialiseGV();

	//print horizontals
	void printGH();
	//print verticals
	void printGV();
	//Need methods for floor and ceiling.


};

struct pointer 
{
	//Given that in the 2d plane we see from orthogonal, Y is -Z and X is X.
	int id = 1;
	vec3 color = vec3(0.0, 0.0, 1.0);
	vec3 vert1 = vec3(0.0, 5.0, -0.9);
	vec3 vert2 = vec3(-0.5, 5.0, 0.7);
	vec3 vert3 = vec3(0.5, 5.0, 0.7);
	std::array<double, 4> localRotation = { 0.0, 0.0, 0.0, 1.0};
	std::array<double, 3> absTranslation = { 0.0, 0.0, 0.0 };
};

enum class Face
{
	North = 0,
	Northeast,
	East,
	Southeast,
	South,
	Southwest,
	West,
	Northwest
};



struct pCamera 
{
	pointer icon;
	std::array<double, 4> g_eye_pos = { PLAYER_START_POSX, 1.5, PLAYER_START_POSZ, 1.0 };
	std::array<double, 4> l_eye_pos = { 0, 1.5, 0.5, 1.0 };
	std::array<double, 4> g_center_pos = { 0, 1.5, 9, 1.0 };
	std::array<double, 4> l_center_pos = { 0, 1.5, -5.0, 1.0 };
	std::array<double, 3> up_vector = { 0, 1, 0 };
	std::array<double, 3> local_origin = { 0, 0, 0 };
	std::array<double, 3> origin_displacement = { 0, 0, 0 };	//This vector represents the local origin position vector - grid position.

	std::array<Face, 8> face_iter;
	int direction = 0;
	bool wall_ahead = false;
	bool wall_behind = false;
	gridCombo* grids = NULL;

	pCamera(gridCombo* grids) 
	{
		prepIterator();
		this->grids = grids;
		icon.absTranslation[0] = PLAYER_START_POSX;
		icon.absTranslation[2] = PLAYER_START_POSZ - 0.5;
		origin_displacement[0] = PLAYER_START_POSX;
		origin_displacement[2] = PLAYER_START_POSZ - 0.5;
	}

	//Set the face_iter mappings
	void prepIterator();

	//Move forward - increment the global origins & global centers and apply pos'
	void forward();
	//Move backward - decrement the global origins & global centers and apply pos'
	void backward();

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

	//Handle movement and rotational collision of an entity with walls. - Not defining rotation collision for maze
	bool collision();

	void checkBehind();


	/*
		//Set new absolute camera facing direction. - DISABLED!
		void absFaceCamera(Face direction);

		//Rotate camera view 45deg in L|R direction. 0 = L, 1 = R - DISABLED!
		void angleTurn(int dir);

		//Move the eye position and its focus point - DISABLED!
		void moveForward();

		//Move the eye position and its focus point -DISABLED!
		void moveBackward();
	*/
};

struct oCamera
{
	pCamera* target = NULL;
	std::array<double, 4> g_eye_pos = { 0, 6.0, 0, 1.0 };
	std::array<double, 4> l_eye_pos = { 0, 0, 0, 1.0 };
	std::array<double, 4> g_center_pos = { 0, -1.0, 0, 1.0 };
	std::array<double, 4> l_center_pos = { 0, 0, 0, 1.0 };
	std::array<double, 3> up_vector = { 0, 0, -1 };

	void setTarget(pCamera* pCam);

	void update_ortho();
};