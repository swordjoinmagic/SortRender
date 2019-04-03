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
		清除当前缓冲区
	*/
	void Clear();

	/*
		Bresenham算法光栅化线段
	*/
	void DrawLine(
		int x1,int y1,
		int x2,int y2,
		COLORREF color);

	/*
		用扫描法光栅化三角形的方法
	*/
	void DrawTriangle(
		int x0, int y0,
		int x1, int y1,
		int x2, int y2,
		COLORREF color);

	/*
	获得 模型-世界 变换矩阵

	position: 物体的世界坐标
	rotation: 物体的旋转度数
	scale: 物体的缩放比例
	*/
	Matrix GetModelMatrix(Vector3 position, Vector3 rotation, Vector3 scale);

	/*
	获得 世界-观察 变换矩阵，即观察矩阵（以UVN相机模型的形式）

	position: 相机位置
	centerPosition: 相机观察的中心点坐标
	upDir: 相机指向上的方向（粗略即可，无需精确，因为后面还要再用一次cross获得正确的upDir）
	*/
	Matrix GetViewMatrixWithUVN(Vector3 position, Vector3 centerPosition, Vector3 upDir);

	/*
	获得透视投影矩阵
	*/
	Matrix GetProjectionMatrixWithFrustum(
		int FOV,				// 摄像机的FOV角度
		int Near, int Far,		// 摄像机距离近/远平面的距离
		int right, int left,		// 近平面中心点距离右边的距离和距离左边的距离
		int bottom, int top		// 近平面中心点距离下边的距离和距离上边的距离
	);

	/*
	根据给定的三个顶点和MVP矩阵,
	在屏幕空间上绘制一个三角形图元
	*/
	void DrawPrimitive(Vector3 v1, Vector3 v2, Vector3 v3, const Matrix & mvp);

private:
	// 后备缓冲区，当画面更新时，将此后备缓冲区的值刷新到前置缓冲区（即screenHDC）
	HDC screenHDC;
	// 窗口宽高度
	int width,height;

	/*
		扫描线填充平底三角形,
		假设有平底三角形,顶点A(x0,y0),顶点B(x1,y1),顶点C(x2,y2)

		其中线段BC是底,即线段(x1,y1)~(x2,y2)

		只需要其中两点就可以进行扫描线填充
	*/
	void DrawBottomFlatTrangle(
		int x0, int y0,
		int x1, int y1,
		int x2, int y2,
		COLORREF color);

	/*
		扫描线填充平顶三角形,
		设有平顶三角形,顶点A(x0,y0),B(x1,y1),C(x2,y2),

		其中线段AB是顶,即线段(x0,y0) ~ (x1,y1)
	*/
	void DrawTopFlatTrangle(
		int x0, int y0,
		int x1, int y1,
		int x2, int y2,
		COLORREF color);

	/*
	获得缩放矩阵
		x: 物体沿x轴缩放比例
		y: 物体沿y轴缩放比例
		z: 物体沿z轴缩放比例
	*/
	Matrix GetScaleMatrix(float x, float y, float z);

	/*
		获得平移矩阵
	*/
	Matrix GetTranslateMatrix(float x, float y, float z);

	/*
	获得沿x轴旋转 angle 度的旋转矩阵
	*/
	Matrix GetRotateMatrixAlongXAxis(int angle);

	/*
	获得沿Y轴旋转 angle 度的旋转矩阵
	*/
	Matrix GetRotateMatrixAlongYAxis(int angle);

	/*
	获得沿Z轴旋转 angle 度的旋转矩阵
	*/
	Matrix GetRotateMatrixAlongZAxis(int angle);

	/*
		获得旋转矩阵
	*/
	Matrix GetRotateMatrix(int xAngle, int yAngle, int zAngle);


	/*
	齐次除法,将一个顶点(处于投影空间内的)的x,y,z分量除以w分量
	将该顶点变换至NDC坐标空间中
	*/
	void ScreenMapping(Vector3 & vertex);

	/*
	CVV裁剪检查,返回True表示需要裁剪,false表示不需要
	*/
	bool CVVCullCheck(Vector3 vertex);

};
