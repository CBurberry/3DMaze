#define _USE_MATH_DEFINES

#include "mymath.h"

//Matrix 4x4 multiplication
Matrix4x4 matrixTimes(Matrix4x4 m1, Matrix4x4 m2)
{
	int acc = 0; //Accumulator
	Matrix4x4 result;
	//per row: row components * column components, add products.

	//Each row of m1
	for (int y = 0; y < 4; y++)
	{
		//Each column of m2
		for (int j = 0; j < 4; j++)
		{
			//Each column of m1
			for (int x = 0; x < 4; x++)
			{
				acc += m1.array[y][x] * m2.array[x][j];
			}
			result.array[y][j] = acc;
			acc = 0;
		}
	}
	return result;
}

//Matrix * vector4 
std::array<double, 4> matrixTimesVector(Matrix4x4 m, std::array<double, 4> v)
{
	std::array<double, 4> result = { 0, 0, 0, 0 };

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			result[y] += m.array[y][x] * v[x];
		}
	}
	return result;
}

Matrix4x4 defineTransformMatrix(std::array<double, 3> input)
{
	Matrix4x4 result;
	result.setAsIdentity();
	result.array[0][3] = input[0];
	result.array[1][3] = input[1];
	result.array[2][3] = input[2];
	return result;
}

//axis: 1-x, 2-y, 3-z
Matrix4x4 defineRotationMatrix(int axis, double degrees)
{
	if (axis == 1 || axis == 2 || axis == 3)
	{
		//continue
	}
	else
	{
		exit(1);
	}
	Matrix4x4 result;
	result.setAsIdentity();
	double theta = degrees * M_PI / 180;
	switch (axis)
	{
	case 1:
		result.array[1][1] = cos(theta);
		result.array[1][2] = (-1) * sin(theta);
		result.array[2][1] = sin(theta);
		result.array[2][2] = cos(theta);
		break;
	case 2:
		result.array[0][0] = cos(theta);
		result.array[0][2] = (-1) * sin(theta);
		result.array[2][0] = sin(theta);
		result.array[2][2] = cos(theta);
		break;
	case 3:
		result.array[0][0] = cos(theta);
		result.array[0][1] = (-1) * sin(theta);
		result.array[1][0] = sin(theta);
		result.array[1][1] = cos(theta);
		break;
	}

	return result;
}



//Debug read of vector result
void printVector(std::array<double, 4> v)
{
	std::cout << "{ " << v[0] << ", "
		<< v[1] << ", "
		<< v[2] << ", "
		<< v[3] << "}\n";
}


//Debug read of matrix result
void printMatrix(Matrix4x4 m)
{
	system("cls");
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			std::cout << m.array[y][x] << ", ";
		}
		std::cout << std::endl;
	}
}
