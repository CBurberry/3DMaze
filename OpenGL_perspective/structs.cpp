#include "structs.h"

bool pCamera::forward() 
{
	//If the camera is still turning, ignore movement.
	if (turning) 
	{
		return false;
	}

	//Get absolute positions for wall position grid based on camera displacement.
	int grid_pos_x = (origin_displacement[0] + 14) / 2;
	int grid_pos_y = ((-1) * (origin_displacement[2]) + 14) / 2;


	//Displace the camera coordinates and the player icon's coordinates.
	switch (this->direction) 
	{
	case 0: //N
		//If colliding, ignore movement.
		if (grids->gridH[grid_pos_y + 1][grid_pos_x] == 1)
		{
			return false;
		}
		g_eye_pos[2] -= 2;
		g_center_pos[2] -= 2;
		origin_displacement[2] -= 2;
		icon.absTranslation[2] -= 2;
		break;
	case 2: //E
		//If colliding, ignore movement.
		if (grids->gridV[grid_pos_y][grid_pos_x + 1])
		{
			return false;
		}
		g_eye_pos[0] += 2;
		g_center_pos[0] += 2;
		origin_displacement[0] += 2;
		icon.absTranslation[0] += 2;
		break;
	case 4: //S
		//If colliding, ignore movement.
		if (grids->gridH[grid_pos_y][grid_pos_x] == 1)
		{
			return false;
		}
		g_eye_pos[2] += 2;
		g_center_pos[2] += 2;
		origin_displacement[2] += 2;
		icon.absTranslation[2] += 2;
		break;
	case 6: //W
		//If colliding, ignore movement.
		if (grids->gridV[grid_pos_y][grid_pos_x])
		{
			return false;
		}
		g_eye_pos[0] -= 2;
		g_center_pos[0] -= 2;
		origin_displacement[0] -= 2;
		icon.absTranslation[0] -= 2;
		break;
	}

	//If the position of the player is the 'win' location, set the bool value.
	if (origin_displacement[0] == 0.0 && origin_displacement[2] == -14.0)
	{
		win = true;
	}
	return true;
}

bool pCamera::backward()
{
	//If the camera is still turning, ignore movement.
	if (turning)
	{
		return false;
	}

	//Get absolute positions for wall position grid based on camera displacement.
	int grid_pos_x = (origin_displacement[0] + 14) / 2;
	int grid_pos_y = ((-1) * (origin_displacement[2]) + 14) / 2;

	//Displace the camera coordinates and the player icon's coordinates.
	switch (this->direction)
	{
	case 0: //N
		//If colliding, ignore movement.
		if (grids->gridH[grid_pos_y][grid_pos_x] == 1)
		{
			return false;
		}
		g_eye_pos[2] += 2;
		g_center_pos[2] += 2;
		origin_displacement[2] += 2;
		icon.absTranslation[2] += 2;
		break;
	case 2: //E
		//If colliding, ignore movement.
		if (grids->gridV[grid_pos_y][grid_pos_x])
		{
			return false;
		}
		g_eye_pos[0] -= 2;
		g_center_pos[0] -= 2;
		origin_displacement[0] -= 2;
		icon.absTranslation[0] -= 2;
		break;
	case 4: //S
		//If colliding, ignore movement.
		if (grids->gridH[grid_pos_y + 1][grid_pos_x] == 1)
		{
			return false;
		}
		g_eye_pos[2] -= 2;
		g_center_pos[2] -= 2;
		origin_displacement[2] -= 2;
		icon.absTranslation[2] -= 2;
		break;
	case 6: //W
		//If colliding, ignore movement.
		if (grids->gridV[grid_pos_y][grid_pos_x + 1])
		{
			return false;
		}
		g_eye_pos[0] += 2;
		g_center_pos[0] += 2;
		origin_displacement[0] += 2;
		icon.absTranslation[0] += 2;
		break;
	}

	//If the position of the player is the 'win' location, set the bool value.
	if (origin_displacement[0] == 0.0 && origin_displacement[2] == -14.0)
	{
		win = true;
	}

	return true;
}

void pCamera::rotate(double theta) 
{
	int i = 0;

	//Rotation matrix for theta about Y-axis.
	Matrix4x4 rotateMatrix_y = defineRotationMatrix(2, theta);

	//Multiplication of perspective camera position vectors by rotation matrix. Using 'local' values.
	std::array<double, 4> rotatedLocalEye = matrixTimesVector(rotateMatrix_y, l_eye_pos);
	std::array<double, 4> rotatedLocalCenter = matrixTimesVector(rotateMatrix_y, l_center_pos);

	//To get the new global camera position we add the absolute displacement from start to the rotated values.
	g_eye_pos[0] = rotatedLocalEye[0] + origin_displacement[0];
	g_eye_pos[2] = rotatedLocalEye[2] + origin_displacement[2];
	g_center_pos[0] = rotatedLocalCenter[0] + origin_displacement[0];
	g_center_pos[2] = rotatedLocalCenter[2] + origin_displacement[2];
	if (theta != 45.0 && theta != -45.0 && theta != 90.0 
		&& theta != -90.0 && theta != 0.0) 
	{
		exit(1);
	}

	//Traunicate all double-type zeroes to remove negatives.
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

	//Update facing direction. (i value is just to allow shorthand if-else.)
	theta == 45.0 ? this->direction++ : theta == 90.0 ? this->direction += 2 : i = 0;
	theta == -45.0 ? this->direction-- : theta == -90.0 ? this->direction -= 2 : i = 0;
	this->direction == -1 ? this->direction = 7 : i = 1;
	this->direction == -2 ? this->direction = 6 : i = 1;
	this->direction == 8 ? this->direction = 0 : i = 1;
	this->direction == 9 ? this->direction = 1 : i = 1;

	//Update player pointer angle.
	icon.localRotation[1] += theta;
}

void gridCombo::intialiseGH() 
{
	//Hard-coded values of Horizontal wall positions 
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
	//Hard-coded values of Vertical wall positions 
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