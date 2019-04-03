#pragma once

#include "Vector3.h"

/*
	用于实现基本的矩阵运算，
	如矩阵乘法、矩阵数乘、加法减法等
	同时还有获得单位矩阵等。
*/
class Matrix{

public:
	Matrix();
	~Matrix();

	float value[4][4];

	Matrix operator + (const Matrix& right) const;
	Matrix operator - (const Matrix& right) const;
	Matrix operator * (const Matrix& right) const;
	Matrix operator * (float k) const;

	/*
		矩阵变换向量（这里将向量作为列矩阵参与矩阵乘法）
	*/
	Vector3 MultiplyVector3(const Vector3& v) const;
	
	/*
		设置单位矩阵
	*/
	void Identity();

	/*
		转置矩阵
	*/
	void Transpose();

	void Print();

	/*
		将向量按行摆放初始化矩阵
	*/
	void InitMatrixWithRow(Vector3 row0, Vector3 row1, Vector3 row2);

	void InitMatrixWithCol(Vector3 col0, Vector3 col1, Vector3 col2);

};

