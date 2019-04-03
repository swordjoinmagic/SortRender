#include "stdafx.h"
#include "Vector3.h"

/*
	Vector3�������͵ĳ�ʼ������
*/
Vector3::Vector3(float fx,float fy,float fz) {
	this->x = fx;
	this->y = fy;
	this->z = fz;

	// ��ʾ��������ʾһ����
	w = 1;
}

Vector3::Vector3()
{
}

/*
	�����ı����˷�
*/
Vector3 Vector3::operator*(float value) const {
	return Vector3(x*value,y*value,z*value);
}

/*
	�����ı����˷�
*/
Vector3 Vector3::operator/(float value) const{
	return Vector3(x/value,y/value,z/value);
}

/*
	�����ӷ�
*/
Vector3 Vector3::operator+(const Vector3& right) const {
	return Vector3(x + right.x, y + right.y, z + right.z);
}

/*
	��������
*/
Vector3 Vector3::operator-(const Vector3& right) const {
	return Vector3(x - right.x, y - right.y, z - right.z);
}

/*
	�������
*/
float Vector3::Dot(const Vector3& left,const Vector3& right) {
	return left.x*right.x + left.y*right.y + left.z*right.z;
}

/*
	����������������ƽ��
*/
float Vector3::SquareMagnitude() {
	return Dot(*this,*this);
}

/*
	����������ģ��
*/
float Vector3::Magnitude() {
	return sqrt(SquareMagnitude());
}

/*
	��һ������
*/
void Vector3::Normalize() {
	*this = *this / this->Magnitude();
}

/*
	������ˣ����㴹ֱ������left��right������
*/
Vector3 Vector3::Cross(const Vector3& left,const Vector3& right) {
	return Vector3(
		left.y*right.z - left.z*right.y,
		left.z*right.x - left.x*right.z,
		left.x*right.y - left.y*right.x
	);
}

/*
	������ֵ
*/
Vector3 Vector3::Lerp(const Vector3& left,const Vector3& right,float t) {
	return left + (right - left)*t;
}

void Vector3::Print() {

}
