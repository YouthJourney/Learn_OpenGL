
// TreemodalView.h: CTreemodalView 类的接口
//

#pragma once
#define PI 3.1415926
#include<GL/GL.h>
#include<gl/GLU.h>
#include<gl/glut.h>
//#include<GL/GLAUX.h>


class CTreemodalView : public CView
{
protected: // 仅从序列化创建
	CTreemodalView();
	DECLARE_DYNCREATE(CTreemodalView)

// 特性
public:
	CTreemodalDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CTreemodalView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

public:
	// 初始化opengl
	BOOL InitOpengl();
	// 设置像素个数
	BOOL SetupPixelFormat();
	void RenderSence();

	//void Mouse(int button, int state, int x, int y);
	//void onMousemove(int x, int y);
private:
	HGLRC m_hRC;
	CDC* m_pDC;
	CPoint mousePoint;
	//控制旋转和位置的变量
	GLfloat m_xAngle;//
	GLfloat m_yAngle;
	GLfloat m_xPos;
	GLfloat m_yPos;
	int windHeight;
	int windLength;

	//int mousex;
	//int mousey;
	//bool lmouswd;
	//int anglex = 0;
	//int angley = 0;

	float c = PI / 180;
	//文件头，共84字节  
	struct Head
	{
		char partName[80];//零件名称  
		int  faceNum;//面的数目  
	};

	//点，三个float类型的，大小为12字节  
	struct Point
	{
		float x;
		float y;
		float z;
	};

	//法线  
	struct Normal
	{
		float i;
		float j;
		float k;
	};

	//三角面，由一个法线，三个点，和一个两字节的保留项，一共50字节  
	struct Face
	{
		Normal normal;
		Point  p1;
		Point  p2;
		Point  p3;
		char  info[2];//保留数据，可以不用管  
	};
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // TreemodalView.cpp 中的调试版本
inline CTreemodalDoc* CTreemodalView::GetDocument() const
   { return reinterpret_cast<CTreemodalDoc*>(m_pDocument); }
#endif

