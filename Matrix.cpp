#include "stdafx.h"
#include "Matrix.h"

/*
	����Ĺ��췽��
*/
Matrix::Matrix() {
	// ��ʼ������
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4;j++) {
			value[i][j] = 0;
		}
	}
}

/*
	���й�������
*/
void Matrix::InitMatrixWithRow(Vector3 row0,Vector3 row1,Vector3 row2) {

	Identity();

	value[0][0] = row0.x;
	value[0][1] = row0.y;
	value[0][2] = row0.z;

	value[1][0] = row1.x;
	value[1][1] = row1.y;
	value[1][2] = row1.z;

	value[2][0] = row2.x;
	value[2][1] = row2.y;
	value[2][2] = row2.z;
}

/*
���й�������
*/
void Matrix::InitMatrixWithCol(Vector3 col0, Vector3 col1, Vector3 col2) {

	Identity();

	value[0][0] = col0.x;
	value[1][0] = col0.y;
	value[2][0] = col0.z;

	value[0][1] = col1.x;
	value[1][1] = col1.y;
	value[2][1] = col1.z;

	value[0][2] = col2.x;
	value[1][2] = col2.y;
	value[2][2] = col2.z;
}

// �������������
Matrix::~Matrix() {

}

/*
	����ӷ�
*/
Matrix Matrix::operator+(const Matrix& right) const{
	Matrix result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.value[i][j] = this->value[i][j] + right.value[i][j];
		}
	}
	return result;
}

/*
	�������
*/
Matrix Matrix::operator-(const Matrix& right) const {
	Matrix result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.value[i][j] = this->value[i][j] - right.value[i][j];
		}
	}
	return result;
}

/*
	��������
*/
Matrix Matrix::operator*(float value) const {
	Matrix result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.value[i][j] = this->value[i][j] * value;
		}
	}
	return result;
}

/*
	����˷�
*/
Matrix Matrix::operator*(const Matrix& right) const {
	Matrix result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {

			// ��ʼ��
			result.value[i][j] = 0;

			// ��i��j�е�ֵ��
			// �൱��������i�е���������Ҿ����j�е�����	
			for (int k = 0; k < 4; k++) {
				result.value[i][j] += this->value[i][k] * right.value[k][j];
			}

		}
	}
	return result;
}

/*
	���õ�λ����
*/
void Matrix::Identity() {
	for (int i = 0, j = 0; i < 4 && j < 4; i++, j++) {
		this->value[i][j] = 1;
	}
}

/*
	����任���������ｫ������Ϊ�о���������˷���
*/
Vector3 Matrix::MultiplyVector3(const Vector3& v) const {
	Vector3 result;
	
	// �����0��*�о����0��
	result.x = 
		this->value[0][0] * v.x + 
		this->value[0][1] * v.y + 
		this->value[0][2] * v.z + 
		this->value[0][3] * v.w;

	// �����1��*�о����0��
	result.y = 
		this->value[1][0] * v.x +
		this->value[1][1] * v.y +
		this->value[1][2] * v.z +
		this->value[1][3] * v.w;

	// �����2��*�о����0��
	result.z = 
		this->value[2][0] * v.x +
		this->value[2][1] * v.y +
		this->value[2][2] * v.z +
		this->value[2][3] * v.w;

	// �����3��*�о����0��
	result.w =
		this->value[3][0] * v.x +
		this->value[3][1] * v.y +
		this->value[3][2] * v.z +
		this->value[3][3] * v.w;

	return result;
}

/*
	ת�þ���
*/
void Matrix::Transpose() {
	for (int i = 0; i < 4;i++) {
		for (int j = 0; j < i;j++) {
			std::swap(value[i][j],value[j][i]);
		}
	}
}