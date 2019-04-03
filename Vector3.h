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

	// 向量模长的平方
	float SquareMagnitude();

	// 向量模长
	float Magnitude();

	// 归一化向量
	void Normalize();

	// 向量点乘
	static float Dot(const Vector3& left, const Vector3& right);

	// 向量叉乘
	static Vector3 Cross(const Vector3& left,const Vector3& right);

	// 插值运算
	inline static Vector3 Lerp(const Vector3& left,const Vector3& right,float t);

	void Print();
};

