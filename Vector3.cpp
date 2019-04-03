#include "stdafx.h"
#include "Vector3.h"

/*
	Vector3数据类型的初始化方法
*/
Vector3::Vector3(float fx,float fy,float fz) {
	this->x = fx;
	this->y = fy;
	this->z = fz;

	// 表示该向量表示一个点
	w = 1;
}

Vector3::Vector3()
{
}

/*
	向量的标量乘法
*/
Vector3 Vector3::operator*(float value) const {
	return Vector3(x*value,y*value,z*value);
}

/*
	向量的标量乘法
*/
Vector3 Vector3::operator/(float value) const{
	return Vector3(x/value,y/value,z/value);
}

/*
	向量加法
*/
Vector3 Vector3::operator+(const Vector3& right) const {
	return Vector3(x + right.x, y + right.y, z + right.z);
}

/*
	向量减法
*/
Vector3 Vector3::operator-(const Vector3& right) const {
	return Vector3(x - right.x, y - right.y, z - right.z);
}

/*
	向量点乘
*/
float Vector3::Dot(const Vector3& left,const Vector3& right) {
	return left.x*right.x + left.y*right.y + left.z*right.z;
}

/*
	计算向量的摸长的平方
*/
float Vector3::SquareMagnitude() {
	return Dot(*this,*this);
}

/*
	计算向量的模长
*/
float Vector3::Magnitude() {
	return sqrt(SquareMagnitude());
}

/*
	归一化向量
*/
void Vector3::Normalize() {
	*this = *this / this->Magnitude();
}

/*
	向量叉乘，计算垂直于向量left和right的向量
*/
Vector3 Vector3::Cross(const Vector3& left,const Vector3& right) {
	return Vector3(
		left.y*right.z - left.z*right.y,
		left.z*right.x - left.x*right.z,
		left.x*right.y - left.y*right.x
	);
}

/*
	向量插值
*/
Vector3 Vector3::Lerp(const Vector3& left,const Vector3& right,float t) {
	return left + (right - left)*t;
}

void Vector3::Print() {

}
