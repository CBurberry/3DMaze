#include "other.h"
#include <stddef.h>
#include "draw_text.h"

//BAD PRACTICE NOTIFICATION
//- Going to tie player pointer movement with pCamera movement becuase we don't care about extensibility

void display()
{
	glClearColor(0, 0, 0, 1);  // (In fact, this is the default.)
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
	else //View_mode = 1, ortho2d
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-20 , 20, -20, 20, 1.0f, 20.0f);
		//set rotation -transform based on rotation vector

		//position and orient camera
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt( oCam.g_eye_pos[0], oCam.g_eye_pos[1], oCam.g_eye_pos[2],			//Eye pos
			oCam.g_center_pos[0], oCam.g_center_pos[1], oCam.g_center_pos[2],		//Center pos
			oCam.up_vector[0], oCam.up_vector[1], oCam.up_vector[2]);				//Up vect
	}
	/* OBSOLETE
		//Update assumes the only derivate for movement is the movement keys.
		if (update_flag) 
		{
			update_current_plane();
		}
	*/

	//draw_axis();
	draw_all_planes();
	update_pointer();
	if (show_solution) 
	{
		draw_solution();
	}
	if (view_mode == 1) 
	{
		draw_text(215, 25, "START");
		draw_text(220, 470, "FINISH");
	}

	glutSwapBuffers(); 
}



void reshape(int w, int h)
{
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	// for perspective projection use the GLU helper
	// take FOV, ASPECT RATIO, NEAR, FAR 
	gluPerspective(80.0, 1.0f, 0.0f, 50.0f);
}

void init()
{
	g_plane = make_plane();
	grids.intialiseGH();
	grids.intialiseGV();
	preset_planes(grids);
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

	init(); 

	glutMainLoop(); 

	return 0; 
}
