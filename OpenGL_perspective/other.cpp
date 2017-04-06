#include "other.h"

int view_mode = 0;				//Mode: 0 - Perspective, 1 = Ortho2d
bool show_solution = false;

// handle for display list
unsigned int g_wall = 0;

std::vector<plane> plane_vec;
gridCombo grids;
std::vector<vec3> solution_vec;

pCamera pCam = pCamera(&grids);
oCamera oCam;


//After moving, if new position == (top - 1)'s position, pop top, else push new position.
void resolve_stack(pCamera& camera) 
{
	vec3 newPos = vec3(camera.origin_displacement[0], camera.origin_displacement[1], camera.origin_displacement[2]);
	int stack_size = solution_vec.size();
	vec3 prevPos = solution_vec[stack_size - 2];

	//Compare 2nd last element if it is equivalent to newPos. (Y is a constant)
	if (newPos.x == prevPos.x && newPos.z == prevPos.z) 
	{
		//Remove last element of stack (we're walking along path)
		solution_vec.pop_back();
	} else {
		//Append the current position (newPos) to stack.
		solution_vec.push_back(newPos);
	}
}


//When moving, if new position == top - 1, pop top, else push new position.
void setup_solution_stack() 
{
	//Stack Bottom
	solution_vec.push_back(vec3(0, 0, -14));
	solution_vec.push_back(vec3(0, 0, -12));
	solution_vec.push_back(vec3(-2, 0, -12));
	solution_vec.push_back(vec3(-2, 0, -14));
	solution_vec.push_back(vec3(-4, 0, -14));
	solution_vec.push_back(vec3(-4, 0, -12));
	solution_vec.push_back(vec3(-6, 0, -12));
	solution_vec.push_back(vec3(-6, 0, -14));
	solution_vec.push_back(vec3(-8, 0, -14));
	solution_vec.push_back(vec3(-8, 0, -12));
	solution_vec.push_back(vec3(-10, 0, -12));
	solution_vec.push_back(vec3(-10, 0, -14));
	solution_vec.push_back(vec3(-12, 0, -14));
	solution_vec.push_back(vec3(-12, 0, -12));
	solution_vec.push_back(vec3(-12, 0, -10));
	solution_vec.push_back(vec3(-14, 0, -10));
	solution_vec.push_back(vec3(-14, 0, -8));
	solution_vec.push_back(vec3(-12, 0, -8));
	solution_vec.push_back(vec3(-12, 0, -6));
	solution_vec.push_back(vec3(-10, 0, -6));
	solution_vec.push_back(vec3(-10, 0, -4));
	solution_vec.push_back(vec3(-12, 0, -4));
	solution_vec.push_back(vec3(-12, 0, -2));
	solution_vec.push_back(vec3(-12, 0, 0));
	solution_vec.push_back(vec3(-12, 0, 2));
	solution_vec.push_back(vec3(-10, 0, 2));
	solution_vec.push_back(vec3(-8, 0, 2));
	solution_vec.push_back(vec3(-8, 0, 0));
	solution_vec.push_back(vec3(-10, 0, 0));
	solution_vec.push_back(vec3(-10, 0, -2));
	solution_vec.push_back(vec3(-8, 0, -2));
	solution_vec.push_back(vec3(-6, 0, -2));
	solution_vec.push_back(vec3(-4, 0, -2));
	solution_vec.push_back(vec3(-4, 0, -4));
	solution_vec.push_back(vec3(-4, 0, -6));
	solution_vec.push_back(vec3(-4, 0, -8));
	solution_vec.push_back(vec3(-2, 0, -8));
	solution_vec.push_back(vec3(-2, 0, -6));
	solution_vec.push_back(vec3(0, 0, -6));
	solution_vec.push_back(vec3(2, 0, -6));
	solution_vec.push_back(vec3(2, 0, -8));
	solution_vec.push_back(vec3(4, 0, -8));
	solution_vec.push_back(vec3(6, 0, -8));
	solution_vec.push_back(vec3(6, 0, -6));
	solution_vec.push_back(vec3(4, 0, -6));
	solution_vec.push_back(vec3(4, 0, -4));
	solution_vec.push_back(vec3(6, 0, -4));
	solution_vec.push_back(vec3(6, 0, -2));
	solution_vec.push_back(vec3(8, 0, -2));
	solution_vec.push_back(vec3(8, 0, 0));
	solution_vec.push_back(vec3(8, 0, 2));
	solution_vec.push_back(vec3(8, 0, 4));
	solution_vec.push_back(vec3(8, 0, 6));
	solution_vec.push_back(vec3(10, 0, 6));
	solution_vec.push_back(vec3(10, 0, 4));
	solution_vec.push_back(vec3(12, 0, 4));
	solution_vec.push_back(vec3(12, 0, 6));
	solution_vec.push_back(vec3(12, 0, 8));
	solution_vec.push_back(vec3(10, 0, 8));
	solution_vec.push_back(vec3(10, 0, 10));
	solution_vec.push_back(vec3(8, 0, 10));
	solution_vec.push_back(vec3(6, 0, 10));
	solution_vec.push_back(vec3(4, 0, 10));
	solution_vec.push_back(vec3(2, 0, 10));
	solution_vec.push_back(vec3(0, 0, 10));
	solution_vec.push_back(vec3(-2, 0, 10));
	solution_vec.push_back(vec3(-4, 0, 10));
	solution_vec.push_back(vec3(-4, 0, 8));
	solution_vec.push_back(vec3(-6, 0, 8));
	solution_vec.push_back(vec3(-6, 0, 10));
	solution_vec.push_back(vec3(-6, 0, 12));
	solution_vec.push_back(vec3(-4, 0, 12));
	solution_vec.push_back(vec3(-2, 0, 12));
	solution_vec.push_back(vec3(0, 0, 12));
	solution_vec.push_back(vec3(0, 0, 14));
	//Stack top
}

void draw_solution() 
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);		//Green
	for ( auto elem : solution_vec ) 
	{
		glVertex3f(elem.x, elem.y, elem.z);
	}
	glEnd();
}




//Add a pre-defined set of planes to the plane_vec vector to be drawn.
void preset_planes(gridCombo& source)
{
	for (int y = 0; y < MAXH_ROWS; y++) 
	{
		for (int x = 0; x < MAXH_PER_ROW; x++) 
		{
			if (source.gridH[y][x] == 1) 
			{
				plane_vec.push_back(plane(0, vec3((x - 7) * 2, FLOOR_OFFSET, -(y - 7) * 2 + 1), vec3(90, 0, 0)));
			}
		}
	}


	for (int y = 0; y < MAXV_ROWS; y++) 
	{
		for (int x = 0; x < MAXV_PER_ROW; x++) 
		{
			if (source.gridV[y][x] == 1) 
			{
				plane_vec.push_back(plane(0, vec3((x - 7) * 2 - 1, FLOOR_OFFSET, -(y - 7) * 2), vec3(0, 0, 90)));
			}
		}
	}
}


void update_pointer()
{
	glPushMatrix();
	glTranslatef(pCam.icon.absTranslation[0], 0.0, pCam.icon.absTranslation[2]);
	glRotatef((int)(-1) * pCam.icon.localRotation[1], 0, 1, 0);	

	glBegin(GL_TRIANGLES);
	glColor3f(pCam.icon.color.x, pCam.icon.color.y, pCam.icon.color.z);
	glVertex3f(pCam.icon.vert1.x, pCam.icon.vert1.y, pCam.icon.vert1.z);
	glVertex3f(pCam.icon.vert2.x, pCam.icon.vert2.y, pCam.icon.vert2.z);
	glVertex3f(pCam.icon.vert3.x, pCam.icon.vert3.y, pCam.icon.vert3.z);
	glEnd();
	glPopMatrix();
}

unsigned int make_wall()
{
	// request a single display list handle
	unsigned int handle = glGenLists(1);

	glNewList(handle, GL_COMPILE);

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0); // red
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glColor3f(0, 1, 0); // green
	glVertex3f(1.0f, 0.0f, -1.0f);
	glColor3f(0, 0, 1); // blue
	glVertex3f(1.0f, 0.0f, 1.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glEnd();


	glBegin(GL_LINE_LOOP);
	glColor3f(1, 0, 0); // red
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glColor3f(1, 0, 0); // red
	glVertex3f(1.0f, 0.0f, -1.0f);
	glColor3f(1, 0, 0); // red
	glVertex3f(1.0f, 0.0f, 1.0f);
	glColor3f(1, 0, 0); // red
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glEnd();
	glEndList();

	return handle;
}


void draw_all_walls()
{
	for (auto plane : plane_vec)
	{
		glPushMatrix();

		//Transforms
		glTranslatef(plane.displacement.x, plane.displacement.y, plane.displacement.z);
		glRotatef(plane.rotation.x, 1, 0, 0);
		glRotatef(plane.rotation.y, 0, 1, 0);
		glRotatef(plane.rotation.z, 0, 0, 1);
		glCallList(g_wall);
		glPopMatrix();
	}
}

void animatedTurn()
{
	std::clock_t start;
	start = std::clock();
	double elapsedTime = 0.0;

	//Rotate 45deg then delay until next 45deg rotation to complete.
	if (pCam.turning)
	{
		while (elapsedTime < TURN_DURATION)
		{
			elapsedTime = (std::clock() - start) / (double) CLOCKS_PER_SEC;
		}
		pCam.rotate(pCam.turn_angle / 2);
		pCam.turn_angle = 0.0;
		pCam.turning = false;

		//Turn off animation loop.
		glutIdleFunc(NULL);
	}
	else 
	{
		pCam.rotate(pCam.turn_angle / 2);
		pCam.turning = true;
	}

	glutPostRedisplay();
}
