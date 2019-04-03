#include "stdafx.h"
#include "Graphics.h"

Graphics::Graphics(HDC hdc, int width,int height) {
	this->screenHDC = hdc;
	this->width = width;
	this->height = height;
}

Graphics::Graphics() {

}

Graphics::~Graphics() {

}

/*
	���������
*/
void Graphics::Clear() {
	BitBlt(screenHDC, 0, 0, width, height, NULL, NULL, NULL, BLACKNESS);
}

/*
��Bresenham�㷨����
*/
void Graphics::DrawLine(int x1, int y1, int x2, int y2, COLORREF color) {
	int dx = x2 - x1;
	int dy = y2 - y1;

	/*
	��x2>x1ʱ(���߶δ���������ұ�),
	x������Ϊ��(2-1=1),����Ϊ����-1��
	*/
	int stepX = ((dx>0) << 1) - 1;
	/*
	��y2>y1ʱ(���߶δ��±����ӵ��ϱ�)
	y������Ϊ��,����Ϊ��
	*/
	int stepY = ((dy>0) << 1) - 1;

	int x = x1, y = y1;

	// ���
	int eps = 0;

	dx = abs(dx); dy = abs(dy);

	// �߶��յ�������x�����Զ
	if (dx > dy) {
		// Ҫ��xΪ+1������,�жϵ�ǰy��y����y+1
		for (x = x1; x != x2; x += stepX) {
			SetPixel(screenHDC, x, y, color);

			// ����ۼ�,����Ҫע��ԭ��������� dy/dx(��ʾxÿ����1,y������)
			// Ϊ����������������,�������߳˸�dx
			// �������ÿ�ε���dy
			eps += dy;

			// ԭ�����ж��� eps(��dy/dx) >= 0.5,����ͬ��dx*2(Ϊ������������)
			// ���ʽ��Ϊ2*dy >= dx
			// ԭ�⻹�ǲ����,��ʾ�������0.5,
			// ��ôy��Ӧ��+1(����Ҫ���Ƕ������,��һ��+1)��
			if ((eps << 1) >= dx) {
				y += stepY;
				eps -= dx;
			}
		}
	}
	else {
		// dy > dx,��ʾҪ��yΪ+1����,xΪ�ж�x����x+1
		for (y = y1; y != y2; y += stepY) {
			SetPixel(screenHDC, x, y, color);

			// ����ۼ�,ԭ�����Ϊdx/dy,����ͬ��dy
			eps += dx;

			if ((eps << 1) >= dy) {
				x += stepX;
				eps -= dy;
			}
		}
	}
}

/*
ɨ�������ƽ��������,
������ƽ��������,����A(x0,y0),����B(x1,y1),����C(x2,y2)

�����߶�BC�ǵ�,���߶�(x1,y1)~(x2,y2)

ֻ��Ҫ��������Ϳ��Խ���ɨ�������
*/
void Graphics::DrawBottomFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2, COLORREF color) {
	for (int y = y0; y <= y1; y++) {
		int xLeft = (y - y1) * (x0 - x1) / (y0 - y1) + x1;
		int xRight = (y - y2) * (x0 - x2) / (y0 - y2) + x2;
		this->DrawLine(xLeft, y, xRight, y, color);
	}
}

/*
ɨ�������ƽ��������,
����ƽ��������,����A(x0,y0),B(x1,y1),C(x2,y2),

�����߶�AB�Ƕ�,���߶�(x0,y0) ~ (x1,y1)
*/
void Graphics::DrawTopFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2, COLORREF color) {
	for (int y = y0; y <= y2; y++) {
		// /0���
		if (y2 - y0 == 0 || y2 - y1 == 0) continue;
		int xLeft = (y - y0) * (x2 - x0) / (y2 - y0) + x0;
		int xRight = (y - y1) * (x2 - x1) / (y2 - y1) + x1;
		this->DrawLine(xLeft, y, xRight, y, color);
	}
}

void Graphics::DrawTriangle(
	int x0, int y0,
	int x1, int y1,
	int x2, int y2,
	COLORREF color) {

	// �Դ������Ķ��㰴y������
	if (y1 < y0) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	if (y2 < y0) {
		std::swap(x0, x2);
		std::swap(y0, y2);
	}
	if (y2 < y1) {
		std::swap(y1, y2);
		std::swap(x1, x2);
	}

	if (y0 == y1) {
		// ƽ��������
		DrawTopFlatTrangle( x0, y0, x1, y1, x2, y2, color);
	}
	else if (y1 == y2) {
		// ƽ��������
		DrawBottomFlatTrangle( x0, y0, x1, y1, x2, y2, color);
	}
	else {
		// ��ͨ������,���Ի��ֳ�ƽ�׺�ƽ��������
		// ����������������߷ֱ�ΪAB,AC,BC
		// ���ж��㰴y���С����˳������ΪA/B/C
		// ��ַ�������ֱ��AC���ҵ�y�����붥��B��ȵĵ�D
		// Ȼ������BD,�õ�ABD/BCD����������,���Ƿֱ���BD��Ϊ�����(��ABD��˵�ǵ�,��BCD��˵�Ƕ�)

		int x3 = (y1 - y0) * (x2 - x0) / (y2 - y0) + x0;
		int y3 = y1;	// �������붥��B��ͬ

						// ��������x���ϰ���С��������
		if (x1 > x3) {
			std::swap(x1, x3);
			std::swap(y1, y3);
		}
		// ��������ABD
		this->DrawBottomFlatTrangle(x0, y0, x1, y1, x3, y3, color);
		// ��������BCD
		this->DrawTopFlatTrangle(x1, y1, x3, y3, x2, y2, color);
	}
}


/*
	������ž���
	x: ������x�����ű���
	y: ������y�����ű���
	z: ������z�����ű���
*/
Matrix Graphics::GetScaleMatrix(float x,float y,float z) {
	Matrix matrix;

	// ���þ�������Ϊ��λ����
	matrix.Identity();

	// �Խ�����Ϊx,y,z
	matrix.value[0][0] = x;
	matrix.value[1][1] = y;
	matrix.value[2][2] = z;

	return matrix;
}

/*
	���ƽ�ƾ���
*/
Matrix Graphics::GetTranslateMatrix(float x,float y,float z) {
	Matrix matrix;

	// ���þ�������Ϊ��λ����
	matrix.Identity();

	matrix.value[0][3] = x;
	matrix.value[1][3] = y;
	matrix.value[2][3] = z;

	return matrix;
}

/*
	�����x����ת angle �ȵ���ת����
*/
Matrix Graphics::GetRotateMatrixAlongXAxis(int xAngle) {

	float angle = xAngle * 3.1415926f / 180;

	Matrix matrix;

	matrix.Identity();

	matrix.value[1][1] = cos(angle);
	matrix.value[1][2] = -sin(angle);
	matrix.value[2][1] = sin(angle);
	matrix.value[2][2] = cos(angle);

	return matrix;
}

/*
�����Y����ת angle �ȵ���ת����
*/
Matrix Graphics::GetRotateMatrixAlongYAxis(int yAngle) {

	float angle = yAngle * 3.1415926f / 180;

	Matrix matrix;

	matrix.Identity();

	matrix.value[0][0] = cos(angle);
	matrix.value[0][2] = sin(angle);
	matrix.value[2][0] = -sin(angle);
	matrix.value[2][2] = cos(angle);

	return matrix;
}


/*
	�����Z����ת angle �ȵ���ת����
*/
Matrix Graphics::GetRotateMatrixAlongZAxis(int zAngle) {

	// �Ƕ�ת����
	float angle = zAngle * 3.1415926f / 180;

	Matrix matrix;

	matrix.Identity();

	matrix.value[0][0] = cos(angle);
	matrix.value[0][1] = -sin(angle);
	matrix.value[1][0] = sin(angle);
	matrix.value[1][1] = cos(angle);

	return matrix;
}

/*
	�����x��y��z����ת��ѡ�����
*/
Matrix Graphics::GetRotateMatrix(int xAngle, int yAngle, int zAngle) {
	Matrix zRotateMatrix = GetRotateMatrixAlongZAxis(zAngle);
	Matrix xRotateMatrix = GetRotateMatrixAlongXAxis(xAngle);
	Matrix yRotateMatrix = GetRotateMatrixAlongYAxis(yAngle);

	return zRotateMatrix * xRotateMatrix * yRotateMatrix;
}

/*
	��� ģ��-���� �任����

	position: �������������
	rotation: �������ת����
	scale: ��������ű���
*/
Matrix Graphics::GetModelMatrix(Vector3 position, Vector3 rotation, Vector3 scale) {
	Matrix scaleMatrix = GetScaleMatrix(scale.x,scale.y,scale.z);
	Matrix rotateMatrix = GetRotateMatrix(rotation.x,rotation.y,rotation.z);
	Matrix translateMatrix = GetTranslateMatrix(position.x,position.y,position.z);

	/*
		�����ǽ����������о��������Ծ������ҳ˲�����
		�任˳����
		
		���� -> ��ת -> ƽ��
	*/
	return translateMatrix * rotateMatrix * scaleMatrix;
}

/*
	��� ����-�۲� �任���󣬼��۲������UVN���ģ�͵���ʽ��

	position: ���λ��
	centerPosition: ����۲�����ĵ�����
	upDir: ���ָ���ϵķ��򣨴��Լ��ɣ����辫ȷ����Ϊ���滹Ҫ����һ��cross�����ȷ��upDir��
*/
Matrix Graphics::GetViewMatrixWithUVN(Vector3 position,Vector3 centerPosition,Vector3 upDir) {
	// ��ô����������ָ��Ŀ�����ĵ�ķ���
	Vector3 forwardDir = centerPosition - position;
	// ����forwardDir��upDir���rightDir(��ֱ��forwardDir��upDir)
	Vector3 rightDir = Vector3::Cross(forwardDir,upDir);
	// ����rightDir��forwardDir�����ȷ��upDir(��ֱ��rightDir��forwardDir)
	upDir = Vector3::Cross(forwardDir,rightDir);

	// ��ת�ռ��x��
	forwardDir.Normalize();
	// ��ת�ռ��y��
	upDir.Normalize();
	// ��ת�ռ��z��
	rightDir.Normalize();
	// ���ݵõ��� �����ת��ռ� ��������׼������,forwardDir,rightDir,upDir,������ת����
	// �����ǰ��аڷ�
	Matrix matrix;
	matrix.InitMatrixWithCol(rightDir,upDir,forwardDir);

	// ���ݵ�ǰ�����λ�ú���ת����,�����۲����(��ƽ�ƺ���ת)
	return GetTranslateMatrix(position.x, position.y, position.z) * matrix;
}

/*
	���͸��ͶӰ����
*/
Matrix Graphics::GetProjectionMatrixWithFrustum(
	int angle,				// �������FOV�Ƕ�
	int Near,int Far,		// ����������/Զƽ��ľ���
	int right,int left,		// ��ƽ�����ĵ�����ұߵľ���;�����ߵľ���
	int top,int bottom		// ��ƽ�����ĵ�����±ߵľ���;����ϱߵľ���
) {

	// �Ƕ�ת����
	float FOV = angle * 3.1415926f / 180;

	// ��ʼ������
	Matrix matrix;

	// ��ƽ������߶ȵı�ֵ
	float aspect = right / top;		// right = 1/2 Width ; top = 1/2 Height; �� right/top = Width / Height

	// �����0�쳣
	float cot = cos(FOV/2) / sin(FOV/2);

	matrix.value[0][0] = cot / aspect;
	matrix.value[1][1] = cot;
	matrix.value[2][2] = -((Far + Near) / (Far - Near));
	matrix.value[2][3] = -((2 * Near*Far) / (Far - Near));
	matrix.value[3][2] = -1;

	return matrix;
}

/*
	��Ļӳ��,
	���Ƚ�����γ�����һ������(����ͶӰ�ռ��ڵ�)��x,y,z��������w����
	���ö���任��NDC����ռ���,
	��������������[-1,1]�Ŀռ���(OpenGL����NDC�豸����ռ�),
	Ȼ�������Ļӳ��,����[-1,1]��Χ�ڵĶ���ӳ������Ļ�ռ��0~Width,0~Height��
*/
void Graphics::ScreenMapping(Vector3& vertex) {
	// ��γ���
	vertex.x /= vertex.w;
	vertex.y /= vertex.w;
	vertex.z /= vertex.w;

	// ��x/y����ӳ����[0,1]��Χ��
	vertex.x = 0.5*vertex.x + 0.5;
	vertex.y = 0.5*vertex.y + 0.5;

	// ��x/y����ӳ����[Width,Height]��Χ��
	vertex.x *= width;
	vertex.y *= height;
}

/*
	CVV�ü����,����True��ʾ��Ҫ�ü�,false��ʾ����Ҫ
*/
bool Graphics::CVVCullCheck(Vector3 vertex) {
	float w = vertex.w;
	if (vertex.x < -w || vertex.x>w) 
		return true;
	if (vertex.y<-w || vertex.y>w)
		return true;
	if (vertex.z<-w || vertex.z>w)
		return true;
	return false;
}

/*
	���ݸ��������������MVP����,
	����Ļ�ռ��ϻ���һ��������ͼԪ
*/
void Graphics::DrawPrimitive(
	Vector3 vertex1,Vector3 vertex2,Vector3 vertex3,
	const Matrix& mvp
) {
	
	// �������������MVP�任,�����Ǳ任��ͶӰ�ռ���
	Vector3 v1 = mvp.MultiplyVector3(vertex1);
	Vector3 v2 = mvp.MultiplyVector3(vertex2);
	Vector3 v3 = mvp.MultiplyVector3(vertex3);

	// ����CVV�޳��ж�
	// (������������ͼԪȫ�����㶼����׵���ⲿ)
	// ��ô�Ͳ������ͼԪ���л���
	if (CVVCullCheck(v1) && CVVCullCheck(v2) && CVVCullCheck(v3))
		return;

	// ���������㾭����γ���,��Ļӳ�������,�õ���������������Ļ�ϵ�����
	ScreenMapping(v1);
	ScreenMapping(v2);
	ScreenMapping(v3);

	// ��դ����������ͼԪ
	DrawTriangle(
		v1.x,v1.y,
		v2.x,v2.y,
		v3.x,v3.y,
		0xff0000
	);
}
