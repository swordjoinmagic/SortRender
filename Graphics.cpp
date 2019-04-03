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
	清除缓冲区
*/
void Graphics::Clear() {
	BitBlt(screenHDC, 0, 0, width, height, NULL, NULL, NULL, BLACKNESS);
}

/*
用Bresenham算法画线
*/
void Graphics::DrawLine(int x1, int y1, int x2, int y2, COLORREF color) {
	int dx = x2 - x1;
	int dy = y2 - y1;

	/*
	当x2>x1时(即线段从左边连到右边),
	x的增量为正(2-1=1),否则为负（-1）
	*/
	int stepX = ((dx>0) << 1) - 1;
	/*
	当y2>y1时(即线段从下边连接到上边)
	y的增量为正,否则为负
	*/
	int stepY = ((dy>0) << 1) - 1;

	int x = x1, y = y1;

	// 误差
	int eps = 0;

	dx = abs(dx); dy = abs(dy);

	// 线段终点离起点的x坐标更远
	if (dx > dy) {
		// 要以x为+1的增量,判断当前y是y还是y+1
		for (x = x1; x != x2; x += stepX) {
			SetPixel(screenHDC, x, y, color);

			// 误差累加,这里要注意原本的误差是 dy/dx(表示x每增加1,y的增量)
			// 为了消除浮点数运算,这里两边乘个dx
			// 所以误差每次叠加dy
			eps += dy;

			// 原本的判断是 eps(即dy/dx) >= 0.5,两边同乘dx*2(为了消除浮点数)
			// 表达式变为2*dy >= dx
			// 原意还是不变的,表示如果误差超过0.5,
			// 那么y就应该+1(这里要考虑多个象限,不一定+1)了
			if ((eps << 1) >= dx) {
				y += stepY;
				eps -= dx;
			}
		}
	}
	else {
		// dy > dx,表示要以y为+1增量,x为判断x还是x+1
		for (y = y1; y != y2; y += stepY) {
			SetPixel(screenHDC, x, y, color);

			// 误差累加,原本误差为dx/dy,两边同乘dy
			eps += dx;

			if ((eps << 1) >= dy) {
				x += stepX;
				eps -= dy;
			}
		}
	}
}

/*
扫描线填充平底三角形,
假设有平底三角形,顶点A(x0,y0),顶点B(x1,y1),顶点C(x2,y2)

其中线段BC是底,即线段(x1,y1)~(x2,y2)

只需要其中两点就可以进行扫描线填充
*/
void Graphics::DrawBottomFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2, COLORREF color) {
	for (int y = y0; y <= y1; y++) {
		int xLeft = (y - y1) * (x0 - x1) / (y0 - y1) + x1;
		int xRight = (y - y2) * (x0 - x2) / (y0 - y2) + x2;
		this->DrawLine(xLeft, y, xRight, y, color);
	}
}

/*
扫描线填充平顶三角形,
设有平顶三角形,顶点A(x0,y0),B(x1,y1),C(x2,y2),

其中线段AB是顶,即线段(x0,y0) ~ (x1,y1)
*/
void Graphics::DrawTopFlatTrangle(int x0, int y0, int x1, int y1, int x2, int y2, COLORREF color) {
	for (int y = y0; y <= y2; y++) {
		// /0检查
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

	// 对传进来的顶点按y轴排序
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
		// 平顶三角形
		DrawTopFlatTrangle( x0, y0, x1, y1, x2, y2, color);
	}
	else if (y1 == y2) {
		// 平底三角形
		DrawBottomFlatTrangle( x0, y0, x1, y1, x2, y2, color);
	}
	else {
		// 普通三角形,可以划分成平底和平顶三角形
		// 假设该三角形三条边分别为AB,AC,BC
		// 其中顶点按y轴从小到大顺序排序为A/B/C
		// 拆分方法是在直线AC上找到y坐标与顶点B相等的点D
		// 然后连接BD,得到ABD/BCD两个三角形,他们分别以BD作为顶或底(对ABD来说是底,对BCD来说是顶)

		int x3 = (y1 - y0) * (x2 - x0) / (y2 - y0) + x0;
		int y3 = y1;	// 纵坐标与顶点B相同

						// 对坐标在x轴上按从小到大排序
		if (x1 > x3) {
			std::swap(x1, x3);
			std::swap(y1, y3);
		}
		// 画三角形ABD
		this->DrawBottomFlatTrangle(x0, y0, x1, y1, x3, y3, color);
		// 画三角形BCD
		this->DrawTopFlatTrangle(x1, y1, x3, y3, x2, y2, color);
	}
}


/*
	获得缩放矩阵
	x: 物体沿x轴缩放比例
	y: 物体沿y轴缩放比例
	z: 物体沿z轴缩放比例
*/
Matrix Graphics::GetScaleMatrix(float x,float y,float z) {
	Matrix matrix;

	// 将该矩阵设置为单位矩阵
	matrix.Identity();

	// 对角线设为x,y,z
	matrix.value[0][0] = x;
	matrix.value[1][1] = y;
	matrix.value[2][2] = z;

	return matrix;
}

/*
	获得平移矩阵
*/
Matrix Graphics::GetTranslateMatrix(float x,float y,float z) {
	Matrix matrix;

	// 将该矩阵设置为单位矩阵
	matrix.Identity();

	matrix.value[0][3] = x;
	matrix.value[1][3] = y;
	matrix.value[2][3] = z;

	return matrix;
}

/*
	获得沿x轴旋转 angle 度的旋转矩阵
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
获得沿Y轴旋转 angle 度的旋转矩阵
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
	获得沿Z轴旋转 angle 度的旋转矩阵
*/
Matrix Graphics::GetRotateMatrixAlongZAxis(int zAngle) {

	// 角度转弧度
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
	获得沿x、y、z轴旋转的选择矩阵
*/
Matrix Graphics::GetRotateMatrix(int xAngle, int yAngle, int zAngle) {
	Matrix zRotateMatrix = GetRotateMatrixAlongZAxis(zAngle);
	Matrix xRotateMatrix = GetRotateMatrixAlongXAxis(xAngle);
	Matrix yRotateMatrix = GetRotateMatrixAlongYAxis(yAngle);

	return zRotateMatrix * xRotateMatrix * yRotateMatrix;
}

/*
	获得 模型-世界 变换矩阵

	position: 物体的世界坐标
	rotation: 物体的旋转度数
	scale: 物体的缩放比例
*/
Matrix Graphics::GetModelMatrix(Vector3 position, Vector3 rotation, Vector3 scale) {
	Matrix scaleMatrix = GetScaleMatrix(scale.x,scale.y,scale.z);
	Matrix rotateMatrix = GetRotateMatrix(rotation.x,rotation.y,rotation.z);
	Matrix translateMatrix = GetTranslateMatrix(position.x,position.y,position.z);

	/*
		这里是将向量当做列矩阵处理，所以矩阵是右乘操作，
		变换顺序是
		
		缩放 -> 旋转 -> 平移
	*/
	return translateMatrix * rotateMatrix * scaleMatrix;
}

/*
	获得 世界-观察 变换矩阵，即观察矩阵（以UVN相机模型的形式）

	position: 相机位置
	centerPosition: 相机观察的中心点坐标
	upDir: 相机指向上的方向（粗略即可，无需精确，因为后面还要再用一次cross获得正确的upDir）
*/
Matrix Graphics::GetViewMatrixWithUVN(Vector3 position,Vector3 centerPosition,Vector3 upDir) {
	// 获得从相机出发，指向目标中心点的方向
	Vector3 forwardDir = centerPosition - position;
	// 根据forwardDir和upDir获得rightDir(垂直于forwardDir和upDir)
	Vector3 rightDir = Vector3::Cross(forwardDir,upDir);
	// 根据rightDir和forwardDir获得正确的upDir(垂直于rightDir和forwardDir)
	upDir = Vector3::Cross(forwardDir,rightDir);

	// 旋转空间的x轴
	forwardDir.Normalize();
	// 旋转空间的y轴
	upDir.Normalize();
	// 旋转空间的z轴
	rightDir.Normalize();
	// 根据得到的 相机旋转后空间 的三个标准正交基,forwardDir,rightDir,upDir,构建旋转矩阵
	// 将他们按列摆放
	Matrix matrix;
	matrix.InitMatrixWithCol(rightDir,upDir,forwardDir);

	// 根据当前摄像机位置和旋转矩阵,构建观察矩阵(先平移后旋转)
	return GetTranslateMatrix(position.x, position.y, position.z) * matrix;
}

/*
	获得透视投影矩阵
*/
Matrix Graphics::GetProjectionMatrixWithFrustum(
	int angle,				// 摄像机的FOV角度
	int Near,int Far,		// 摄像机距离近/远平面的距离
	int right,int left,		// 近平面中心点距离右边的距离和距离左边的距离
	int top,int bottom		// 近平面中心点距离下边的距离和距离上边的距离
) {

	// 角度转弧度
	float FOV = angle * 3.1415926f / 180;

	// 初始化矩阵
	Matrix matrix;

	// 近平面宽度与高度的比值
	float aspect = right / top;		// right = 1/2 Width ; top = 1/2 Height; 则 right/top = Width / Height

	// 避免除0异常
	float cot = cos(FOV/2) / sin(FOV/2);

	matrix.value[0][0] = cot / aspect;
	matrix.value[1][1] = cot;
	matrix.value[2][2] = -((Far + Near) / (Far - Near));
	matrix.value[2][3] = -((2 * Near*Far) / (Far - Near));
	matrix.value[3][2] = -1;

	return matrix;
}

/*
	屏幕映射,
	首先进行齐次除法将一个顶点(处于投影空间内的)的x,y,z分量除以w分量
	将该顶点变换至NDC坐标空间中,
	并将顶点缩放至[-1,1]的空间内(OpenGL风格的NDC设备坐标空间),
	然后进行屏幕映射,将在[-1,1]范围内的顶点映射至屏幕空间的0~Width,0~Height处
*/
void Graphics::ScreenMapping(Vector3& vertex) {
	// 齐次除法
	vertex.x /= vertex.w;
	vertex.y /= vertex.w;
	vertex.z /= vertex.w;

	// 将x/y坐标映射至[0,1]范围内
	vertex.x = 0.5*vertex.x + 0.5;
	vertex.y = 0.5*vertex.y + 0.5;

	// 将x/y坐标映射至[Width,Height]范围内
	vertex.x *= width;
	vertex.y *= height;
}

/*
	CVV裁剪检查,返回True表示需要裁剪,false表示不需要
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
	根据给定的三个顶点和MVP矩阵,
	在屏幕空间上绘制一个三角形图元
*/
void Graphics::DrawPrimitive(
	Vector3 vertex1,Vector3 vertex2,Vector3 vertex3,
	const Matrix& mvp
) {
	
	// 对三个顶点进行MVP变换,将他们变换到投影空间内
	Vector3 v1 = mvp.MultiplyVector3(vertex1);
	Vector3 v2 = mvp.MultiplyVector3(vertex2);
	Vector3 v3 = mvp.MultiplyVector3(vertex3);

	// 进行CVV剔除判断
	// (如果这个三角形图元全部顶点都在视椎体外部)
	// 那么就不对这个图元进行绘制
	if (CVVCullCheck(v1) && CVVCullCheck(v2) && CVVCullCheck(v3))
		return;

	// 将三个顶点经过齐次除法,屏幕映射操作后,得到这三个顶点在屏幕上的坐标
	ScreenMapping(v1);
	ScreenMapping(v2);
	ScreenMapping(v3);

	// 光栅化该三角形图元
	DrawTriangle(
		v1.x,v1.y,
		v2.x,v2.y,
		v3.x,v3.y,
		0xff0000
	);
}
