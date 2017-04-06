#include "other.h"
#include <stddef.h>
#include "draw_text.h"

//Colour values to pass into draw_text.
std::array<float, 3> y_color{ 1.0, 1.0, 0.0 };
std::array<float, 3> w_color{ 1.0, 1.0, 1.0 };


void display()
{
	glClearColor(0, 0, 0, 1); 
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); 
	glLineWidth(3.0f);

	if (view_mode == 0) //perspective
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(80.0, 1.0f, 1.0f, 20.0f);

		// position and orient camera
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(pCam.g_eye_pos[0], pCam.g_eye_pos[1], pCam.g_eye_pos[2],		// eye position
			pCam.g_center_pos[0], pCam.g_center_pos[1], pCam.g_center_pos[2],	// reference point
			pCam.up_vector[0], pCam.up_vector[1], pCam.up_vector[2]				// up vector
		);
	}
	else //View_mode = 1, ortho2d - Orthogonal camera
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-20 , 20, -20, 20, 1.0f, 20.0f);

		//position and orient camera
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt( oCam.g_eye_pos[0], oCam.g_eye_pos[1], oCam.g_eye_pos[2],			//Eye pos
			oCam.g_center_pos[0], oCam.g_center_pos[1], oCam.g_center_pos[2],		//Center pos
			oCam.up_vector[0], oCam.up_vector[1], oCam.up_vector[2]);				//Up vect
	}

	//Draw all the walls and the player icon.
	draw_all_walls();
	update_pointer();

	//If solution was toggled by player.
	if (show_solution) 
	{
		draw_solution();
	}
	//Only show map helper text when not in win position
	if (view_mode == 0 && pCam.win == false) 
	{
		draw_text(5, 490, "m - Map", 0.15, y_color);
	}

	//Draw text only in orthogonal.
	if (view_mode == 1) 
	{
		draw_text(215, 25, "START", 0.2, y_color);
		draw_text(220, 470, "FINISH", 0.2, y_color);
		draw_text(5, 490, "HOME - Solution", 0.15, w_color);
	}

	if (pCam.win) 
	{
		draw_text(216, 300, "You're", 0.3, y_color);
		draw_text(210, 255, "Winner!", 0.3, y_color);
	}

	glutSwapBuffers(); 

	//After win is set, quit after 4s.
	if (pCam.win) 
	{
		std::clock_t start = std::clock();
		double elapsedTime = 0.0;
		while (elapsedTime < 4.0) 
		{
			elapsedTime = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		}
		exit(0);
	}
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	gluPerspective(80.0, 1.0f, 0.0f, 50.0f);
}

void keyboard(unsigned char key, int, int)
{
	switch (key)
	{
	//Map toggle.
	case 'm':case 'M': view_mode == 1 ? view_mode = 0 : view_mode = 1;
		break;
	//Esc to quit.
	case 27: exit(0);
		break;
	}

	glutPostRedisplay();
}

void special(int key, int, int)
{
	switch (key)
	{
	case GLUT_KEY_HOME:
		//Toggle solution
		show_solution ? show_solution = false : show_solution = true;
		break;

		//Movement only allowed in perspective view.
	case GLUT_KEY_UP:
		if (view_mode == 0) 
		{
			if (pCam.forward())
				resolve_stack(pCam);
		}
		break;
	case GLUT_KEY_LEFT:
		if (view_mode == 0)
		{
			pCam.turn_angle = -90.0;
			glutIdleFunc(animatedTurn);
		}
		break;
	case GLUT_KEY_DOWN:
		if (view_mode == 0)
		{
			if (pCam.backward())
				resolve_stack(pCam);
		}
		break;
	case GLUT_KEY_RIGHT:
		if (view_mode == 0)
		{
			pCam.turn_angle = 90.0;
			glutIdleFunc(animatedTurn);
		}
		break;
	}

	glutPostRedisplay();
}

void init()
{
	g_wall = make_wall();
	grids.intialiseGH();
	grids.intialiseGV();
	preset_planes(grids);
	setup_solution_stack();
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); 
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH); 
	
	glutInitWindowSize(512, 512); 
	glutInitWindowPosition(50, 50); 

	glutCreateWindow("Perspective Viewing"); 

	glutKeyboardFunc(keyboard); 
	glutSpecialFunc(special);
	glutReshapeFunc(reshape); 
	glutDisplayFunc(display); 
	glutIdleFunc(animatedTurn);

	init(); 

	glutMainLoop(); 

	return 0; 
}
