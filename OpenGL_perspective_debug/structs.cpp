#include "structs.h"

void pCamera::prepIterator() 
{
	face_iter[0] = Face::North;
	face_iter[1] = Face::Northeast;
	face_iter[2] = Face::East;
	face_iter[3] = Face::Southeast;
	face_iter[4] = Face::South;
	face_iter[5] = Face::Southwest;
	face_iter[6] = Face::West;
	face_iter[7] = Face::Northwest;
}

void pCamera::checkBehind() //- DEBUG ONLY
{
	int grid_pos_x = (origin_displacement[0] + 14) / 2;
	int grid_pos_y = ((-1) * (origin_displacement[2]) + 14) / 2;
	system("cls");
	std::cout << "\n\ncurrent pos: " << grid_pos_x << ", " << grid_pos_y << std::endl;
	//N
	/*
	std::cout << "ahead pos: " << grid_pos_x << ", " << grid_pos_y + 1  << std::endl;
	if (grids->gridH[grid_pos_y + 1][grid_pos_x] == 1)
	{
		std::cout << "wall ahead" << std::endl;
	}*/

	
	//E
	/*std::cout << "ahead pos: " << grid_pos_x + 1 << ", " << grid_pos_y << std::endl;
	if (grids->gridV[grid_pos_y][grid_pos_x + 1])
	{
		std::cout << "wall ahead" << std::endl;
	}*/

	//S - No distance to the southern wall, current = ahead.
	/*std::cout << "ahead pos: " << grid_pos_x << ", " << grid_pos_y << std::endl;
	if (grids->gridH[grid_pos_y][grid_pos_x] == 1)
	{
		std::cout << "wall ahead" << std::endl;
	}*/

	//W - No distance to the left wall, current = ahead.
	/*std::cout << "ahead pos: " << grid_pos_x << ", " << grid_pos_y << std::endl;
	if (grids->gridV[grid_pos_y][grid_pos_x])
	{
		std::cout << "wall ahead" << std::endl;
	}*/


//	grids->printGH();
}


void pCamera::forward() 
{
	int grid_pos_x = (origin_displacement[0] + 14) / 2;
	int grid_pos_y = ((-1) * (origin_displacement[2]) + 14) / 2;

	//Get facing direction
	//Return if not axis aligned facing
	if (this->direction != 0 && this->direction != 2 && 
		this->direction != 4 && this->direction != 6)
	{
		std::cout << "Current direction: " << this->direction << " | not aligned" << std::endl;
		return;
	}

	//Displace the relative 'forward' on global position
	//Displacement to the origin_displacement vector on the moved axis.
	switch (this->direction) 
	{
		//& Update player pointer - set pointer variables and update in main.cpp
		//& set if a wall is now ahead

	case 0: //N
		if (grids->gridH[grid_pos_y + 1][grid_pos_x] == 1)
		{
			return;
		}
		g_eye_pos[2] -= 2;
		g_center_pos[2] -= 2;
		origin_displacement[2] -= 2;
		icon.absTranslation[2] -= 2;
		break;
	case 2: //E
		if (grids->gridV[grid_pos_y][grid_pos_x + 1])
		{
			return;
		}
		g_eye_pos[0] += 2;
		g_center_pos[0] += 2;
		origin_displacement[0] += 2;
		icon.absTranslation[0] += 2;
		break;
	case 4: //S
		if (grids->gridH[grid_pos_y][grid_pos_x] == 1)
		{
			return;
		}
		g_eye_pos[2] += 2;
		g_center_pos[2] += 2;
		origin_displacement[2] += 2;
		icon.absTranslation[2] += 2;
		break;
	case 6: //W
		if (grids->gridV[grid_pos_y][grid_pos_x])
		{
			return;
		}
		g_eye_pos[0] -= 2;
		g_center_pos[0] -= 2;
		origin_displacement[0] -= 2;
		icon.absTranslation[0] -= 2;
		break;
	}
}

void pCamera::backward() 
{
	//Get facing direction
	//Return if not axis aligned facing
	if (this->direction != 0 && this->direction != 2 &&
		this->direction != 4 && this->direction != 6)
	{
		std::cout << "Current direction: " << this->direction << " | not aligned" << std::endl;
		return;
	}

	//Displace the relative 'backward' on global position
	//Displacement to the origin_displacement vector on the moved axis.
	switch (this->direction)
	{
	//Update player pointer - set pointer variables and update in main.cpp
	//& set if a wall is now behind.

	case 0: //N
		g_eye_pos[2] += 2;
		g_center_pos[2] += 2;
		origin_displacement[2] += 2;
		icon.absTranslation[2] += 2;
		break;
	case 2: //E
		g_eye_pos[0] -= 2;
		g_center_pos[0] -= 2;
		origin_displacement[0] -= 2;
		icon.absTranslation[0] -= 2;
		break;
	case 4: //S
		g_eye_pos[2] -= 2;
		g_center_pos[2] -= 2;
		origin_displacement[2] -= 2;
		icon.absTranslation[2] -= 2;
		break;
	case 6: //W
		g_eye_pos[0] += 2;
		g_center_pos[0] += 2;
		origin_displacement[0] += 2;
		icon.absTranslation[0] += 2;
		break;
	}
}

void pCamera::rotate(double theta) 
{
	int i = 0;
	//Create rotating matrix about y using theta
	Matrix4x4 rotateMatrix_y = defineRotationMatrix(2, theta);
	//apply rotation to local eye_pos&center_pos
	std::array<double, 4> rotatedLocalEye = matrixTimesVector(rotateMatrix_y, l_eye_pos);
	std::array<double, 4> rotatedLocalCenter = matrixTimesVector(rotateMatrix_y, l_center_pos);
	//get new global eye_pos&center_pos by adding origin_displacement to rotated vectors
	//Overwrite current global/local eye_pos&center_pos
	g_eye_pos[0] = rotatedLocalEye[0] + origin_displacement[0];
	g_eye_pos[2] = rotatedLocalEye[2] + origin_displacement[2];
	g_center_pos[0] = rotatedLocalCenter[0] + origin_displacement[0];
	g_center_pos[2] = rotatedLocalCenter[2] + origin_displacement[2];
	if (theta != 45.0 && theta != -45.0 && theta != 90.0 
		&& theta != -90.0 && theta != 0.0) 
	{
		//Currently no intention of supporting other angles beyond listed above.
		std::cout << "theta: " << theta << std::endl;
		while (true) {}
		exit(1);
	}

	//Traunicate all double zeroes to remove negatives.
	g_eye_pos[0] = traunicate(g_eye_pos[0]);
	g_eye_pos[2] = traunicate(g_eye_pos[2]);
	g_center_pos[0] = traunicate(g_center_pos[0]);
	g_center_pos[2] = traunicate(g_center_pos[2]);
	l_eye_pos[0] = traunicate(l_eye_pos[0]);
	l_eye_pos[2] = traunicate(l_eye_pos[2]);
	l_center_pos[0] = traunicate(l_center_pos[0]);
	l_center_pos[2] = traunicate(l_center_pos[2]);
	//update local pos vectors with rotations.
	l_eye_pos = rotatedLocalEye;
	l_center_pos = rotatedLocalCenter;

	//Update facing direction.
	theta == 45.0 ? this->direction++ : theta == 90.0 ? this->direction += 2 : i = 0;
	theta == -45.0 ? this->direction-- : theta == -90.0 ? this->direction -= 2 : i = 0;
	this->direction == -1 ? this->direction = 7 : i = 1;
	this->direction == -2 ? this->direction = 6 : i = 1;
	this->direction == 8 ? this->direction = 0 : i = 1;
	this->direction == 9 ? this->direction = 1 : i = 1;


	//Update player pointer - set pointer variables and update in main.cpp
	icon.localRotation[1] += theta;

	//Set if a wall is ahead && set if a wall is behind.
}

void oCamera::setTarget(pCamera* target) 
{
	this->target = target;
}

void oCamera::update_ortho()
{
	g_eye_pos[0] = target->origin_displacement[0];
	g_eye_pos[2] = target->origin_displacement[2];
	g_center_pos[0] = target->origin_displacement[0];
	g_center_pos[2] = target->origin_displacement[2];
}

void gridCombo::intialiseGH() 
{
	std::array<int, MAXH_ROWS> Hcol0 = { 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol1 = { 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol2 = { 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol3 = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol4 = { 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol5 = { 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol6 = { 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol7 = { 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0 };
	std::array<int, MAXH_ROWS> Hcol8 = { 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol9 = { 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1 };
	std::array<int, MAXH_ROWS> Hcol10 = { 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1 };
	std::array<int, MAXH_ROWS> Hcol11 = { 1, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1 };
	std::array<int, MAXH_ROWS> Hcol12 = { 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1 };
	std::array<int, MAXH_ROWS> Hcol13 = { 1, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1 };
	std::array<int, MAXH_ROWS> Hcol14 = { 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1 };
	std::vector<std::array<int, MAXH_ROWS>> Hcol_vec = { Hcol0, Hcol1, Hcol2, Hcol3, Hcol4, Hcol5, Hcol6, Hcol7,
		Hcol8, Hcol9, Hcol10, Hcol11, Hcol12, Hcol13, Hcol14 };
	this->Hcol_vec = Hcol_vec;

	//For horizontal outer walls: create every wall in the first and last row.
	for (int y = 0; y < MAXH_ROWS; y++)
	{
		for (int x = 0; x < MAXH_PER_ROW; x++)
		{
			gridH[y][x] = Hcol_vec[x][y];
		}
	}
}

void gridCombo::intialiseGV()
{
	std::array<int, MAXV_PER_ROW> Vrow14 = { 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow13 = { 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1 };
	std::array<int, MAXV_PER_ROW> Vrow12 = { 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow11 = { 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow10 = { 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow9 = { 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1 };
	std::array<int, MAXV_PER_ROW> Vrow8 = { 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow7 = { 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 1 };
	std::array<int, MAXV_PER_ROW> Vrow6 = { 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow5 = { 1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow4 = { 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1 };
	std::array<int, MAXV_PER_ROW> Vrow3 = { 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1 };
	std::array<int, MAXV_PER_ROW> Vrow2 = { 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow1 = { 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1 };
	std::array<int, MAXV_PER_ROW> Vrow0 = { 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1 };
	std::vector<std::array<int, MAXV_PER_ROW>> Vrow_vec = { Vrow0, Vrow1, Vrow2, Vrow3, Vrow4, Vrow5, Vrow6, Vrow7,
		Vrow8, Vrow9, Vrow10, Vrow11, Vrow12, Vrow13, Vrow14 };
	this->Vrow_vec = Vrow_vec;

	//For vertical outer walls: each vertical row, create a wall at every first and last position.
	for (int y = 0; y < MAXV_ROWS; y++)
	{
		for (int x = 0; x < MAXV_PER_ROW; x++)
		{
			gridV[y][x] = Vrow_vec[y][x];
		}
	}
}

void gridCombo::printGH() 
{
	for (int y = 0; y < MAXH_ROWS; y++)
	{
		for (int x = 0; x < MAXH_PER_ROW; x++)
		{
			std::cout << "gridH[" << y << "][" << x << "] = " << gridH[y][x] << std::endl;
		}
	}
}

void gridCombo::printGV() 
{
	for (int y = 0; y < MAXV_ROWS; y++)
	{
		for (int x = 0; x < MAXV_PER_ROW; x++)
		{
			std::cout << "gridV[" << y << "][" << x << "] = " << gridV[y][x] << std::endl;
		}
	}
}

bool pCamera::collision() 
{
	//wall_ahead & wall_behind are set based on movement.
	//All we need to do is check against the given direction.
	//If in the direction is a wall, do nothing.
	return true;
}




//ALL BELOW ARE LEGACY METHODS - STILL USEFUL FOR LEARNING

/*

//face the camera N/E/S/W direction - DISABLED!
void pCamera::absFaceCamera(Face direction)
{
	double temp = 0;
	switch (direction)
	{
	case Face::North:
		switch (this->face_iter[this->direction])
		{
		case Face::North:
			//Do nothing
			break;
		case Face::East:
			//Swap x and z
			temp = eye_x;
			eye_x = eye_z;
			eye_z = temp;

			temp = center_x;
			center_x = center_z;
			center_z = temp;
			break;
		case Face::South:
			//Inv z
			eye_z *= (-1);
			center_z *= (-1);
			break;
		case Face::West:
			//swap x and z, then inv x
			temp = eye_x;
			eye_x = eye_z;
			eye_z = temp;
			eye_x *= (-1);

			temp = center_x;
			center_x = center_z;
			center_z = temp;
			center_x *= (-1);
			break;
		}
		break;
	case Face::East:
		switch (this->face_iter[this->direction])
		{
		case Face::North:
			//swap x and z, then inv z
			temp = eye_x;
			eye_x = eye_z;
			eye_z = temp;
			eye_z *= (-1);

			temp = center_x;
			center_x = center_z;
			center_z = temp;
			center_z *= (-1);
			break;
		case Face::East:
			//Do nothing
			break;
		case Face::South:
			//Swap x and z
			temp = eye_x;
			eye_x = eye_z;
			eye_z = temp;

			temp = center_x;
			center_x = center_z;
			center_z = temp;
			break;
		case Face::West:
			//Inv x
			eye_x *= (-1);
			center_x *= (-1);
			break;
		}
		break;
	case Face::South:
		switch (this->face_iter[this->direction])
		{
		case Face::North:
			//Inv z
			eye_z *= (-1);
			center_z *= (-1);
			break;
		case Face::East:
			//swap x and z, then inv x
			temp = eye_x;
			eye_x = eye_z;
			eye_z = temp;
			eye_x *= (-1);

			temp = center_x;
			center_x = center_z;
			center_z = temp;
			center_x *= (-1);
			break;
		case Face::South:
			//Do nothing
			break;
		case Face::West:
			//swap x and z
			temp = eye_x;
			eye_x = eye_z;
			eye_z = temp;

			temp = center_x;
			center_x = center_z;
			center_z = temp;
			break;
		}
		break;
	case Face::West:
		switch (this->face_iter[this->direction])
		{
		case Face::North:
			//swap x and z
			temp = eye_x;
			eye_x = eye_z;
			eye_z = temp;

			temp = center_x;
			center_x = center_z;
			center_z = temp;
			break;
		case Face::East:
			//inv x
			eye_x *= (-1);
			center_x *= (-1);
			break;
		case Face::South:
			//swap x and z, then inv z
			temp = eye_x;
			eye_x = eye_z;
			eye_z = temp;
			eye_z *= (-1);

			temp = center_x;
			center_x = center_z;
			center_z = temp;
			center_z *= (-1);
			break;
		case Face::West:
			//Do nothing
			break;
		}
		break;
	}
	for (int i = 0; i < 8; i++) 
	{
		if (direction == this->face_iter[i]) 
		{
			this->direction = i;
			return;
		}
	}
}
*/