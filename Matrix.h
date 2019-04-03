#pragma once

#include "Vector3.h"

/*
	����ʵ�ֻ����ľ������㣬
	�����˷����������ˡ��ӷ�������
	ͬʱ���л�õ�λ����ȡ�
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
		����任���������ｫ������Ϊ�о���������˷���
	*/
	Vector3 MultiplyVector3(const Vector3& v) const;
	
	/*
		���õ�λ����
	*/
	void Identity();

	/*
		ת�þ���
	*/
	void Transpose();

	void Print();

	/*
		���������аڷų�ʼ������
	*/
	void InitMatrixWithRow(Vector3 row0, Vector3 row1, Vector3 row2);

	void InitMatrixWithCol(Vector3 col0, Vector3 col1, Vector3 col2);

};

