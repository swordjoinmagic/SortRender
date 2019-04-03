#pragma once
#include <iostream>
#include <cmath>

class Vector3{
public:

	float x, y, z, w;

	Vector3(float fx, float fy, float fz);
	Vector3();

	Vector3 operator + (const Vector3& right) const;
	Vector3 operator - (const Vector3& right) const;
	Vector3 operator * (float value) const;
	Vector3 operator / (float value) const;

	// ����ģ����ƽ��
	float SquareMagnitude();

	// ����ģ��
	float Magnitude();

	// ��һ������
	void Normalize();

	// �������
	static float Dot(const Vector3& left, const Vector3& right);

	// �������
	static Vector3 Cross(const Vector3& left,const Vector3& right);

	// ��ֵ����
	inline static Vector3 Lerp(const Vector3& left,const Vector3& right,float t);

	void Print();
};

