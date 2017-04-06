#pragma once
#include <iostream>
#include <array>
#include <math.h>

/*
	Each sub-vector is a row ([row][column]).
	[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0,]]
*/

struct Matrix4x4 {
	double array[4][4];
	Matrix4x4() 
	{
		for (int y = 0; y < 4; y++) 
		{
			for (int x = 0; x < 4; x++) 
			{
				array[y][x] = 0.0;
			}
		}
	}
	void setAsIdentity()
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				x == y ? array[y][x] = 1.0 : array[y][x] = 0.0;
			}
		}
	}
};

//Matrix 4x4 multiplication
Matrix4x4 matrixTimes(Matrix4x4 m1, Matrix4x4 m2);

//Matrix * vector4 
std::array<double, 4> matrixTimesVector(Matrix4x4 m, std::array<double, 4> v);

Matrix4x4 defineTransformMatrix(std::array<double, 3> input);

//axis: 1-x, 2-y, 3-z
Matrix4x4 defineRotationMatrix(int axis, double theta);

//Debug read of vector result
void printVector(std::array<double, 4> v);

//Debug read of matrix result
void printMatrix(Matrix4x4 m);