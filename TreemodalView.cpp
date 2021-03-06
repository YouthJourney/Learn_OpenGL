
// TreemodalView.cpp: CTreemodalView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Treemodal.h"
#endif

#include "TreemodalDoc.h"
#include "TreemodalView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTreemodalView

IMPLEMENT_DYNCREATE(CTreemodalView, CView)

BEGIN_MESSAGE_MAP(CTreemodalView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CTreemodalView 构造/析构

CTreemodalView::CTreemodalView()
{
	// TODO: 在此处添加构造代码
	m_xAngle = 0;
	m_yAngle = 0;
	m_xPos = 0;
	m_yPos = 0;
}

CTreemodalView::~CTreemodalView()
{
}

BOOL CTreemodalView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTreemodalView 绘图

void CTreemodalView::OnDraw(CDC* /*pDC*/)
{
	CTreemodalDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	// Clear out the color & depth buffers
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderSence();//绘图都放在这
				  // Tell OpenGL to flush its pipeline
	::glFinish();
	// Now Swap the buffers
	::SwapBuffers(m_pDC->GetSafeHdc());
}


// CTreemodalView 打印

BOOL CTreemodalView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTreemodalView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTreemodalView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CTreemodalView 诊断

#ifdef _DEBUG
void CTreemodalView::AssertValid() const
{
	CView::AssertValid();
}

void CTreemodalView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTreemodalDoc* CTreemodalView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTreemodalDoc)));
	return (CTreemodalDoc*)m_pDocument;
}
#endif //_DEBUG


// CTreemodalView 消息处理程序
// 初始化opengl
BOOL CTreemodalView::InitOpengl()
{
	//Get a DC for the Client Area
	m_pDC = new CClientDC(this);
	//Failure to Get DC
	if (m_pDC == NULL)
	{
		//::MessageBox("Error Obtaining DC");
		return FALSE;
	}
	//Failure to set the pixel format
	if (!SetupPixelFormat())
	{
		return FALSE;
	}
	//Create Rendering Context
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//Failure to Create Rendering Context
	if (m_hRC == 0)
	{
		// MessageBox("Error Creating RC");
		return FALSE;
	}
	//Make the RC Current
	if (::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC) == FALSE)
	{
		// MessageBox("Error making RC Current");
		return FALSE;
	}
	//Specify Black as the clear color
	::glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Specify the back of the buffer as clear depth
	::glClearDepth(1.0f);
	//Enable Depth Testing
	::glEnable(GL_DEPTH_TEST);
	return TRUE;

}


// 设置像素个数
BOOL CTreemodalView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,                // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		16,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored


	};
	int m_nPixelFormat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);
	if (m_nPixelFormat == 0)
	{
		return FALSE;
	}
	if (::SetPixelFormat(m_pDC->GetSafeHdc(), m_nPixelFormat, &pfd) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
	return 0;
}


void CTreemodalView::RenderSence()
{
	Head head;
	FILE *fp;
	//char fileName[128];

	//printf("请输入文件名：\n");
	//gets_s(fileName);//控制台里输入文件名称  
	fopen_s(&fp,"D:\\OpenGL练习\\Treemodal\\Tree.stl", "rb");

	if (fp != NULL)
	{
		fread(head.partName, 80, 1, fp);//获取部件名  
		fread(&head.faceNum, 4, 1, fp);//获取三角面片的数目  
	}

	Face *faces = (Face*)malloc(head.faceNum * sizeof(Face));//根据三角面片的数目，创建一个Face类型的数组  

															 //循环读取三角面片数据  
	for (int i = 0; i < head.faceNum; i++)
	{
		fread(&faces[i].normal, 12, 1, fp);   //读取法线数据  
		fread(&faces[i].p1, 12, 1, fp);       //读取顶点1的数据  
		fread(&faces[i].p2, 12, 1, fp);       //读取顶点2的数据  
		fread(&faces[i].p3, 12, 1, fp);       //读取顶点3的数据  
		fread(&faces[i].info, 2, 1, fp);      //读取保留项数据，这一项一般没什么用，这里选择读取是为了移动文件指针  

	}

	fclose(fp);
	//在控制台上输出STL文件的数据 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);
	glFrontFace(GL_CCW);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 200, 50, 0, 0, 70, 0, 0, 1);//大树
											 //gluLookAt(-40, 70, 50, 0, 50, 10, 0, 0, 1);//房子
											 //gluLookAt(500, 0, 50, 0, 0, 70, 0, 0, 1);//现代黑色手枪
	glTranslatef(-m_xPos, -m_yPos, 0.0f);
	glRotatef(m_xAngle, 0, 0, 1);
	glRotatef(m_yAngle, -cos(m_xAngle*c), sin(m_xAngle*c), 0);

	glBegin(GL_LINES);
	glVertex3f(-150, 0, 0);
	glVertex3f(150, 0, 0);
	glVertex3f(0, -300, 0);
	glVertex3f(0, 300, 0);
	glVertex3f(0, 0, -300);
	glVertex3f(0, 0, 300);
	glEnd();
	glBegin(GL_TRIANGLES);
	glColor3f(0.8, 0.8, 0.8);
	for (int i = 0; i < head.faceNum; i++)
	{
		//glColor3f(0.0, 0.0, 1);
		glVertex3f(faces[i].p1.x, faces[i].p1.y, faces[i].p1.z);
		//glColor3f(0.0, 1.0, 0.0);
		glVertex3f(faces[i].p2.x, faces[i].p2.y, faces[i].p2.z);
		//glColor3f(1.0, 0.0, 0.0);
		glVertex3f(faces[i].p3.x, faces[i].p3.y, faces[i].p3.z);

	}
	glEnd();
	glFlush();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear Screen And Depth Buffer
	//glLoadIdentity();                                   // Reset The Current Modelview Matrix
	//glTranslatef(-1.5f, 0.0f, -6.0f);                     // Move Left 1.5 Units And Into The Screen 6.0
	//glRotatef(30, 0.0f, 1.0f, 0.0f);                       // Rotate The Triangle On The Y axis ( NEW )
	////glTranslatef(m_xPos, m_yPos, -5.0f);
	////glRotatef(m_xAngle, 1.0f, 0.0f, 0.0f);
	////glRotatef(m_yAngle, 0.0f, 1.0f, 0.0f);
	//glBegin(GL_TRIANGLES);                              // Start Drawing A Triangle
	//glColor3f(1.0f, 0.0f, 0.0f);                      // Red
	//glVertex3f(0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Front)
	//glColor3f(0.0f, 1.0f, 0.0f);                      // Green
	//glVertex3f(-1.0f, -1.0f, 1.0f);                  // Left Of Triangle (Front)
	//glColor3f(0.0f, 0.0f, 1.0f);                      // Blue
	//glVertex3f(1.0f, -1.0f, 1.0f);                  // Right Of Triangle (Front)
	//glColor3f(1.0f, 0.0f, 0.0f);                      // Red
	//glVertex3f(0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Right)
	//glColor3f(0.0f, 0.0f, 1.0f);                      // Blue
	//glVertex3f(1.0f, -1.0f, 1.0f);                  // Left Of Triangle (Right)
	//glColor3f(0.0f, 1.0f, 0.0f);                      // Green
	//glVertex3f(1.0f, -1.0f, -1.0f);                 // Right Of Triangle (Right)
	//glColor3f(1.0f, 0.0f, 0.0f);                      // Red
	//glVertex3f(0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Back)
	//glColor3f(0.0f, 1.0f, 0.0f);                      // Green
	//glVertex3f(1.0f, -1.0f, -1.0f);                 // Left Of Triangle (Back)
	//glColor3f(0.0f, 0.0f, 1.0f);                      // Blue
	//glVertex3f(-1.0f, -1.0f, -1.0f);                 // Right Of Triangle (Back)
	//glColor3f(1.0f, 0.0f, 0.0f);                      // Red
	//glVertex3f(0.0f, 1.0f, 0.0f);                  // Top Of Triangle (Left)
	//glColor3f(0.0f, 0.0f, 1.0f);                      // Blue
	//glVertex3f(-1.0f, -1.0f, -1.0f);                  // Left Of Triangle (Left)
	//glColor3f(0.0f, 1.0f, 0.0f);                      // Green
	//glVertex3f(-1.0f, -1.0f, 1.0f);                  // Right Of Triangle (Left)
	//glEnd();                                            // Done Drawing The Pyramid

	//glLoadIdentity();                                   // Reset The Current Modelview Matrix
	//glTranslatef(1.5f, 0.0f, -7.0f);                      // Move Right 1.5 Units And Into The Screen 7.0
	//glRotatef(25, 1.0f, 1.0f, 1.0f);                   // Rotate The Quad On The X axis ( NEW )
	//glBegin(GL_QUADS);                                  // Draw A Quad
	//glColor3f(0.0f, 1.0f, 0.0f);                      // Set The Color To Green
	//glVertex3f(1.0f, 1.0f, -1.0f);                  // Top Right Of The Quad (Top)
	//glVertex3f(-1.0f, 1.0f, -1.0f);                  // Top Left Of The Quad (Top)
	//glVertex3f(-1.0f, 1.0f, 1.0f);                  // Bottom Left Of The Quad (Top)
	//glVertex3f(1.0f, 1.0f, 1.0f);                  // Bottom Right Of The Quad (Top)
	//glColor3f(1.0f, 0.5f, 0.0f);                      // Set The Color To Orange
	//glVertex3f(1.0f, -1.0f, 1.0f);                  // Top Right Of The Quad (Bottom)
	//glVertex3f(-1.0f, -1.0f, 1.0f);                  // Top Left Of The Quad (Bottom)
	//glVertex3f(-1.0f, -1.0f, -1.0f);                  // Bottom Left Of The Quad (Bottom)
	//glVertex3f(1.0f, -1.0f, -1.0f);                  // Bottom Right Of The Quad (Bottom)
	//glColor3f(1.0f, 0.0f, 0.0f);                      // Set The Color To Red
	//glVertex3f(1.0f, 1.0f, 1.0f);                  // Top Right Of The Quad (Front)
	//glVertex3f(-1.0f, 1.0f, 1.0f);                  // Top Left Of The Quad (Front)
	//glVertex3f(-1.0f, -1.0f, 1.0f);                  // Bottom Left Of The Quad (Front)
	//glVertex3f(1.0f, -1.0f, 1.0f);                  // Bottom Right Of The Quad (Front)
	//glColor3f(1.0f, 1.0f, 0.0f);                      // Set The Color To Yellow
	//glVertex3f(1.0f, -1.0f, -1.0f);                  // Top Right Of The Quad (Back)
	//glVertex3f(-1.0f, -1.0f, -1.0f);                  // Top Left Of The Quad (Back)
	//glVertex3f(-1.0f, 1.0f, -1.0f);                  // Bottom Left Of The Quad (Back)
	//glVertex3f(1.0f, 1.0f, -1.0f);                  // Bottom Right Of The Quad (Back)
	//glColor3f(0.0f, 0.0f, 1.0f);                      // Set The Color To Blue
	//glVertex3f(-1.0f, 1.0f, 1.0f);                  // Top Right Of The Quad (Left)
	//glVertex3f(-1.0f, 1.0f, -1.0f);                  // Top Left Of The Quad (Left)
	//glVertex3f(-1.0f, -1.0f, -1.0f);                  // Bottom Left Of The Quad (Left)
	//glVertex3f(-1.0f, -1.0f, 1.0f);                  // Bottom Right Of The Quad (Left)
	//glColor3f(1.0f, 0.0f, 1.0f);                      // Set The Color To Violet
	//glVertex3f(1.0f, 1.0f, -1.0f);                  // Top Right Of The Quad (Right)
	//glVertex3f(1.0f, 1.0f, 1.0f);                  // Top Left Of The Quad (Right)
	//glVertex3f(1.0f, -1.0f, 1.0f);                  // Bottom Left Of The Quad (Right)
	//glVertex3f(1.0f, -1.0f, -1.0f);                  // Bottom Right Of The Quad (Right)
	//glEnd();                                            // Done Drawing The Quad

}


int CTreemodalView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	InitOpengl();//初始化openGL绘图
	return 0;
}


void CTreemodalView::OnDestroy()
{
	CView::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	if (::wglMakeCurrent(0, 0) == FALSE)
	{
		MessageBox(_T("Could not make RC non-current"));
	}

	//Delete the rendering context
	if (::wglDeleteContext(m_hRC) == FALSE)
	{
		MessageBox(_T("Could not delete RC"));
	}
	//Delete the DC
	if (m_pDC)
	{
		delete m_pDC;
	}
	//Set it to NULL
	m_pDC = NULL;
}


void CTreemodalView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	GLdouble aspect_ratio; // width/height ratio

	if (0 >= cx || 0 >= cy)
	{
		return;
	}
	// select the full client area
	::glViewport(0, 0, cx, cy);
	windLength = cx; windHeight = cy;
	// compute the aspect ratio
	// this will keep all dimension scales equal
	aspect_ratio = (GLdouble)cx / (GLdouble)cy;
	// select the projection matrix and clear it
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	// select the viewing volume
	::gluPerspective(60.0f, aspect_ratio, .01f, 1000.0f);//画三维
														 //::gluOrtho2D(-10.0f, 10.0f, -10.0f, 10.0f);    //二维


														 // switch back to the modelview matrix and clear it
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();
}


BOOL CTreemodalView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	return CView::OnEraseBkgnd(pDC);
}


void CTreemodalView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	mousePoint = point;
	CView::OnLButtonDown(nFlags, point);
}

void CTreemodalView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	mousePoint = point;
	CView::OnRButtonDown(nFlags, point);
}

void CTreemodalView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (nFlags&MK_LBUTTON) {
		m_yAngle += (point.y - mousePoint.y)/3.6;
		m_xAngle += (point.x - mousePoint.x)/3.6 ;
		//Set the mouse point
		mousePoint = point;
		InvalidateRect(NULL, FALSE);

	}
	if (nFlags&MK_RBUTTON)
	{
		m_xPos += point.x - mousePoint.x;
		m_yPos += point.y - mousePoint.y;
		mousePoint = point;
		InvalidateRect(NULL, FALSE);
	}
	CView::OnMouseMove(nFlags, point);

}