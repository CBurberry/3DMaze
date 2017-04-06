#include "other.h"

float g_trans_x = 0.0f;
float g_trans_y = 0.0f;
float g_trans_z = 0.0f;
int g_axis_selection = 3;
float g_angle = 0.0f;

bool update_flag = false;   //set if current plane should update based on trans.
int view_mode = 0; //Mode: 0 - Perspective, 1 = Ortho2d
bool show_solution = false;

// handle for display list
unsigned int g_plane = 0;
std::vector<plane> plane_vec;
gridCombo grids;

pCamera pCam = pCamera(&grids);
oCamera oCam;

//Add a pre-defined set of planes to the plane_vec vector to be drawn. - CURRENTLY USING TO DRAW MAZE LEVEL
void preset_planes(gridCombo& source)
{


	/*
		INFO
		- Currently floor (&ceiling) planes are not offset, such that player pointer is centered in a plane.
		- Walls raised by 1.0 along y.
		- Walls rotate about their origin so there is a need to displace them by -1.0x for z-aligned and 1.0z for x-aligned 
			(such that each wall is at 0->1 of the respective axis given that the plane is at (0, 0 ,0))
		- x-aligned walls are rotated about the x-axis, z-aligned walls rotated about z etc. (90deg) 
		- For the entire definition of the 15x15 orthogonal maze BL = (-14, -14), TR = (14, 14), O = (0, 0)

		MINS/MAXES OF EACH WALL POSITIONAL DIMENSIONS
		horizontal trans_x -14.0 to 14.0
		horizontal trans_z -16.0 to 14.0
		vertical trans_x -14.0 to 16.0
		vertical trans_z -14.0 to 14.0
		pattern   minH = (minH_trans_x, 1.0, maxH_trans_z + 1.0), minV = ((maxH_trans_z + 1.0) * (-1), 1.0, (minH_trans_x) * (-1))
				  maxH = (maxH_trans_x, 1.0, minH_trans_z + 1.0), maxV = ((minH_trans_z + 1.0) * (-1), 1.0, (maxH_trans_x) * (-1))
		Able to define the vertical in terms of horizontal positioning.
		On grid 0,0 wall is minH and minV for horizontal (aligned bottom) and vertical (aligned left) respectively.
		On grid 15,15 wall is maxH and maxV for horizontal (aligned top) and vertical (aligned right) respectively.
	*/

	//Walls via arrays - although we can use the above loop, for just outer walls when we create inner walls this practice will be better.
	//Remember x = x, y = -z
	for (int y = 0; y < MAXH_ROWS; y++) 
	{
		for (int x = 0; x < MAXH_PER_ROW; x++) 
		{
			if (source.gridH[y][x] == 1) 
			{
				//Horizontal (7 either side of center)
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
				//Vertical (7 behind 8 ahead of center)
				plane_vec.push_back(plane(0, vec3((x - 7) * 2 - 1, FLOOR_OFFSET, -(y - 7) * 2), vec3(0, 0, 90)));
			}
		}
	}



	//FLOORS - DISABLED FOR MAZE CW
	/*
		for (int j = (-NUM_OF_SQV); j < NUM_OF_SQV + 1; j += 2)
		{
			for (int i = (-NUM_OF_SQH); i < NUM_OF_SQH + 1; i += 2)
			{
				plane_vec.push_back(plane(0, vec3(i, 0, (-j)), vec3(0, 0, 0)));
			}
		}
	*/

	//CEILINGS - NOT BEING EXTENDED TO FOR MAZE CW






	// CROSS - DEV USAGE ONLY
	/*
		plane_vec.push_back(plane(0, vec3(0, 0, 0), vec3(0, 0, 0)));
		plane_vec.push_back(plane(1, vec3(2, 0, 0), vec3(0, 0, 0)));
		plane_vec.push_back(plane(2, vec3(4, 0, 0), vec3(0, 0, 0)));
		plane_vec.push_back(plane(3, vec3(-2, 0, 0), vec3(0, 0, 0)));
		plane_vec.push_back(plane(4, vec3(-4, 0, 0), vec3(0, 0, 0)));
		plane_vec.push_back(plane(5, vec3(0, 0, 2), vec3(0, 0, 0)));
		plane_vec.push_back(plane(6, vec3(0, 0, 4), vec3(0, 0, 0)));
		plane_vec.push_back(plane(7, vec3(0, 0, -2), vec3(0, 0, 0)));
		plane_vec.push_back(plane(8, vec3(0, 0, -4), vec3(0, 0, 0)));
	*/
}

void draw_pointer()
{
	pointer p = pCam.icon;
	glBegin(GL_TRIANGLES);
	glColor3f(p.color.x, p.color.y, p.color.z);
	glVertex3f(p.vert1.x, p.vert1.y, p.vert1.z);
	glVertex3f(p.vert2.x, p.vert2.y, p.vert2.z);
	glVertex3f(p.vert3.x, p.vert3.y, p.vert3.z);
	glEnd();
}

void update_pointer()
{
	glPushMatrix();
	glTranslatef(pCam.icon.absTranslation[0], 0.0, pCam.icon.absTranslation[2]);
	//We assume that the icon is always being rotated about Y, value is assigned in pCam.rotate
	glRotatef((int)(-1) * pCam.icon.localRotation[1], 0, 1, 0);							//NOT QUITE SURE WHY NEEDED TO *(-1) TO ROTATE CW

	glBegin(GL_TRIANGLES);
	glColor3f(pCam.icon.color.x, pCam.icon.color.y, pCam.icon.color.z);
	glVertex3f(pCam.icon.vert1.x, pCam.icon.vert1.y, pCam.icon.vert1.z);
	glVertex3f(pCam.icon.vert2.x, pCam.icon.vert2.y, pCam.icon.vert2.z);
	glVertex3f(pCam.icon.vert3.x, pCam.icon.vert3.y, pCam.icon.vert3.z);
	glEnd();
	glPopMatrix();
}

void draw_axis()
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); //red
	glVertex3f(0.0, 0.0, -20.0);
	glVertex3f(0.0, 0.0, 20.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0, 0.0, 1.0); //blue
	glVertex3f(-20.0, 0.0, 0.0);
	glVertex3f(20.0, 0.0, 0.0);
	glEnd();
}

void draw_solution() 
{
	glBegin(GL_LINE_STRIP);
	glColor3f(0, 1, 0);	//Green
	glVertex3f(0, 0, 14);
	glVertex3f(0, 0, 12);
	glVertex3f(-6, 0, 12);
	glVertex3f(-6, 0, 8);
	glVertex3f(-4, 0, 8);
	glVertex3f(-4, 0, 10);
	glVertex3f(10, 0, 10);
	glVertex3f(10, 0, 8);
	glVertex3f(12, 0, 8);
	glVertex3f(12, 0, 4);
	glVertex3f(10, 0, 4);
	glVertex3f(10, 0, 6);
	glVertex3f(8, 0, 6);
	glVertex3f(8, 0, -2);
	glVertex3f(6, 0, -2);
	glVertex3f(6, 0, -4);
	glVertex3f(4, 0, -4);
	glVertex3f(4, 0, -6);
	glVertex3f(6, 0, -6);
	glVertex3f(6, 0, -8);
	glVertex3f(2, 0, -8);
	glVertex3f(2, 0, -6);
	glVertex3f(-2, 0, -6);
	glVertex3f(-2, 0, -8);
	glVertex3f(-4, 0, -8);
	glVertex3f(-4, 0, -2);
	glVertex3f(-10, 0, -2);
	glVertex3f(-10, 0, 0);
	glVertex3f(-8, 0, 0);
	glVertex3f(-8, 0, 2);
	glVertex3f(-12, 0, 2);
	glVertex3f(-12, 0, -4);
	glVertex3f(-10, 0, -4);
	glVertex3f(-10, 0, -6);
	glVertex3f(-12, 0, -6);
	glVertex3f(-12, 0, -8);
	glVertex3f(-14, 0, -8);
	glVertex3f(-14, 0, -10);
	glVertex3f(-12, 0, -10);
	glVertex3f(-12, 0, -14);
	glVertex3f(-10, 0, -14);
	glVertex3f(-10, 0, -12);
	glVertex3f(-8, 0, -12);
	glVertex3f(-8, 0, -14);
	glVertex3f(-6, 0, -14);
	glVertex3f(-6, 0, -12);
	glVertex3f(-4, 0, -12);
	glVertex3f(-4, 0, -14);
	glVertex3f(-2, 0, -14);
	glVertex3f(-2, 0, -12);
	glVertex3f(0, 0, -12);
	glVertex3f(0, 0, -14);
	glEnd();
}


unsigned int make_plane()
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

void control_help()
{
	system("cls");
	std::cout << "Eye pos controls: i, o, p" << std::endl;
	std::cout << "Eye center controls: j, k, l" << std::endl;
	std::cout << "Eye up controls: b, n, m" << std::endl;
	std::cout << "Plane translation controls: q, w, e" << std::endl;
	std::cout << "Rotation axis selection: 1,2,3 | Rotation angle adjustment: '+'/'-'" << std::endl;
	std::cout << "SPACE: Camera status, HOME: Plane status, INSERT/DELETE" << std::endl;
}

void create_new_plane()
{
	if (plane_vec.size() == 0)
	{
		//Create a new plane based on global translation parameters
		plane_vec.push_back(plane(plane_vec.size(), vec3(g_trans_x, g_trans_y, g_trans_z), vec3(0, 0, 0)));
		std::cout << "ID " << plane_vec.size() - 1 << " Activated!" << std::endl;
	}
	else
	{
		//Create a new plane based on the predecessors parameters for translation and rotation.
		plane_vec.push_back(plane(plane_vec.size(), vec3(g_trans_x, g_trans_y, g_trans_z),
			vec3(plane_vec[plane_vec.size() - 1].rotation.x, plane_vec[plane_vec.size() - 1].rotation.y, plane_vec[plane_vec.size() - 1].rotation.z)));
		std::cout << "ID " << plane_vec.size() - 1 << " Activated!" << std::endl;
	}
}

void update_current_plane()	//OBSOLETE
{
	if (plane_vec.size() == 0)
	{
		return;
	}
	plane_vec[plane_vec.size() - 1].displacement = vec3(g_trans_x, g_trans_y, g_trans_z);
}

void draw_all_planes()
{
	for (auto plane : plane_vec)
	{
		glPushMatrix();

		//Transforms
		glTranslatef(plane.displacement.x, plane.displacement.y, plane.displacement.z);
		glRotatef(plane.rotation.x, 1, 0, 0);
		glRotatef(plane.rotation.y, 0, 1, 0);
		glRotatef(plane.rotation.z, 0, 0, 1);
		glCallList(g_plane);
		glPopMatrix();
	}
}

void plane_displaced(vec3 axis_displacement, vec4 rotation)
{
	glPushMatrix();
	glTranslatef(axis_displacement.x, axis_displacement.y, axis_displacement.z);
	glRotatef(rotation.a, rotation.b, rotation.c, rotation.d);
	glCallList(g_plane);
	glPopMatrix();
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
		//Camera displacement controls
	case 'i': pCam.g_eye_pos[0] += 0.5f; break;
	case 'I': pCam.g_eye_pos[0] -= 0.5f; break;
	case 'o': pCam.g_eye_pos[1] += 0.5f; break;
	case 'O': pCam.g_eye_pos[1] -= 0.5f; break;
	case 'p': pCam.g_eye_pos[2] += 0.5f; break;
	case 'P': pCam.g_eye_pos[2] -= 0.5f; break;
	case 'j': pCam.g_center_pos[0] += 0.5f; break;
	case 'J': pCam.g_center_pos[0] -= 0.5f; break;
	case 'k': pCam.g_center_pos[1] += 0.5f; break;
	case 'K': pCam.g_center_pos[1] -= 0.5f; break;
	case 'l': pCam.g_center_pos[2] += 0.5f; break;
	case 'L': pCam.g_center_pos[2] -= 0.5f; break;
	case 'b': pCam.up_vector[0] += 0.5f; break;
	case 'B': pCam.up_vector[0] -= 0.5f; break;
	case 'n': pCam.up_vector[1] += 0.5f; break;
	case 'N': pCam.up_vector[1] -= 0.5f; break;
	case 'm': pCam.up_vector[2] += 0.5f; break;
	case 'M': pCam.up_vector[2] -= 0.5f; break;

		//Debug Plane controls - translation & rotation
	case 'q':  g_trans_x += 0.5f; break;
	case 'Q':  g_trans_x -= 0.5f; break;
	case 'w':  g_trans_y += 0.5f; break;
	case 'W':  g_trans_y -= 0.5f; break;
	case 'e':  g_trans_z += 0.5f; break;
	case 'E':  g_trans_z -= 0.5f; break;
	case '+':
		switch (g_axis_selection)
		{
		case 1: plane_vec[plane_vec.size() - 1].rotation.x += 5.0f; break;
		case 2: plane_vec[plane_vec.size() - 1].rotation.y += 5.0f; break;
		case 3: plane_vec[plane_vec.size() - 1].rotation.z += 5.0f; break;
		}
		g_angle += 5.0f;
		break;
	case '-':
		switch (g_axis_selection)
		{
		case 1: plane_vec[plane_vec.size() - 1].rotation.x -= 5.0f; break;
		case 2: plane_vec[plane_vec.size() - 1].rotation.y -= 5.0f; break;
		case 3: plane_vec[plane_vec.size() - 1].rotation.z -= 5.0f; break;
		}
		g_angle -= 5.0f;
		break;
	case '1': g_axis_selection = 1; break;
	case '2': g_axis_selection = 2; break;
	case '3': g_axis_selection = 3; break;

	case 'r': glutPostRedisplay(); break;
	case ' ':  	system("cls");
		std::cerr << "eye x  = " << pCam.g_eye_pos[0] << " || eye y  = " << pCam.g_eye_pos[1] << " ||eye z  = " << pCam.g_eye_pos[2] << std::endl;
		std::cerr << "center x = " << pCam.g_center_pos[0] << " || center y = " << pCam.g_center_pos[1] << " || center z = " << pCam.g_center_pos[2] << std::endl;
		std::cerr << "up x = " << pCam.up_vector[0] << " || up y = " << pCam.up_vector[1] << " || up z = " << pCam.up_vector[2] << std::endl;
		break;
	case 127: //Delete key
		if (plane_vec.size() == 0)
		{
			g_trans_x = 0;
			g_trans_y = 0;
			g_trans_z = 0;
			break;
		}
		plane_vec.pop_back();
		std::cout << "ID " << plane_vec.size() << " Removed!" << std::endl;
		break;
	case 'v':case 'V': view_mode == 1 ? view_mode = 0 : view_mode = 1;
		break;
	case '0': control_help();
		break;
	}

	glutPostRedisplay();
}

void special(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_INSERT:
		create_new_plane();
		break;
	case GLUT_KEY_HOME:
		//Toggle solution
		show_solution ? show_solution = false : show_solution = true;
		break;

	//Movement only allowed in perspective view.
	case GLUT_KEY_UP:
		if (view_mode == 0) 
			pCam.forward();
		break;
	case GLUT_KEY_LEFT:
		if (view_mode == 0)
			pCam.rotate(-90.0);
		break;
	case GLUT_KEY_DOWN:
		if (view_mode == 0)
			pCam.backward();
		break;
	case GLUT_KEY_RIGHT:
		if (view_mode == 0)
			pCam.rotate(90.0);
		break;
	}

	glutPostRedisplay();
}