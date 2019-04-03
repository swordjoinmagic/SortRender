#pragma once
#include "stdafx.h"
#include "Matrix.h"
#include "Vector3.h"

class Graphics{
public:
	Graphics(HDC hdc, int width, int height);
	Graphics();
	~Graphics();

	/*
		�����ǰ������
	*/
	void Clear();

	/*
		Bresenham�㷨��դ���߶�
	*/
	void DrawLine(
		int x1,int y1,
		int x2,int y2,
		COLORREF color);

	/*
		��ɨ�跨��դ�������εķ���
	*/
	void DrawTriangle(
		int x0, int y0,
		int x1, int y1,
		int x2, int y2,
		COLORREF color);

	/*
	��� ģ��-���� �任����

	position: �������������
	rotation: �������ת����
	scale: ��������ű���
	*/
	Matrix GetModelMatrix(Vector3 position, Vector3 rotation, Vector3 scale);

	/*
	��� ����-�۲� �任���󣬼��۲������UVN���ģ�͵���ʽ��

	position: ���λ��
	centerPosition: ����۲�����ĵ�����
	upDir: ���ָ���ϵķ��򣨴��Լ��ɣ����辫ȷ����Ϊ���滹Ҫ����һ��cross�����ȷ��upDir��
	*/
	Matrix GetViewMatrixWithUVN(Vector3 position, Vector3 centerPosition, Vector3 upDir);

	/*
	���͸��ͶӰ����
	*/
	Matrix GetProjectionMatrixWithFrustum(
		int FOV,				// �������FOV�Ƕ�
		int Near, int Far,		// ����������/Զƽ��ľ���
		int right, int left,		// ��ƽ�����ĵ�����ұߵľ���;�����ߵľ���
		int bottom, int top		// ��ƽ�����ĵ�����±ߵľ���;����ϱߵľ���
	);

	/*
	���ݸ��������������MVP����,
	����Ļ�ռ��ϻ���һ��������ͼԪ
	*/
	void DrawPrimitive(Vector3 v1, Vector3 v2, Vector3 v3, const Matrix & mvp);

private:
	// �󱸻����������������ʱ�����˺󱸻�������ֵˢ�µ�ǰ�û���������screenHDC��
	HDC screenHDC;
	// ���ڿ�߶�
	int width,height;

	/*
		ɨ�������ƽ��������,
		������ƽ��������,����A(x0,y0),����B(x1,y1),����C(x2,y2)

		�����߶�BC�ǵ�,���߶�(x1,y1)~(x2,y2)

		ֻ��Ҫ��������Ϳ��Խ���ɨ�������
	*/
	void DrawBottomFlatTrangle(
		int x0, int y0,
		int x1, int y1,
		int x2, int y2,
		COLORREF color);

	/*
		ɨ�������ƽ��������,
		����ƽ��������,����A(x0,y0),B(x1,y1),C(x2,y2),

		�����߶�AB�Ƕ�,���߶�(x0,y0) ~ (x1,y1)
	*/
	void DrawTopFlatTrangle(
		int x0, int y0,
		int x1, int y1,
		int x2, int y2,
		COLORREF color);

	/*
	������ž���
		x: ������x�����ű���
		y: ������y�����ű���
		z: ������z�����ű���
	*/
	Matrix GetScaleMatrix(float x, float y, float z);

	/*
		���ƽ�ƾ���
	*/
	Matrix GetTranslateMatrix(float x, float y, float z);

	/*
	�����x����ת angle �ȵ���ת����
	*/
	Matrix GetRotateMatrixAlongXAxis(int angle);

	/*
	�����Y����ת angle �ȵ���ת����
	*/
	Matrix GetRotateMatrixAlongYAxis(int angle);

	/*
	�����Z����ת angle �ȵ���ת����
	*/
	Matrix GetRotateMatrixAlongZAxis(int angle);

	/*
		�����ת����
	*/
	Matrix GetRotateMatrix(int xAngle, int yAngle, int zAngle);


	/*
	��γ���,��һ������(����ͶӰ�ռ��ڵ�)��x,y,z��������w����
	���ö���任��NDC����ռ���
	*/
	void ScreenMapping(Vector3 & vertex);

	/*
	CVV�ü����,����True��ʾ��Ҫ�ü�,false��ʾ����Ҫ
	*/
	bool CVVCullCheck(Vector3 vertex);

};
