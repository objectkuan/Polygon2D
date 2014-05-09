// Artist2DDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Artist2D.h"
#include "Artist2DDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CArtist2DDlg 对话框



CArtist2DDlg::CArtist2DDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArtist2DDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CArtist2DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CArtist2DDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOVE()
	ON_WM_CLOSE()
	ON_EN_CHANGE(3000, &CArtist2DDlg::OnEnChangeR)
	ON_EN_CHANGE(3001, &CArtist2DDlg::OnEnChangeG)
	ON_EN_CHANGE(3002, &CArtist2DDlg::OnEnChangeB)
	ON_NOTIFY(NM_CUSTOMDRAW, 2000, &CArtist2DDlg::OnNMCustomdrawR)
	ON_NOTIFY(NM_CUSTOMDRAW, 2001, &CArtist2DDlg::OnNMCustomdrawG)
	ON_NOTIFY(NM_CUSTOMDRAW, 2002, &CArtist2DDlg::OnNMCustomdrawB)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDBLCLK()
	ON_CLBN_CHKCHANGE(4000, &CArtist2DDlg::OnVisibleChanged)
	ON_LBN_SELCHANGE(4000, &CArtist2DDlg::OnSelPolygonChanged)
	ON_BN_CLICKED(5000, &CArtist2DDlg::PerformLayerUp)
	ON_BN_CLICKED(5001, &CArtist2DDlg::PerformLayerDown)
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()


// CArtist2DDlg 消息处理程序

BOOL CArtist2DDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	// 工具栏相关
	ToolBoxItemUp = new ToolBoxItemIDType[8];
	ToolBoxItemUp[0] = IDB_BITMAP2;
	ToolBoxItemUp[1] = IDB_BITMAP4;
	ToolBoxItemUp[2] = IDB_BITMAP6;
	ToolBoxItemUp[3] = IDB_BITMAP8;
	ToolBoxItemUp[4] = IDB_BITMAP10;
	ToolBoxItemUp[5] = IDB_BITMAP12;
	ToolBoxItemUp[6] = IDB_BITMAP14;
	ToolBoxItemUp[7] = IDB_BITMAP16;

	ToolBoxItemDown = new ToolBoxItemIDType[8];
	ToolBoxItemDown[0] = IDB_BITMAP1;
	ToolBoxItemDown[1] = IDB_BITMAP3;
	ToolBoxItemDown[2] = IDB_BITMAP5;
	ToolBoxItemDown[3] = IDB_BITMAP7;
	ToolBoxItemDown[4] = IDB_BITMAP9;
	ToolBoxItemDown[5] = IDB_BITMAP11;
	ToolBoxItemDown[6] = IDB_BITMAP13;
	ToolBoxItemDown[7] = IDB_BITMAP15;

	curTool = 0;
	ToolItemWidth = 21;
	ToolBoxX1 = 20;
	ToolBoxY1 = 40;
	ToolBoxX2 = ToolBoxX1 + 2 * ToolItemWidth;
	ToolBoxY2 = ToolBoxY1 +  4 * ToolItemWidth;

	// 画布
	CanvasWidth = 700;
	CanvasHeight = 500;
	CanvasX1 = 100;
	CanvasY1 = 30;
	CanvasX2 = CanvasX1 + CanvasWidth;
	CanvasY2 = CanvasY1 + CanvasHeight;
	
	// 颜色
	curColor = -1;
	LineColor = RGB(0, 0, 0);
	FillColor = RGB(255, 255, 255);
	ColorButtonWidth = 45;
	ColorButtonHeight = 24;
	ColorX1 = ToolBoxX1;
	ColorY1 = ToolBoxY2 + 50;
	ColorX2 = ColorX1 + ColorButtonWidth;
	ColorY2 = ColorY1 + ColorButtonHeight * 2;
	ColorSelectorPanelX1 = CanvasX1 + 10;
	ColorSelectorPanelY1 = ColorY1;
	ColorSelectorPanelX2 = ColorSelectorPanelX1 + 200;
	ColorSelectorPanelY2 = ColorSelectorPanelY1 + 100;

	// 多边形列表
	PolygonListHeight = 200;
	PolygonListX1 = ToolBoxX1 - 10;
	PolygonListY1 = ColorY2 + 20;
	PolygonListX2 = ToolBoxX2 + 15;
	PolygonListY2 = PolygonListY1 + PolygonListHeight;
	PolygonID = 0;

	Statics = new CStatic[3];
	Sliders = new CSliderCtrl[3];
	Edits = new CEdit[3];
	PolygonList = new CCheckListBox[1];
	LayerUp = new CButton[1];
	LayerDown = new CButton[1];
	{
		DWORD dwStyle = WS_CHILD | WS_VISIBLE; 
		RECT rt;

		rt.left = ColorSelectorPanelX1 + 5;
		rt.top = ColorSelectorPanelY1 + 10;
		rt.right = rt.left + 15;
		rt.bottom = rt.top + 22;
		Statics[0].Create("R:", dwStyle, rt, this, 1000);
		Statics[0].ShowWindow(FALSE);

		rt.left = ColorSelectorPanelX1 + 20;
		rt.right = rt.left + 130;
		Sliders[0].Create(dwStyle, rt, this, 2000);
		Sliders[0].SetRange(0, 255);
		Sliders[0].ShowWindow(FALSE);

		dwStyle =  WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP;
		rt.left = ColorSelectorPanelX1 + 150;
		rt.right = rt.left + 40;
		Edits[0].CreateEx(WS_EX_CLIENTEDGE, "EDIT", NULL, dwStyle, rt, this, 3000);
		Edits[0].ShowWindow(FALSE);
	}
	{
		DWORD dwStyle = WS_CHILD | WS_VISIBLE; 
		RECT rt;

		rt.left = ColorSelectorPanelX1 + 5;
		rt.right = rt.left + 15;
		rt.top = ColorSelectorPanelY1 + 10 + 30 * 1; 
		rt.bottom = rt.top + 22; 
		Statics[1].Create("G:", dwStyle, rt, this, 1001);
		Statics[1].ShowWindow(FALSE);

		rt.left = ColorSelectorPanelX1 + 20; 
		rt.right = rt.left + 130;
		Sliders[1].Create(dwStyle, rt, this, 2001);
		Sliders[1].SetRange(0, 255);
		Sliders[1].ShowWindow(FALSE);

		dwStyle =  WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP;
		rt.left = ColorSelectorPanelX1 + 150;
		rt.right = rt.left + 40;
		Edits[1].CreateEx(WS_EX_CLIENTEDGE, "EDIT", NULL, dwStyle, rt, this, 3001);
		Edits[1].ShowWindow(FALSE);
	}
	{
		DWORD dwStyle = WS_CHILD | WS_VISIBLE; 
		RECT rt;

		rt.left = ColorSelectorPanelX1 + 5;
		rt.right = rt.left + 15;
		rt.top = ColorSelectorPanelY1 + 10 + 30 * 2; 
		rt.bottom = rt.top + 22; 
		Statics[2].Create("B:", dwStyle, rt, this, 1002);
		Statics[2].ShowWindow(FALSE);

		rt.left = ColorSelectorPanelX1 + 20; 
		rt.right = rt.left + 130;
		Sliders[2].Create(dwStyle, rt, this, 2002);
		Sliders[2].SetRange(0, 255);
		Sliders[2].ShowWindow(FALSE);

		dwStyle =  WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP;
		rt.left = ColorSelectorPanelX1 + 150;
		rt.right = rt.left + 40;
		Edits[2].CreateEx(WS_EX_CLIENTEDGE, "EDIT", NULL, dwStyle, rt, this, 3002);
		Edits[2].ShowWindow(FALSE);
	}
	{
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_HASSTRINGS | LBS_OWNERDRAWFIXED | LBS_NOTIFY ; 
		RECT rt;

		rt.left = PolygonListX1;
		rt.right = PolygonListX2;
		rt.top = PolygonListY1;
		rt.bottom = PolygonListY2;

		PolygonList[0].CreateEx(WS_EX_CLIENTEDGE, "ListBox", NULL, dwStyle, rt, this, 4000);
		PolygonList[0].ShowWindow(TRUE);
	}
	{
		DWORD dwStyle =WS_CHILD | WS_VISIBLE; 
		RECT rt;

		rt.left = PolygonListX1;
		rt.right =rt.left + 20;
		rt.top = PolygonListY2 + 10; 
		rt.bottom = rt.top + 20; 
		
		LayerUp[0].Create("+", dwStyle, rt, this, 5000);
		LayerUp[0].ShowWindow(TRUE);
	}
	{
		DWORD dwStyle =WS_CHILD | WS_VISIBLE; 
		RECT rt;

		rt.left = PolygonListX1 + 30;
		rt.right =rt.left + 20;
		rt.top = PolygonListY2 + 10; 
		rt.bottom = rt.top + 20; 

		LayerDown[0].Create("-", dwStyle, rt, this, 5001);
		LayerDown[0].ShowWindow(TRUE);
	}


	// 绘图相关
	MouseInside = FALSE;
	MyPolygon = NULL;
	WhiteCanvas = NULL;
	BackupCanvasBeforeRotating = NULL;
	BackupCanvasBeforeScaling = NULL;

	// 状态变量
	ChoosingColor = FALSE;

	NewToolBoxPanel = TRUE;
	NewToolBoxItems = TRUE;

	BackupCanvasPart = FALSE;
	RecoverCanvasPart = FALSE;
	NewColorButtonPanel = TRUE;
	NewColorButtons = TRUE;
	NewColorButtonColors = TRUE;

	NewColorSelectorPanel = TRUE;
	NewColorSelectorSliders = FALSE;

	NewCanvas = TRUE;

	BackupCanvasBeforePolygon = FALSE;
	BackupCanvasForEachPolygonLine = FALSE;

	NeedToDrawAllPolygons = FALSE;
	NeedToDrawPolygonWhileMoving = FALSE;
	NeedToDrawPolygonAfterClicked = FALSE;
		BeginPolygonCycle = TRUE;
		BeginPolygon = TRUE;

	NeedToDrawFourAngles = FALSE;
	NeedToDrawFourAnglesWhileRotating = FALSE;
	NeedToDrawEightBorders = FALSE;
	NeedToDrawEightBordersWhileScaling = FALSE;

	BackupWhiteCanvas = FALSE;

	Dragging = FALSE;

	RotationStatus = NONEROTATION;
	CenterRadius = 3;
	RotatedPolygon = NULL;

	ScalingStatus = NONESCALING;
	ScaledPolygon = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CArtist2DDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);

		// 备份与恢复画布
		if(BackupCanvasPart)
		{
			BackupBeforeChoosingColor = GetBitmap(dc, ColorSelectorPanelX1 - 1, ColorSelectorPanelY1 - 1, ColorSelectorPanelX2 + 1, ColorSelectorPanelY2 + 1);
			BackupCanvasPart = FALSE;
		}
		if(RecoverCanvasPart)
		{
			DrawBitmapAndRelease(dc, &BackupBeforeChoosingColor, ColorSelectorPanelX1 - 1, ColorSelectorPanelY1 - 1, ColorSelectorPanelX2 + 1, ColorSelectorPanelY2 + 1);
			RecoverCanvasPart = FALSE;
		}
		// 刷新画布
		if(NewCanvas)
		{
			DrawCanvas(dc);
			NewCanvas = FALSE;
		}

		// 绘画相关
		if (BackupWhiteCanvas)
		{
			WhiteCanvas = GetBitmap(dc, 0, 0, ClientRect.Width(), ClientRect.Height());
			BackupWhiteCanvas = FALSE;
		}
		if(WhiteCanvas != NULL && NeedToDrawAllPolygons)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);

			CBitmap memBitmap, *oldmemBitmap;
			CopyBitmap(*WhiteCanvas, memBitmap);
			oldmemBitmap = memDC.SelectObject(&memBitmap);

			int PolygonsAmount = Polygons.GetCount();
			for(int i = PolygonsAmount - 1; i >= 0; i--)
			{
				ComplexPolygon* polygon = Polygons.GetAt(Polygons.FindIndex(i));
				if (polygon->Filled)
					FillPolygon(memDC, polygon);
				DrawPolygon(memDC, polygon);
			}

			BitBlt(dc, 0, 0, ClientRect.Width(), ClientRect.Height(), memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(oldmemBitmap);
			NeedToDrawAllPolygons = FALSE;
		}
		if(NeedToDrawPolygonWhileMoving)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);

			CBitmap memBitmap, *oldmemBitmap;
			CopyBitmap(*BackupCanvasForEachPolygonLine, memBitmap);
			oldmemBitmap = memDC.SelectObject(&memBitmap);

			DrawLine(memDC, BeginPoint, EndPoint, LineColor);

			BitBlt(dc, 0, 0, ClientRect.Width(), ClientRect.Height(), memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(oldmemBitmap);
			NeedToDrawPolygonWhileMoving = FALSE;
		}
		if(NeedToDrawPolygonAfterClicked)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);

			CBitmap memBitmap, *oldmemBitmap;
			CopyBitmap(*BackupCanvasForEachPolygonLine, memBitmap);
			oldmemBitmap = memDC.SelectObject(&memBitmap);

			DrawLine(memDC, BeginPoint, EndPoint, LineColor);

			BitBlt(dc, 0, 0, ClientRect.Width(), ClientRect.Height(), memDC, 0, 0, SRCCOPY);
			CopyBitmap(memBitmap, *BackupCanvasForEachPolygonLine);

			memDC.SelectObject(oldmemBitmap);

			BeginPoint = EndPoint;
			EndPoint = StoredPoint;

			NeedToDrawPolygonAfterClicked = FALSE;
		}
		if(NeedToDrawFourAngles)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);

			CBitmap memBitmap, *oldmemBitmap;
			CopyBitmap(*BackupCanvasBeforeRotating, memBitmap);
			oldmemBitmap = memDC.SelectObject(&memBitmap);

			int CenterX, CenterY;


			DrawEllipse(memDC, RotationCenter - CPoint(CenterRadius, CenterRadius), RotationCenter + CPoint(CenterRadius, CenterRadius));
			CenterX = RotateLB.x;
			CenterY = RotateLB.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = RotateRT.x;
			CenterY = RotateRT.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = RotateRB.x;
			CenterY = RotateRB.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = RotateLT.x;
			CenterY = RotateLT.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));

			BitBlt(dc, 0, 0, ClientRect.Width(), ClientRect.Height(), memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(oldmemBitmap);

			NeedToDrawFourAngles = FALSE;
		}
		if(NeedToDrawEightBorders)
		{
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);

			CBitmap memBitmap, *oldmemBitmap;
			CopyBitmap(*BackupCanvasBeforeScaling, memBitmap);
			oldmemBitmap = memDC.SelectObject(&memBitmap);

			int CenterX, CenterY;

			DrawEllipse(memDC, RotationCenter - CPoint(CenterRadius, CenterRadius), RotationCenter + CPoint(CenterRadius, CenterRadius));
			CenterX = Scale_1.x;
			CenterY = Scale_1.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_2.x;
			CenterY = Scale_2.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_3.x;
			CenterY = Scale_3.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_4.x;
			CenterY = Scale_4.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_6.x;
			CenterY = Scale_6.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_7.x;
			CenterY = Scale_7.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_8.x;
			CenterY = Scale_8.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_9.x;
			CenterY = Scale_9.y;
			DrawEllipse(memDC, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));

			BitBlt(dc, 0, 0, ClientRect.Width(), ClientRect.Height(), memDC, 0, 0, SRCCOPY);

			memDC.SelectObject(oldmemBitmap);

			NeedToDrawEightBorders = FALSE;
		}
		if(NeedToDrawFourAnglesWhileRotating)
		{
			int CenterX, CenterY;
			DrawEllipse(dc, RotationCenter - CPoint(CenterRadius, CenterRadius), RotationCenter + CPoint(CenterRadius, CenterRadius));
			CenterX = RotateLB.x;
			CenterY = RotateLB.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = RotateRT.x;
			CenterY = RotateRT.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = RotateRB.x;
			CenterY = RotateRB.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = RotateLT.x;
			CenterY = RotateLT.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));

			NeedToDrawFourAnglesWhileRotating = FALSE;
		}
		if(NeedToDrawEightBordersWhileScaling)
		{
			int CenterX, CenterY;
			DrawEllipse(dc, RotationCenter - CPoint(CenterRadius, CenterRadius), RotationCenter + CPoint(CenterRadius, CenterRadius));
			CenterX = Scale_1.x;
			CenterY = Scale_1.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_2.x;
			CenterY = Scale_2.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_3.x;
			CenterY = Scale_3.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_4.x;
			CenterY = Scale_4.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_6.x;
			CenterY = Scale_6.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_7.x;
			CenterY = Scale_7.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_8.x;
			CenterY = Scale_8.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));
			CenterX = Scale_9.x;
			CenterY = Scale_9.y;
			DrawEllipse(dc, CPoint(CenterX - CenterRadius, CenterY - CenterRadius), CPoint(CenterX + CenterRadius, CenterY + CenterRadius));

			NeedToDrawEightBordersWhileScaling = FALSE;
		}

		// 颜色选择框
		if(NewColorSelectorPanel)
		{
			DrawColorSelectorPanel(dc);
			NewColorSelectorPanel = FALSE;
		}
		if(NewColorSelectorSliders)
		{
			DrawColorSelectorSliders(dc);
			NewColorSelectorSliders = FALSE;
		}

		// 工具栏
		if(NewToolBoxPanel)
		{
			DrawToolBoxPanel(dc);
			NewToolBoxPanel = FALSE;
		}
		if(NewToolBoxItems)
		{
			DrawToolBoxItems(dc);
			NewToolBoxItems = FALSE;
		}

		//  颜色按钮
		if(NewColorButtonPanel)
		{
			DrawColorButtonPanel(dc);
			NewColorButtonPanel = FALSE;
		}
		if(NewColorButtons)
		{
			DrawColorButtons(dc);
			NewColorButtons = FALSE;
		}
		if(NewColorButtonColors)
		{
			DrawColorButtonColors(dc);
			NewColorButtonColors = FALSE;
		}
		//CDialog::OnPaint();
	}
}

// 击中范围
BOOL CArtist2DDlg::HitTool(CPoint point)
{
	return (point.x >= ToolBoxX1
		&& point.x <= ToolBoxX2
		&& point.y >= ToolBoxY1
		&& point.y <= ToolBoxY2);
}

BOOL CArtist2DDlg::HitColor(CPoint point)
{
	return (point.x >= ColorX1
		&& point.x <= ColorX2
		&& point.y >= ColorY1
		&& point.y <= ColorY2);
}



BOOL CArtist2DDlg::HitColorSelector(CPoint point)
{
	return (point.x >= ColorSelectorPanelX1
		&& point.x <= ColorSelectorPanelX2
		&& point.y >= ColorSelectorPanelY1
		&& point.y <= ColorSelectorPanelY2);
}
BOOL CArtist2DDlg::HitCanvas(CPoint point)
{
	return (point.x > CanvasX1
		&& point.x < CanvasX2
		&& point.y > CanvasY1
		&& point.y < CanvasY2);
}
// 多边形操作相关
void CArtist2DDlg::PerformRotation(CPoint* point, double angle)
{
	CPoint cord;
	cord.x = (long)(point->x * cos(angle) - point->y * sin(angle) + 0.5);
	cord.y = (long)(point->x * sin(angle) + point->y * cos(angle) + 0.5);
	point->x = cord.x;
	point->y = cord.y;
}
void CArtist2DDlg::PerformScaling(CPoint* point, double sx, double sy)
{
	point->x = (int)(point->x * sx + 0.5);
	point->y = (int)(point->y * sy + 0.5);
}
double CArtist2DDlg::GetFromTanTheta(CPoint p)
{
	double PI = acos(-1.0);
	if(p.x == 0)
	{
		return p.y > 0 ? PI * 0.5 : PI * 1.5;
	}
	if(p.y == 0)
	{
		return p.x > 0 ? 0 : PI;
	}
	double TanTheta = 1.0 * p.y / p.x;
	if(p.x > 0)
	{
		if(p.y > 0)
			return atan(TanTheta);
		else
			return atan(TanTheta) + 2 * PI;
	}else
	{
		if(p.y > 0)
			return atan(TanTheta) + PI;
		else
			return atan(TanTheta) + PI;
	}
}
double CArtist2DDlg::DistanceBetween(CPoint p1, CPoint p2)
{
	int dx = p1.x - p2.x;
	int dy = p1.y - p2.y;
	return sqrt(1.0 * (dx * dx + dy * dy));
}
double CArtist2DDlg::DistanceBetween(PolygonPoint* p1, PolygonPoint* p2)
{
	int dx = p1->cord.x - p2->cord.x;
	int dy = p1->cord.y - p2->cord.y;
	return sqrt(1.0 * (dx * dx + dy * dy));
}
BOOL CArtist2DDlg::ReallyBefore(PolygonPoint* p1, PolygonPoint* p2, PolygonCycle* SharedCycle, BOOL IsCli)
{
	int Edge1 = IsCli ? p1->CliEdge : p1->OriEdge;
	int Edge2 = IsCli ? p2->CliEdge : p2->OriEdge;
	if(Edge1 < Edge2)
		return TRUE;
	else if(Edge1 > Edge2)
		return FALSE;
	PolygonPoint* StartPoint = SharedCycle->GetPoint(Edge1);
	return (DistanceBetween(StartPoint, p1) < DistanceBetween(StartPoint, p2));
}
CPoint CArtist2DDlg::GetCrossPoint(CPoint p1, CPoint p2, CPoint p3, CPoint p4)
{
	//a1 x+b1 y = c1
	int a1 = p1.y - p2.y, b1 = p2.x - p1.x, c1 = p1.y * p2.x - p1.x * p2.y;
	//a2 x+b2 y = c2
	int a2 = p3.y - p4.y, b2 = p4.x - p3.x, c2 = p3.y * p4.x - p3.x * p4.y;
	/*
	(a1 b1)(x) = (c1)
	(a2 b2)(y)    (c2)

	(x) = ( b2 -b1) (c1)  /  | a1 b1 |
	(y)    (-a2   a1) (c2)  /  | a2 b2 |
	*/

	double a = a1 * b2 - a2 * b1;
	double x = (b2 * c1 - b1 * c2) / a;
	double y = (a1 * c2 - a2 * c1) / a;
	return CPoint((int)(x + 0.5), (int)(y + 0.5));
}
BOOL CArtist2DDlg::InsidePolygonCycle(PolygonPoint* point, PolygonCycle* cycle)
{
	int PointAmount = cycle->GetCount();
	int Count = 0;
	for(int i = 0; i < PointAmount; i++)
	{
		PolygonPoint* StartPoint = cycle->GetPoint(i);
		PolygonPoint* EndPoint = cycle->GetPoint((i + 1) % PointAmount);
		int dy1 = StartPoint->cord.y - point->cord.y;
		int dy2 = EndPoint->cord.y - point->cord.y;
		if (
			(
				(dy1 == 0 && dy2 !=0) ||
				(dy1 != 0 && dy2 ==0) ||
				(dy1 > 0 && dy2 < 0) ||
				(dy1 < 0 && dy2 > 0)
			) &&
			(
				GetCrossPoint(StartPoint->cord, EndPoint->cord, CPoint(0, point->cord.y), point->cord).x < point->cord.x
			)
		)
		{
			Count++;
		}
	}
	return Count % 2 == 1;
}
BOOL CArtist2DDlg::CheckSegmentCrossed(PolygonPoint* a1, PolygonPoint* b1, PolygonPoint* a2, PolygonPoint* b2)
{
	//F(x,y) = (x1-x)(y2-y1)-(x2-x1)(y1-y)
	//F(p) = (a1.x-p.x)*(b1.y-a1.y)-(b1.x-a1.x)*(a1.y-p.y)
	int d1 = (a1->cord.x-a2->cord.x)*(b1->cord.y-a1->cord.y)-(b1->cord.x-a1->cord.x)*(a1->cord.y-a2->cord.y);
	int d2 = (a1->cord.x-b2->cord.x)*(b1->cord.y-a1->cord.y)-(b1->cord.x-a1->cord.x)*(a1->cord.y-b2->cord.y);

	//F(p) = (a2.x-p.x)*(b2.y-a2.y)-(b2.x-a2.x)*(a2.y-p.y)
	int d3 = (a2->cord.x-a1->cord.x)*(b2->cord.y-a2->cord.y)-(b2->cord.x-a2->cord.x)*(a2->cord.y-a1->cord.y);
	int d4 = (a2->cord.x-b1->cord.x)*(b2->cord.y-a2->cord.y)-(b2->cord.x-a2->cord.x)*(a2->cord.y-b1->cord.y);

	return
		((d1 >= 0) && (d2 <= 0) || (d1 <= 0) && (d2 >= 0))
		&&
		((d3 >= 0) && (d4 <= 0) || (d3 <= 0) && (d4 >= 0));
}
BOOL CArtist2DDlg::CheckPolygonCrossed(ComplexPolygon* polygon)
{
	int CycleAmount = polygon->GetCount();
	//圈内检测
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = polygon->GetCycle(i);
		int PointAmount = cycle->GetCount();

		if(PointAmount < 3)
			return true;

		//第一条边
		for(int k = 2; k < PointAmount - 1; k++)
		{
			if(CheckSegmentCrossed
				(
				cycle->GetPoint(0),
				cycle->GetPoint(1),
				cycle->GetPoint(k),
				cycle->GetPoint((k + 1) % PointAmount)
				)
				)
				return true;
		}
		
		//后面的边
		for(int j = 1; j < PointAmount; j++)
		{
			//[j + 2, i - 1]

			for(int k = j + 2; k < PointAmount; k++)
			{
				if(CheckSegmentCrossed
					(
					cycle->GetPoint(j),
					cycle->GetPoint((j + 1) % PointAmount),
					cycle->GetPoint(k),
					cycle->GetPoint((k + 1) % PointAmount)
					)
				)
				return true;
			}
		}
	}

	//圈间检测
	for(int h =0; h<CycleAmount; h++)
	{
		for(int i = h + 1; i < CycleAmount; i++)
		{
			PolygonCycle* cycle1 = polygon->GetCycle(h);
			PolygonCycle* cycle2 = polygon->GetCycle(i);
			int PointAmount1 = cycle1->GetCount();
			int PointAmount2 = cycle2->GetCount();
			for(int j = 0; j < PointAmount1; j++)
			{
				for(int k = 0; k < PointAmount2; k++)
				{
					if(CheckSegmentCrossed
						(
						cycle1->GetPoint(j),
						cycle1->GetPoint((j + 1) % PointAmount1),
						cycle2->GetPoint(k),
						cycle2->GetPoint((k + 1) % PointAmount2)
						)
						)
						return true;
				}
			}
		}
	}
	return false;
}
void CArtist2DDlg::SortPolygonCycle(PolygonCycle* cycle, BOOL reversed)
{
	cycle->Reversed = reversed;
	int PointAmount = cycle->GetCount();
	int MinX = INT_MAX;
	int Reci = 0;
	for(int i = 0; i < PointAmount; i++)
	{
		PolygonPoint* CurPoint = cycle->GetPoint(i);
		if(CurPoint->cord.x < MinX)
		{
			MinX= CurPoint->cord.x;
			Reci = i;
		}
	}

	PolygonPoint* p0 = cycle->GetPoint((Reci - 1 + PointAmount) % PointAmount);
	PolygonPoint* p2 = cycle->GetPoint((Reci + 1 + PointAmount) % PointAmount);

	BOOL CurReversed = (p0->cord.y < p2->cord.y);

	if(reversed ^ CurReversed)
	{
		for(int i = (PointAmount - 1) / 2; i >=0 ; i--)
		{
			int j = PointAmount - 1 - i;
			PolygonPoint* temp = cycle->GetPoint(i);
			cycle->SetPoint(i, cycle->GetPoint(j));
			cycle->SetPoint(j, temp);
		}
	}
}

// 多边形操作
BOOL CArtist2DDlg::IsNumber(CString str)
{
	int len = str.GetLength();
	for(int i = 0; i < len; i++)
	{
		char c = str.GetAt(i);
		if(c > '9' || c < '0')
			return FALSE;
	}
	return TRUE;
}
void CArtist2DDlg::GetClippingInformation(CString* pStrClippedID ,CString* pStrClippingID) 
{
	CClippingDialog inputbox;
	inputbox.m_StrClippedID = "0";
	inputbox.m_StrClippingID = "0";
	while(IDOK == inputbox.DoModal())
	{
		if(!IsNumber(inputbox.m_StrClippedID) || !IsNumber( inputbox.m_StrClippingID))
		{
			AfxMessageBox("多边形编号格式不正确。");
			continue;
		}
		*pStrClippedID = inputbox.m_StrClippedID;
		*pStrClippingID = inputbox.m_StrClippingID;
		break;
	}
}
void CArtist2DDlg::RefreshMinMaxXY(ComplexPolygon* polygon)
{
	polygon->MinX = polygon->MinY = INT_MAX;
	polygon->MaxX = polygon->MaxY = INT_MIN;
	int CycleAmount = polygon->GetCount();
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = polygon->GetCycle(i);
		int PointAmount = cycle->GetCount();
		for (int j = 0; j < PointAmount; j++)
		{
			PolygonPoint* point = cycle->GetPoint(j);
			if(point->cord.x < polygon->MinX)
				polygon->MinX = point->cord.x;
			if(point->cord.x > polygon->MaxX)
				polygon->MaxX = point->cord.x;
			if(point->cord.y < polygon->MinY)
				polygon->MinY = point->cord.y;
			if(point->cord.y > polygon->MaxY)
				polygon->MaxY = point->cord.y;
		}
	}
	polygon->width = polygon->MaxX - polygon->MinX + 1;
	polygon->height = polygon->MaxY - polygon->MinY + 1;
}	
void CArtist2DDlg::TurnOverPolygon(ComplexPolygon* polygon, BOOL horizontal)
{
	CPoint center;
	center.x = (polygon->MinX + polygon->MaxX) >> 1;
	center.y = (polygon->MinY + polygon->MaxY) >> 1;
	double sx = horizontal ? -1 : 1;
	double sy = horizontal ? 1 : -1;
	ScalePolygon(polygon, center, sx, sy);
}
void CArtist2DDlg::ScalePolygon(ComplexPolygon* polygon, CPoint center, double sx, double sy)
{
	DragPolygon(polygon, center, CPoint(0, 0));
	int CycleAmount = polygon->GetCount();
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = polygon->GetCycle(i);
		int PointAmount = cycle->GetCount();
		for(int j = 0; j < PointAmount; j++)
		{
			PolygonPoint* point = cycle->GetPoint(j);
			PerformScaling(&(point->cord), sx, sy);
		}
	}
	DragPolygon(polygon, CPoint(0, 0), center);
}
void CArtist2DDlg::RotatePolygon(ComplexPolygon* polygon, CPoint center, double angle)
{
	DragPolygon(polygon, center, CPoint(0, 0));
	int CycleAmount = polygon->GetCount();
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = polygon->GetCycle(i);
		int PointAmount = cycle->GetCount();
		for(int j = 0; j < PointAmount; j++)
		{
			PolygonPoint* point = cycle->GetPoint(j);
			PerformRotation(&(point->cord), angle);
		}
	}
	DragPolygon(polygon, CPoint(0, 0), center);
}
void CArtist2DDlg::DragPolygon(ComplexPolygon* polygon, CPoint from, CPoint to)
{
	CPoint delta = to - from;
	int CycleAmount = polygon->GetCount();
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = polygon->GetCycle(i);
		int PointAmount = cycle->GetCount();
		for(int j = 0; j < PointAmount; j++)
		{
			PolygonPoint* point = cycle->GetPoint(j);
			point->cord += delta;
		}
	}
}
void CArtist2DDlg::FormalizePolygon(ComplexPolygon* polygon)
{
	int CycleAmount = polygon->GetCount();
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = polygon->GetCycle(i);
		int PointAmount = cycle->GetCount();
		for(int j = 1; j < PointAmount; j++)
		{
			PolygonPoint* PrePoint = cycle->GetPoint(j - 1);
			PolygonPoint* CurPoint = cycle->GetPoint(j);
			if(PrePoint->cord == CurPoint->cord)
			{
				cycle->Remove(j);
				PointAmount--;
			}
		}
		if(cycle->GetTail()->cord == cycle->GetHead()->cord)
			cycle->Remove(cycle->GetCount() - 1);
	}
	SortPolygon(polygon);
	RefreshMinMaxXY(polygon);

	CycleAmount = polygon->GetCount();
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = polygon->GetCycle(i);
		int PointAmount = cycle->GetCount();
		for(int j = 0; j < PointAmount; j++)
		{
			PolygonPoint* point = cycle->GetPoint(j);
			point->CliCycle = point->CliEdge = 
				point->OriCycle = point->OriEdge = 
				point->LaterCliEdge = point->LaterOriEdge = -1;
			point->CrossPointID = -1;
			point->OriInserted = point->CliInserted = FALSE;
			point->kind = VERTEX;
		}
	}
}
void CArtist2DDlg::InsertPolygon(ComplexPolygon* polygon)
{
	FormalizePolygon(polygon);
	Polygons.AddHead(polygon);
	PolygonID++;
	CString strID;
	strID.Format("%d", PolygonID);
	PolygonList->InsertString(0, strID);
	PolygonList->SetCheck(0, BST_CHECKED );
	PolygonList->SetCurSel(0);
}
void CArtist2DDlg::RemovePolygon(int index)
{
	Polygons.RemoveAt(Polygons.FindIndex(index));
	PolygonList->DeleteString(index);
}
ComplexPolygon* CArtist2DDlg::ClipPolygon(ComplexPolygon* pOriPolygon, ComplexPolygon* pCliPolygon)
{
	ComplexPolygon* result = new ComplexPolygon();
	ComplexPolygon* OriPolygon = DuplicatePolygon(pOriPolygon);
	ComplexPolygon* CliPolygon = DuplicatePolygon(pCliPolygon);
	FormalizePolygon(OriPolygon);
	FormalizePolygon(CliPolygon);

	// 找出所有交点，并标记入点和出点
	CList<PolygonPoint*> CrossPoints = CList<PolygonPoint*>();
	{
		int CrossPointID = 0;
		int OCycleAmount = OriPolygon->GetCount();
		for(int oi = 0; oi < OCycleAmount; oi++)
		{
			PolygonCycle* OCycle = OriPolygon->GetCycle(oi);
			int OPointAmount = OCycle->GetCount();

			for(int oj = 0; oj < OPointAmount; oj++)
			{

				PolygonPoint* OStartPoint = OCycle->GetPoint(oj);
				PolygonPoint* OEndPoint = OCycle->GetPoint((oj + 1) % OPointAmount);
				int CCycleAmount = CliPolygon->GetCount();


				for(int ci = 0; ci < CCycleAmount; ci++)
				{
					PolygonCycle* CCycle = CliPolygon->GetCycle(ci);
					int CPointAmount = CCycle->GetCount();

					for(int cj = 0; cj < CPointAmount; cj++)
					{

						PolygonPoint* CStartPoint = CCycle->GetPoint(cj);
						PolygonPoint* CEndPoint = CCycle->GetPoint((cj + 1) % CPointAmount);

						if(CheckSegmentCrossed(OStartPoint, OEndPoint, CStartPoint, CEndPoint))
						{
							/* 保证相交而不共线
							|x1 y1|
							|x2 y2|
							=x1*y2 + x2*y1
							=(OEndPoint.x - OStartPoint.x) * (CEndPoint.y - CStarttPoint.y) - 
							(CEndPoint.x - CStartPoint.x) * (CEndPoint.y - CStartPoint.y)
							*/
							if ((OEndPoint->cord.x - OStartPoint->cord.x) * (CEndPoint->cord.y - CStartPoint->cord.y) != (CEndPoint->cord.x - CStartPoint->cord.x) * (OEndPoint->cord.y - OStartPoint->cord.y))
							{
								CPoint CrossPoint = GetCrossPoint(OStartPoint->cord, OEndPoint->cord, CStartPoint->cord, CEndPoint->cord);
								PolygonPoint* cp = new PolygonPoint();
								cp->CrossPointID = CrossPointID++;
								cp->cord = CrossPoint;
								cp->OriCycle = oi; cp->OriEdge = cp->LaterOriEdge = oj;
								cp->CliCycle = ci; cp->CliEdge = cp->LaterCliEdge = cj;
								CPoint Vector_C = CEndPoint->cord - CStartPoint->cord;
								CPoint Vector_O = OEndPoint->cord - OStartPoint->cord;
								if (Vector_O.x * Vector_C.y - Vector_C.x * Vector_O.y < 0)
									cp->kind = OUTPOINT;//出点
								else
									cp->kind = INPOINT;//入点
								cp->OriInserted = cp->CliInserted = FALSE;
								CrossPoints.AddTail(cp);
							}
						}
					}
				}
			}
		}
	}

	// 得到交点插入后的顺序
	// 交点们顺势插入
	{
		// 得到交点插入后的顺序
		int CrossPointAmount = CrossPoints.GetCount();
		for(int i = 0; i < CrossPointAmount; i++)
		{
			for(int j = i + 1; j < CrossPointAmount; j++)
			{
				PolygonPoint* p1 = CrossPoints.GetAt(CrossPoints.FindIndex(i));
				PolygonPoint* p2 = CrossPoints.GetAt(CrossPoints.FindIndex(j));
				if(p1->CliCycle == p2->CliCycle)
				{
					PolygonCycle* SharedCycle = CliPolygon->GetCycle(p1->CliCycle);

					if(ReallyBefore(p1, p2, SharedCycle, TRUE))
						p2->LaterCliEdge++;
					else
						p1->LaterCliEdge++;
				}
				if(p1->OriCycle == p2->OriCycle)
				{
					PolygonCycle* SharedCycle = OriPolygon->GetCycle(p1->OriCycle);

					if(ReallyBefore(p1, p2, SharedCycle, FALSE))
						p2->LaterOriEdge++;
					else
						p1->LaterOriEdge++;
				}
			}
		}


		// 交点们顺势插入
		int OriCycleAmount = OriPolygon->GetCount();
		for(int i = 0; i < OriCycleAmount; i++)
		{
			int PointsInOriCycle = 0;
			for(int j = 0; j < CrossPointAmount; j++)
			{
				PolygonPoint* cp = CrossPoints.GetAt(CrossPoints.FindIndex(j));
				if(cp->OriCycle == i)
					PointsInOriCycle++;
			}
			while(PointsInOriCycle > 0)
			{
				int Recj = -1, Earliest = INT_MAX; 
				for(int j = 0; j < CrossPointAmount; j++)
				{
					PolygonPoint* cp = CrossPoints.GetAt(CrossPoints.FindIndex(j));
					if(!cp->OriInserted && cp->OriCycle == i && cp->LaterOriEdge < Earliest)
					{
						Recj = j;
						Earliest = cp->LaterOriEdge;
					}
				}

				if(Recj >= 0)
				{
					PolygonCycle* OCycle = OriPolygon->GetCycle(i);
					PolygonPoint* cp = CrossPoints.GetAt(CrossPoints.FindIndex(Recj));
					cp->OriInserted = TRUE;
					OCycle->InsertAfter(cp->LaterOriEdge, cp);
					PointsInOriCycle--;
				}
			}
		}

		int CliCycleAmount = CliPolygon->GetCount();
		for(int i = 0; i < CliCycleAmount; i++)
		{
			int PointsInCliCycle = 0;
			for(int j = 0; j < CrossPointAmount; j++)
			{
				PolygonPoint* cp = CrossPoints.GetAt(CrossPoints.FindIndex(j));
				if(cp->CliCycle == i)
					PointsInCliCycle++;
			}
			while(PointsInCliCycle > 0)
			{
				int Recj = -1, Earliest = INT_MAX; 
				for(int j = 0; j < CrossPointAmount; j++)
				{
					PolygonPoint* cp = CrossPoints.GetAt(CrossPoints.FindIndex(j));
					if(!cp->CliInserted && cp->CliCycle == i && cp->LaterCliEdge < Earliest)
					{
						Recj = j;
						Earliest = cp->LaterCliEdge;
					}
				}
				if(Recj >= 0)
				{
					PolygonCycle* CCycle = CliPolygon->GetCycle(i);
					PolygonPoint* cp = CrossPoints.GetAt(CrossPoints.FindIndex(Recj));
					cp->CliInserted = TRUE;
					cp = DuplicatePoint(cp);
					CCycle->InsertAfter(cp->LaterCliEdge, cp);
					PointsInCliCycle--;
				}
			}
		}

	}

	// 不相交部分
	{
		int OCycleAmount = OriPolygon->GetCount();
		for(int i = 0; i < OCycleAmount; i++)
		{
			PolygonCycle* c = OriPolygon->GetCycle(i);

			// 如果有交点就飞走
			BOOL HasCross = FALSE;
			int OPointAmount = c->GetCount();
			for(int j = 0; j < OPointAmount; j++)
			{
				PolygonPoint* p = c->GetPoint(j);
				if(p->kind == INPOINT || p->kind == OUTPOINT)
				{
					HasCross = TRUE;
					break;
				}
			}
			if(HasCross)
				continue;

			// 没有交点的时候，看看是不是在那啥里面
			int CCycleAmount = CliPolygon->GetCount();
			PolygonCycle* _c = NULL;
			for(int j = 0; j < CCycleAmount; j++)
			{
				PolygonCycle* tmpc = CliPolygon->GetCycle(j);
				if (	InsidePolygonCycle(c->GetPoint(0), tmpc) && 
					(_c==NULL || InsidePolygonCycle(tmpc->GetPoint(0), _c))
					)
					_c = tmpc;
			}
			if(_c == NULL || !_c->Reversed)
				continue;

			// 下面就是了，在那啥里面
			PolygonCycle* CycleToInsert = DuplicateCycle(c); 
			CycleToInsert->LineColor = LineColor;
			result->InsertToTail(CycleToInsert);
		}

		int CCycleAmount = CliPolygon->GetCount();
		for(int i = 0; i < CCycleAmount; i++)
		{
			PolygonCycle* c = CliPolygon->GetCycle(i);

			// 如果有交点就飞走
			BOOL HasCross = FALSE;
			int CPointAmount = c->GetCount();
			for(int j = 0; j < CPointAmount; j++)
			{
				PolygonPoint* p = c->GetPoint(j);
				if(p->kind == INPOINT || p->kind == OUTPOINT)
				{
					HasCross = TRUE;
					break;
				}
			}
			if(HasCross)
				continue;

			// 没有交点的时候，看看是不是在那啥里面
			int OCycleAmount = OriPolygon->GetCount();
			PolygonCycle* _c = NULL;
			for(int j = 0; j < OCycleAmount; j++)
			{
				PolygonCycle* tmpc = OriPolygon->GetCycle(j);
				if (	InsidePolygonCycle(c->GetPoint(0), tmpc) && 
					(_c==NULL || InsidePolygonCycle(tmpc->GetPoint(0), _c))
					)
					_c = tmpc;
			}
			if(_c == NULL || !_c->Reversed)
				continue;

			// 下面就是了，在那啥里面
			PolygonCycle* CycleToInsert = DuplicateCycle(c); 
			CycleToInsert->LineColor = LineColor;
			result->InsertToTail(CycleToInsert);
		}
	}

	// 开始计算得到裁剪出来的多边形
	{
		do
		{
			// 找出交点
			BOOL HasCross = FALSE;
			int Reci, Recj, Recp;
			int OCycleAmount = OriPolygon->GetCount();
			for(int i = 0; i < OCycleAmount; i++)
			{
				PolygonCycle* OCycle = OriPolygon->GetCycle(i);
				int OPointAmount = OCycle->GetCount();

				for(int j = 0; j < OPointAmount; j++)
				{
					if (OCycle->GetPoint(j)->kind == INPOINT || OCycle->GetPoint(j)->kind == OUTPOINT)
					{
						HasCross = TRUE;
						Reci = i; Recj = j; Recp = 0;
						break;
					}
				}
				if(HasCross)
					break;
			}
			if(!HasCross)
			{
				for(int i = 0; i < OCycleAmount; i++)
				{
					PolygonCycle* OCycle = OriPolygon->GetCycle(i);
					int OPointAmount = OCycle->GetCount();

					for(int j = 0; j < OPointAmount; j++)
					{
						if (OCycle->GetPoint(j)->kind == INPOINT || OCycle->GetPoint(j)->kind == OUTPOINT)
						{
							HasCross = TRUE;
							Reci = i; Recj = j; Recp = 1;
							break;
						}
					}
					if(HasCross)
						break;
				}
			}
			if(!HasCross)
				break;

			// 抽出来了交点位置(Recp, Reci, Recj)
			ComplexPolygon* polygon = (Recp == 0 ? OriPolygon : CliPolygon);
			PolygonCycle* cycle = polygon->GetCycle(Reci);
			PolygonPoint* point = cycle->GetPoint(Recj);

			PolygonCycle* NewCycle = new PolygonCycle();
			PolygonPoint* FinishPoint = point;

			PolygonPoint* NewPoint;

			BOOL jumped = FALSE;
			BOOL FirstPoint = TRUE;
			do 
			{
				switch (point->kind)
				{
				case INPOINT:
					{
						if(jumped)
						{
							//point->kind = VERTEX;
							PolygonCycle* pc = OriPolygon->GetCycle(point->OriCycle);
							pc->GetPoint((point->LaterOriEdge + 1) % pc->GetCount())->kind = VERTEX;
							pc = CliPolygon->GetCycle(point->CliCycle);
							pc->GetPoint((point->LaterCliEdge + 1) % pc->GetCount())->kind = VERTEX;
							NewPoint = DuplicatePoint(point);
							NewPoint->kind = VERTEX;
							NewCycle->InsertToTail(NewPoint);

							ComplexPolygon* _polygon = (Recp == 0 ? OriPolygon : CliPolygon);
							PolygonCycle* _cycle = _polygon->GetCycle(Reci);
							Recj = (Recj + 1) % _cycle->GetCount();

							jumped = FALSE;
							FirstPoint = FALSE;
						}
						else
						{
							Recp = 0;
							Reci = point->OriCycle;

							ComplexPolygon* _polygon = (Recp == 0 ? OriPolygon : CliPolygon);
							PolygonCycle* _cycle = _polygon->GetCycle(Reci);
							Recj = (point->LaterOriEdge + 1) % _cycle->GetCount();

							jumped = TRUE;
						}
						break;
					}
				case OUTPOINT:
					{

						if(jumped)
						{
							//point->kind = VERTEX;
							PolygonCycle* pc = OriPolygon->GetCycle(point->OriCycle);
							pc->GetPoint((point->LaterOriEdge + 1) % pc->GetCount())->kind = VERTEX;
							pc = CliPolygon->GetCycle(point->CliCycle);
							pc->GetPoint((point->LaterCliEdge + 1) % pc->GetCount())->kind = VERTEX;

							NewPoint = DuplicatePoint(point);
							NewPoint->kind = VERTEX;
							NewCycle->InsertToTail(NewPoint);

							ComplexPolygon* _polygon = (Recp == 0 ? OriPolygon : CliPolygon);
							PolygonCycle* _cycle = _polygon->GetCycle(Reci);
							Recj = (Recj + 1) % _cycle->GetCount();

							jumped = FALSE;
							FirstPoint = FALSE;
						}else
						{
							Recp = 1;
							Reci = point->CliCycle;

							ComplexPolygon* _polygon = (Recp == 0 ? OriPolygon : CliPolygon);
							PolygonCycle* _cycle = _polygon->GetCycle(Reci);
							Recj = (point->LaterCliEdge + 1) % _cycle->GetCount();

							jumped = TRUE;
						}
						break;
					}
				case VERTEX:
					{
						NewPoint = DuplicatePoint(point);
						NewPoint->kind = VERTEX;
						NewCycle->InsertToTail(NewPoint);

						ComplexPolygon* _polygon = (Recp == 0 ? OriPolygon : CliPolygon);
						PolygonCycle* _cycle = _polygon->GetCycle(Reci);

						Recj = (Recj + 1) % _cycle->GetCount();
					}
				default:
					{
						break;
					}
				}
				polygon = (Recp == 0 ? OriPolygon : CliPolygon);
				cycle = polygon->GetCycle(Reci);
				point = cycle->GetPoint(Recj);
			} while (point->CrossPointID < 0 || (FinishPoint->CrossPointID != point->CrossPointID) || FirstPoint);

			//point->kind = VERTEX;
			PolygonCycle* pc = OriPolygon->GetCycle(point->OriCycle);
			pc->GetPoint((point->LaterOriEdge + 1) % pc->GetCount())->kind = VERTEX;
			pc = CliPolygon->GetCycle(point->CliCycle);
			pc->GetPoint((point->LaterCliEdge + 1) % pc->GetCount())->kind = VERTEX;

			NewCycle->LineColor = LineColor;
			result->InsertToTail(NewCycle);
		}while(TRUE);
	}
	FormalizePolygon(result);
	ClearUpPolygon(OriPolygon);
	ClearUpPolygon(CliPolygon);
	return result;
}
void CArtist2DDlg::SortPolygon(ComplexPolygon* polygon)
{
	if(CheckPolygonCrossed(polygon))
		return;
	int CycleAmount = polygon->GetCount();
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonPoint* point = polygon->GetCycle(i)->GetPoint(0);
		int OutsideAmount = 0;
		for(int j = 0; j < CycleAmount; j++)
		{
			if(i == j)
				continue;
			if(InsidePolygonCycle(point, polygon->GetCycle(j)))
				OutsideAmount++;
		}
		SortPolygonCycle(polygon->GetCycle(i), (OutsideAmount % 2 == 0));
	}
}
void CArtist2DDlg::ClearUpPolygon(ComplexPolygon* polygon)
{
	int CycleAmount = polygon->GetCount();
	for (int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* c = polygon->GetCycle(i);
		int PointAmount = c->GetCount();
		for(int j = 0; j < PointAmount; j++)
		{
			PolygonPoint* p = c->GetPoint(j);
			delete p;
		}
		delete c;
	}
	delete polygon;
}
ComplexPolygon* CArtist2DDlg::DuplicatePolygon(ComplexPolygon* polygon)
{
	ComplexPolygon* result;
	result = new ComplexPolygon();
	result->Filled = polygon->Filled;
	result->FilledColor = polygon->FilledColor;
	result->visible = polygon->visible;
	result->MinX = polygon->MinX;
	result->MaxX = polygon->MaxX;
	result->MinY = polygon->MinY;
	result->MaxY = polygon->MaxY;
	result->width = polygon->width;
	result->height = polygon->height;
	int CycleAmount = polygon->GetCount();
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = new PolygonCycle();
		PolygonCycle* OriCycle = polygon->GetCycle(i);
		cycle->Reversed = OriCycle->Reversed;
		cycle->LineColor = OriCycle->LineColor;

		int PointAmount = OriCycle->GetCount();
		for(int j = 0; j < PointAmount; j++)
		{
			PolygonPoint* point = DuplicatePoint(OriCycle->GetPoint(j));
			cycle->InsertToTail(point);
		}
		result->InsertToTail(cycle);
	}
	return result;
}
PolygonCycle* CArtist2DDlg::DuplicateCycle(PolygonCycle* cycle)
{
	PolygonCycle* result = new PolygonCycle();
	result->Reversed = cycle->Reversed;
	result->LineColor = cycle->LineColor;

	int PointAmount = cycle->GetCount();
	for(int j = 0; j < PointAmount; j++)
	{
		PolygonPoint* point = DuplicatePoint(cycle->GetPoint(j));
		result->InsertToTail(point);
	}
	return result;
}
PolygonPoint* CArtist2DDlg::DuplicatePoint(PolygonPoint* point)
{
	PolygonPoint* result;
	result = new PolygonPoint();
	result->CliCycle = point->CliCycle;
	result->CliEdge = point->CliEdge;
	result->CliInserted = point->CliInserted;
	result->cord = point->cord;
	result->kind = point->kind;
	result->LaterCliEdge = point->LaterCliEdge;
	result->LaterOriEdge = point->LaterOriEdge;
	result->OriCycle = point->OriCycle;
	result->OriEdge = point->OriEdge;
	result->OriInserted = point->OriInserted;
	result->CrossPointID = point->CrossPointID;
	return result;
}
// 位图相关
CBitmap* CArtist2DDlg::GetBitmap(CDC& dc, int x1, int y1, int x2, int y2)
{
	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(&dc);

	CBitmap *memBitmap, *oldmemBitmap;
	memBitmap = new CBitmap();
	memBitmap->CreateCompatibleBitmap(&dc, x2 - x1, y2 - y1);

	oldmemBitmap = memDC.SelectObject(memBitmap);
	memDC.BitBlt(0, 0, x2 - x1, y2 - y1, &dc, x1, y1, SRCCOPY);
	memDC.SelectObject(oldmemBitmap);

	return memBitmap;
}

bool CArtist2DDlg::CopyBitmap( const CBitmap &bmpSrc, CBitmap &bmpDest )
{
	bool bResult = false;
	BITMAP bm;

	// Attempt to retrieve the existing bitmap structure.
	if ( 0 == bmpSrc.GetObject( sizeof(BITMAP), &bm ) )
	{
		return bResult;
	}

	// Get a handle to the current device context.
	HDC hDC = ::GetDC( NULL );

	// Prepare a new bitmap info structure.
	// Note this function allocates the memory for the structure,
	// so make sure that the memory gets freed.
	BITMAPINFO *pBmpInfo = new BITMAPINFO;
	::ZeroMemory( pBmpInfo, sizeof(BITMAPINFO) );

	pBmpInfo->bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
	pBmpInfo->bmiHeader.biWidth        = bm.bmWidth;
	pBmpInfo->bmiHeader.biHeight    = bm.bmHeight ;
	pBmpInfo->bmiHeader.biPlanes    = 1;
	pBmpInfo->bmiHeader.biBitCount    = 24;

	// This calculation was taken from Ivaylo Byalkov's article about smooth bitmap scaling.
	pBmpInfo->bmiHeader.biSizeImage    = ((3 * bm.bmWidth + 3) & ~3) * bm.bmHeight;

	// Don't try to use a NULL pointer.
	if ( NULL != pBmpInfo )
	{
		// Allocate a buffer to store the device independent bits from the source bitmap.
		BYTE *pData = new BYTE[pBmpInfo->bmiHeader.biSizeImage];

		if ( NULL != pData )
		{
			// Get the device independent bits from the source bitmap.
			::GetDIBits( hDC, (HBITMAP)bmpSrc, 0, bm.bmHeight, pData, pBmpInfo, DIB_RGB_COLORS );

			// Make sure that the destination bitmap is empty.
			bmpDest.DeleteObject();

			// Create a temporary bitmap handle with the same dimensions of the source bitmap.
			HBITMAP hTmpBmp = ::CreateCompatibleBitmap( hDC, bm.bmWidth, bm.bmHeight );

			// Set the device independent bits from the source bitmap into the temporary bitmap handle.
			::SetDIBits( hDC, hTmpBmp, 0, bm.bmHeight, pData, pBmpInfo, DIB_RGB_COLORS );

			// Detach the old bitmap.
			bmpDest.Detach();

			// Attach the temporary bitmap handle to the destination bitmap.
			bmpDest.Attach( hTmpBmp );

			// Set the dimensions of the bitmap so that they can be used later.
			// Otherwise, they will be invalid.
			bmpDest.SetBitmapDimension( bm.bmWidth, bm.bmHeight );

			// Free the buffer used to store the bits.
			delete [] pData;

			// The bitmap was successfully copied.
			bResult = true;
		}

		// Free the memory for the bitmap info structure.
		delete pBmpInfo;
	}

	// Release the device context.
	::ReleaseDC( NULL, hDC );

	return bResult;
}



void CArtist2DDlg::DrawBitmapAndRelease(CDC& dc, CBitmap** bitmap, int x1, int y1, int x2, int y2)
{
	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(&dc);

	CBitmap *memBitmap = *bitmap, *oldmemBitmap;

	oldmemBitmap = memDC.SelectObject(memBitmap);
	dc.BitBlt(x1, y1, x2 - x1, y2 - y1, &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(oldmemBitmap);
	delete *bitmap;
	*bitmap = NULL;
}
void CArtist2DDlg::ReleaseBitmap(CBitmap** bitmap)
{
	if(*bitmap != NULL)
	{
		delete *bitmap;
		*bitmap = NULL;
	}
}

// 工具栏相关
void CArtist2DDlg::DrawToolBoxPanel(CPaintDC& dc)
{
	CPen* backupPen = dc.GetCurrentPen();
	int x1 = ToolBoxX1 - 10, y1 = ToolBoxY1 - 10, x2 = ToolBoxX2 + 10, y2 = ToolBoxY2 + 10;
	{
		CPen newPen(PS_COSMETIC, 1, RGB(0x33, 0x33, 0x33));
		CPen * pOldPen = dc.SelectObject (&newPen);
		dc.MoveTo(x1, y2);
		dc.LineTo(x1, y1);
		dc.LineTo(x2, y1);
	}
	{
		CPen newPen(PS_COSMETIC, 1, RGB(0xff, 0xff, 0xff));
		CPen * pOldPen = dc.SelectObject (&newPen);
		dc.MoveTo(x2, y1);
		dc.LineTo(x2, y2);
		dc.LineTo(x1, y2);
	}
	dc.SelectObject(backupPen);
	//dc.Rectangle(ToolBoxX1 - 10, ToolBoxY1 - 10, ToolBoxX2 + 10, ToolBoxY2 + 10);
}
void CArtist2DDlg::DrawToolBoxItems(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	int x = ToolBoxX1, y = ToolBoxY1;
	for(int i = 0, j = 0; i < 8; i++, j = 1- j)
	{
		CBitmap bmpBackground;
		BITMAP bitmap;
		bmpBackground.LoadBitmap(
			curTool == i ? ToolBoxItemDown[i] : ToolBoxItemUp[i]);
		bmpBackground.GetBitmap(&bitmap);
		CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(x, y, ToolItemWidth, ToolItemWidth, &dcMem, 0, 0,
			ToolItemWidth, ToolItemWidth, SRCCOPY);

		if(j){
			x -= ToolItemWidth;
			y += ToolItemWidth;
		}else
		{
			x += ToolItemWidth;
		}
	}
}

// 颜色选择相关
void CArtist2DDlg::DrawColorButtonPanel(CPaintDC& dc)
{
	CPen* backupPen = dc.GetCurrentPen();
	int x1 = ColorX1 - 10, y1 = ColorY1 - 10, x2 = ColorX2 + 10, y2 = ColorY2 + 10;
	{
		CPen newPen(PS_COSMETIC, 1, RGB(0x33, 0x33, 0x33));
		CPen * pOldPen = dc.SelectObject (&newPen);
		dc.MoveTo(x1, y2);
		dc.LineTo(x1, y1);
		dc.LineTo(x2, y1);
	}
	{
		CPen newPen(PS_COSMETIC, 1, RGB(0xff, 0xff, 0xff));
		CPen * pOldPen = dc.SelectObject (&newPen);
		dc.MoveTo(x2, y1);
		dc.LineTo(x2, y2);
		dc.LineTo(x1, y2);
	}
	dc.SelectObject(backupPen);
}
void CArtist2DDlg::DrawColorButtons(CPaintDC& dc)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	
	{
		CBitmap bmpBackground;
		BITMAP bitmap;
		bmpBackground.LoadBitmap(curColor == 0 ? IDB_BITMAP23 : IDB_BITMAP24);
		bmpBackground.GetBitmap(&bitmap);
		CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(ColorX1, ColorY1, ColorButtonWidth, ColorButtonHeight, &dcMem, 0, 0,
			ColorButtonWidth, ColorButtonHeight, SRCCOPY);
	}
	{
		CBitmap bmpBackground;
		BITMAP bitmap;
		bmpBackground.LoadBitmap(curColor == 1 ? IDB_BITMAP21 : IDB_BITMAP22);
		bmpBackground.GetBitmap(&bitmap);
		CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
		dc.StretchBlt(ColorX1, ColorY1 + ColorButtonHeight, ColorButtonWidth, ColorButtonHeight, &dcMem, 0, 0,
			ColorButtonWidth, ColorButtonHeight, SRCCOPY);
	}
}
void CArtist2DDlg::DrawColorButtonColors(CPaintDC& dc)
{
	{
		CRect rect(ColorX1 + 25, ColorY1 + 6, ColorX1 + 39, ColorY1 + 17);
		CBrush Brush(LineColor);
		dc.FillRect(&rect, &Brush);
	}
	{
		CRect rect(ColorX1 + 25, ColorY1 + ColorButtonHeight + 6, ColorX1 + 39, ColorY1 + ColorButtonHeight + 17);
		CBrush Brush(FillColor);
		dc.FillRect(&rect, &Brush);
	}
}
void CArtist2DDlg::DrawColorSelectorPanel(CPaintDC& dc)
{
	{
		CRect rect(ColorSelectorPanelX1, ColorSelectorPanelY1, ColorSelectorPanelX2, ColorSelectorPanelY2);
		CBrush Brush(RGB(0xee, 0xee, 0xee));
		dc.FillRect(&rect, &Brush);
	}
	CPen* backupPen = dc.GetCurrentPen();
	{
		CPen newPen(PS_COSMETIC, 1, RGB(0xf5, 0xf5, 0xf5));
		CPen * pOldPen = dc.SelectObject (&newPen);
		dc.MoveTo(ColorSelectorPanelX1, ColorSelectorPanelY2);
		dc.LineTo(ColorSelectorPanelX1, ColorSelectorPanelY1);
		dc.LineTo(ColorSelectorPanelX2, ColorSelectorPanelY1);
	}
	{
		CPen newPen(PS_COSMETIC, 1, RGB(0x33, 0x33, 0x33));
		CPen * pOldPen = dc.SelectObject (&newPen);
		dc.MoveTo(ColorSelectorPanelX2, ColorSelectorPanelY1);
		dc.LineTo(ColorSelectorPanelX2, ColorSelectorPanelY2);
		dc.LineTo(ColorSelectorPanelX1, ColorSelectorPanelY2);
	}
	dc.SelectObject(backupPen);
}
void CArtist2DDlg::DrawColorSelectorSliders(CPaintDC& dc)
{
	for(int i = 0; i < 3; i++)
	{
		Statics[i].ShowWindow(SW_SHOW);
		Sliders[i].ShowWindow(SW_SHOW);
		Edits[i].ShowWindow(SW_SHOW);
	}
	COLORREF cr = curColor == 0 ? LineColor : FillColor;
	byte r = GetRValue(cr);
	byte g = GetGValue(cr);
	byte b = GetBValue(cr);
	CString str;
	str.Format("%d", r);
	Edits[0].SetWindowText(str);
	Sliders[0].SetPos(r);
	str.Format("%d", g);
	Edits[1].SetWindowText(str);
	Sliders[1].SetPos(g);
	str.Format("%d", b);
	Edits[2].SetWindowText(str);
	Sliders[2].SetPos(b);
}


// 画布相关
void CArtist2DDlg::DrawCanvas(CPaintDC& dc)
{
	dc.Rectangle(CanvasX1, CanvasY1, CanvasX2, CanvasY2);
}

// 当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CArtist2DDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
// 事件相关
void CArtist2DDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	/************************************************************************/
	/* 正在选择颜色                                                                    */
	/************************************************************************/
	if(ChoosingColor)
	{
		if(!HitColorSelector(point))
		{
			for(int i = 0; i < 3; i++)
			{
				Statics[i].ShowWindow(FALSE);
				Sliders[i].ShowWindow(FALSE);
				Edits[i].ShowWindow(FALSE);
			}
			curColor = -1;
			ChoosingColor = FALSE;
			RecoverCanvasPart = TRUE;
			NewColorButtons = TRUE;
			NewColorButtonColors = TRUE;
			InvalidateRect(CRect(ColorSelectorPanelX1 - 1, ColorSelectorPanelY1 - 1, ColorSelectorPanelX2 + 1, ColorSelectorPanelY2 + 1), FALSE);
			InvalidateRect(CRect(ColorX1, ColorY1, ColorX2, ColorY2), FALSE);
		}
	}
	/************************************************************************/
	/* 击中工具按钮                                                                    */
	/************************************************************************/
	else if (HitTool(point))
	{
		int oldTool = curTool;

		switch (oldTool)
		{
		case POLYGON:
			{
				OnRButtonDown(0, CPoint(CanvasX1 + 1, CanvasY1 + 1));
				OnRButtonDblClk(0, CPoint(0, 0));
				MyPolygon = NULL;
				break;
			}
		case ROTATOR:
			{
				RotationStatus = NONEROTATION;
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				break;
			}
		case SCALER:
			{
				ScalingStatus = NONESCALING;
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				break;
			}
		default:
			break;
		}

		curTool = 2 * ((point.y - ToolBoxY1) / ToolItemWidth) + (point.x - ToolBoxX1) / ToolItemWidth;

		switch (curTool)
		{
		case POLYGON:
			{
				if(MyPolygon != NULL)
					ClearUpPolygon(MyPolygon);
				MyPolygon = NULL;
				break;
			}
		case FILLER:
			{
				curTool = oldTool;
				if(PolygonList->GetCurSel() == LB_ERR)
				{
					AfxMessageBox("目前没有选定多边形");
					break;
				}
				if(!PolygonList->GetCheck(PolygonList->GetCurSel()))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				ComplexPolygon* polygon = Polygons.GetAt(Polygons.FindIndex(PolygonList->GetCurSel()));
				polygon->FilledColor = FillColor;
				polygon->Filled = !polygon->Filled;

				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				
				break;
			}
		case ERASER:
			{
				curTool = oldTool;
				int index = PolygonList->GetCurSel();
				if(index == LB_ERR)
				{
					AfxMessageBox("目前没有选定多边形。");
					break;
				}

				CString StrIndex;
				PolygonList->GetText(index, StrIndex);
				if(AfxMessageBox("确定要删除多边形" + StrIndex, MB_YESNO) == IDYES)
				{
					RemovePolygon(index);
					NeedToDrawAllPolygons = TRUE;
					InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				}

				break;
			}
		case ROTATOR:
			{
				CClientDC dc(this);
				GetClientRect(ClientRect);
				if(BackupCanvasBeforeRotating != NULL)
					ReleaseBitmap(&BackupCanvasBeforeRotating);
				BackupCanvasBeforeRotating = GetBitmap(dc , 0, 0, ClientRect.Width(), ClientRect.Height());

				int index = PolygonList->GetCurSel();
				if(index == LB_ERR)
				{
					break;
				}

				if(!PolygonList->GetCheck(index))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				if(RotatedPolygon != NULL)
					ClearUpPolygon(RotatedPolygon);
				RotatedPolygon = DuplicatePolygon(Polygons.GetAt(Polygons.FindIndex(index)));
				RotatePolygonID = index;
				RotateLT.x = RotateLB.x = RotatedPolygon->MinX;
				RotateLT.y = RotateRT.y = RotatedPolygon->MinY;
				RotateRT.x = RotateRB.x = RotatedPolygon->MaxX;
				RotateRB.y = RotateLB.y = RotatedPolygon->MaxY;
				BackupRotateLB = RotateLB;
				BackupRotateLT = RotateLT;
				BackupRotateRB = RotateRB;
				BackupRotateRT = RotateRT;
				RotationCenter = CPoint((RotatedPolygon->MinX + RotatedPolygon->MaxX) >> 1, (RotatedPolygon->MinY + RotatedPolygon->MaxY) >> 1);
				NeedToDrawFourAngles = TRUE;
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				break;
			}
		case SCALER:
			{
				CClientDC dc(this);
 				GetClientRect(ClientRect);
				if(BackupCanvasBeforeScaling != NULL)
					ReleaseBitmap(&BackupCanvasBeforeScaling);
				BackupCanvasBeforeScaling = GetBitmap(dc , 0, 0, ClientRect.Width(), ClientRect.Height());

				int index = PolygonList->GetCurSel();
				if(index == LB_ERR)
				{
					break;
				}

				if(!PolygonList->GetCheck(index))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				if(ScaledPolygon != NULL)
					ClearUpPolygon(ScaledPolygon);
				ScaledPolygon = DuplicatePolygon(Polygons.GetAt(Polygons.FindIndex(index)));
				ScalePolygonID = index;
				Scale_7.x = Scale_4.x = Scale_1.x = ScaledPolygon->MinX;
				Scale_7.y = Scale_8.y = Scale_9.y = ScaledPolygon->MinY;
				Scale_3.x = Scale_6.x = Scale_9.x = ScaledPolygon->MaxX;
				Scale_1.y = Scale_2.y = Scale_3.y = ScaledPolygon->MaxY;
				Scale_2.x = Scale_8.x = (ScaledPolygon->MinX + ScaledPolygon->MaxX) >> 1;
				Scale_4.y = Scale_6.y = (ScaledPolygon->MinY + ScaledPolygon->MaxY) >> 1;
				BackupScale_1 = Scale_1;
				BackupScale_2 = Scale_2;
				BackupScale_3 = Scale_3;
				BackupScale_4 = Scale_4;
				BackupScale_6 = Scale_6;
				BackupScale_7 = Scale_7;
				BackupScale_8 = Scale_8;
				BackupScale_9 = Scale_9;

				RotationCenter = CPoint((ScaledPolygon->MinX + ScaledPolygon->MaxX) >> 1, (ScaledPolygon->MinY + ScaledPolygon->MaxY) >> 1);
				NeedToDrawEightBorders = TRUE;
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				break;
			}
		case OVERTURNER:
			{
				if(!PolygonList->GetCheck(PolygonList->GetCurSel()))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				int index = PolygonList->GetCurSel();
				OverturnedPolygonID = index;
				if(index == LB_ERR)
				{
					break;
				}
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				break;
			}
		case CLIPPER:
			{
				CString StrClippedID = "-1", StrClippingID = "-1";
				GetClippingInformation(&StrClippedID, &StrClippingID);
				if(StrClippedID == "-1" || StrClippingID == "-1")
				{
					AfxMessageBox("输入的多边形编号有误");
					curTool = DRAGGER;
					break;
				}

				int id_1 = PolygonList->FindString(-1, StrClippedID);
				int id_2 = PolygonList->FindString(-1, StrClippingID);
				if(id_1 == LB_ERR)
				{
					AfxMessageBox("不存在编号为\"" + StrClippedID + "\"的多边形。");
					curTool = DRAGGER;
					break;
				}
				if(id_2 == LB_ERR)
				{
					AfxMessageBox("不存在编号为\"" + StrClippingID + "\"的多边形。");
					curTool = DRAGGER;
					break;
				}

				if(!PolygonList->GetCheck(id_1) || !PolygonList->GetCheck(id_2))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				ComplexPolygon* p1 = Polygons.GetAt(Polygons.FindIndex(id_1));
				ComplexPolygon* p2 = Polygons.GetAt(Polygons.FindIndex(id_2));
				if(CheckPolygonCrossed(p1))
				{
					AfxMessageBox("编号为\"" + StrClippedID + "\"的多边形自相交，无法参与裁剪。");
					curTool = DRAGGER;
					break;
				}
				if(CheckPolygonCrossed(p2))
				{
					AfxMessageBox("编号为\"" + StrClippingID + "\"的多边形自相交，无法参与裁剪。");
					curTool = DRAGGER;
					break;
				}
				InsertPolygon(ClipPolygon(p1, p2));
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);

				curTool = DRAGGER;
				break;
			}
		default:
			break;
		}
		

		NewToolBoxItems = TRUE;
		InvalidateRect(CRect(ToolBoxX1, ToolBoxY1, ToolBoxX2, ToolBoxY2), FALSE);
	}
	/************************************************************************/
	/* 击中颜色按钮                                                                    */
	/************************************************************************/
	else if(HitColor(point))
	{
		curColor = (point.y - ColorY1) / ColorButtonHeight;
		NewColorButtons = TRUE;
		NewColorButtonColors = TRUE;

		ChoosingColor = TRUE;
		NewColorSelectorPanel = ChoosingColor;
		NewColorSelectorSliders = ChoosingColor;
		
		BackupCanvasPart = TRUE;

		InvalidateRect(CRect(ColorX1, ColorY1, ColorX2, ColorY2), FALSE);
		InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
	/************************************************************************/
	/* 正在使用工具                                                                 */
	/************************************************************************/
	}else if(HitCanvas(point))
	{

		CClientDC dc(this);
		GetClientRect(ClientRect);

		switch (curTool)
		{
		case POLYGON:
			{
				if(BeginPolygon)
				{
					if(Polygons.IsEmpty())
					{
						ReleaseBitmap(&WhiteCanvas);
						WhiteCanvas = GetBitmap(dc, 0, 0, ClientRect.Width(), ClientRect.Height());
					}
					MyPolygon = new ComplexPolygon();
					BackupCanvasBeforePolygon = GetBitmap(dc , 0, 0, ClientRect.Width(), ClientRect.Height());
				}
				if(BeginPolygonCycle)
				{
					PolygonCycle* NewPolygonCycle = new PolygonCycle();
					PolygonPoint* NewPolygonPoint = new PolygonPoint(point, VERTEX);

					NewPolygonCycle->LineColor = LineColor;
					NewPolygonCycle->InsertToTail(NewPolygonPoint);
					MyPolygon->InsertToTail(NewPolygonCycle);

					ReleaseBitmap(&BackupCanvasForEachPolygonLine);
					BackupCanvasForEachPolygonLine = GetBitmap(dc , 0, 0, ClientRect.Width(), ClientRect.Height());
					BeginPoint = EndPoint = HeadPoint = point;
					BeginPolygonCycle = FALSE;
					BeginPolygon = FALSE;
				}
				else
				{
					MyPolygon->GetTail()->InsertToTail(new PolygonPoint(point, VERTEX));
					
					StoredPoint = point;
					NeedToDrawPolygonAfterClicked = TRUE;
					InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
				}
				break;
			}
		case DRAGGER:
			{
				DragPolygonID = PolygonList->GetCurSel();
				if(!PolygonList->GetCheck(DragPolygonID))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				if(DragPolygonID == LB_ERR)
				{
					AfxMessageBox("当前没有选定任何多边形。");
					break;
				}
				Dragging = TRUE;
				ComplexPolygon* polygon = Polygons.GetAt(Polygons.FindIndex(DragPolygonID));
				OriginalFilled = polygon->Filled;
				polygon->Filled = FALSE;
				DragFrom = point;
				break;
			}
		case ROTATOR:
			{
				if(!PolygonList->GetCheck(PolygonList->GetCurSel()))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				switch (RotationStatus)
				{
				case NONEROTATION:
					{
						if(DistanceBetween(CPoint(RotationCenter), point) < CenterRadius)
						{
							RotationStatus = CENTERINGROTATION;
							CenterFrom = point;
						}else if(DistanceBetween(RotateLB, point) < CenterRadius
							|| DistanceBetween(RotateLT, point) < CenterRadius
							|| DistanceBetween(RotateRB, point) < CenterRadius
							|| DistanceBetween(RotateRT, point) < CenterRadius)
						{
							RotationStatus = ROTATING;
							OriginalFilled = RotatedPolygon->Filled;
							RotatedPolygon->Filled = FALSE;
							RotateFrom = point;
						}
						break;
					}
				case CENTERINGROTATION:
					{
						break;
					}
				case ROTATING:
					{
						break;
					}
				default:
					break;
				}
				break;
			}
		case SCALER:
			{
				if(!PolygonList->GetCheck(PolygonList->GetCurSel()))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				switch (ScalingStatus)
				{
				case NONESCALING:
					{
						if(DistanceBetween(CPoint(RotationCenter), point) < CenterRadius)
						{
							ScalingStatus = CENTERINGSCALING;
							CenterFrom = point;
						}else if(DistanceBetween(Scale_7, point) < CenterRadius
							|| DistanceBetween(Scale_9, point) < CenterRadius
							|| DistanceBetween(Scale_1, point) < CenterRadius
							|| DistanceBetween(Scale_3, point) < CenterRadius)
						{
							ScalingFormat = BOTHSCALING;
							ScalingStatus = SCALING;
							OriginalFilled = ScaledPolygon->Filled;
							ScaledPolygon->Filled = FALSE;
							ScaleFrom = point;
						}else if(DistanceBetween(Scale_8, point) < CenterRadius
							|| DistanceBetween(Scale_2, point) < CenterRadius)
						{
							ScalingFormat = VERTICALSCALING;
							ScalingStatus = SCALING;
							OriginalFilled = ScaledPolygon->Filled;
							ScaledPolygon->Filled = FALSE;
							ScaleFrom = point;
						}else if(DistanceBetween(Scale_4, point) < CenterRadius
							|| DistanceBetween(Scale_6, point) < CenterRadius)
						{
							ScalingFormat = HORIZONTALSCALING;
							ScalingStatus = SCALING;
							OriginalFilled = ScaledPolygon->Filled;
							ScaledPolygon->Filled = FALSE;
							ScaleFrom = point;
						}
						break;
					}
				case CENTERINGSCALING:
					{
						break;
					}
				case SCALING:
					{
						break;
					}
				default:
					break;
				}
				break;
			}
		case OVERTURNER:
			{
				if(!PolygonList->GetCheck(PolygonList->GetCurSel()))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				if(OverturnedPolygonID == LB_ERR)
				{
					AfxMessageBox("当前没有选择多边形。");
					break;
				}
				TurnOverPolygon(Polygons.GetAt(Polygons.FindIndex(OverturnedPolygonID)), TRUE);
				RefreshMinMaxXY(Polygons.GetAt(Polygons.FindIndex(OverturnedPolygonID)));
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
				break;
			}
		default:
			break;
		} 
	}

	CDialog::OnLButtonDown(nFlags, point);
}
void CArtist2DDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(MouseInside = HitCanvas(point))
	{
		switch (curTool)
		{
		case POLYGON:
			{
				if(!BeginPolygonCycle){
					EndPoint = point;
					NeedToDrawPolygonWhileMoving = TRUE;
					InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
				}
				break;
			}
		case DRAGGER:
			{
				if(Dragging)
				{
					DragTo = point;
					ComplexPolygon* polygon = Polygons.GetAt(Polygons.FindIndex(DragPolygonID));
					DragPolygon(polygon, DragFrom, DragTo);
					polygon->MinX += DragTo.x - DragFrom.x;
					polygon->MinY += DragTo.y - DragFrom.y;
					polygon->MaxX += DragTo.x - DragFrom.x;
					polygon->MaxY += DragTo.y - DragFrom.y;
					DragFrom = point;
					NeedToDrawAllPolygons = TRUE;
					InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
				}
				break;
			}
		case ROTATOR:
			{
				switch (RotationStatus)
				{
				case NONEROTATION:
					{
						break;
					}
				case CENTERINGROTATION:
					{
						CenterTo = point;
						RotationCenter += CenterTo - CenterFrom;
						CenterFrom = point;
						NeedToDrawFourAngles = TRUE;
						InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
						break;
					}
				case ROTATING:
					{
						RotateTo = point;
						double alpha = GetFromTanTheta(RotateFrom - RotationCenter);
						double beta = GetFromTanTheta(RotateTo - RotationCenter);
						double angle = - alpha + beta;
						ComplexPolygon* ActorPolygon = Polygons.GetAt(Polygons.FindIndex(RotatePolygonID));
						ClearUpPolygon(ActorPolygon);
						Polygons.SetAt(Polygons.FindIndex(RotatePolygonID), DuplicatePolygon(RotatedPolygon));
						ActorPolygon = Polygons.GetAt(Polygons.FindIndex(RotatePolygonID));
						RotatePolygon(ActorPolygon, RotationCenter, angle);
						RotateLB = BackupRotateLB;
						RotateLT = BackupRotateLT;
						RotateRB = BackupRotateRB;
						RotateRT = BackupRotateRT;
						RotateLB -= RotationCenter;
						RotateLT -= RotationCenter;
						RotateRB -= RotationCenter;
						RotateRT -= RotationCenter;
						PerformRotation(&RotateLB, angle);
						PerformRotation(&RotateLT, angle);
						PerformRotation(&RotateRB, angle);
						PerformRotation(&RotateRT, angle);
						RotateLB += RotationCenter;
						RotateLT += RotationCenter;
						RotateRB += RotationCenter;
						RotateRT += RotationCenter;
						NeedToDrawAllPolygons = TRUE;
						NeedToDrawFourAnglesWhileRotating = TRUE;
						InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
						break;
					}
				default:
					break;
				}
				break;
			}
		case SCALER:
			{
				switch (ScalingStatus)
				{
				case NONESCALING:
					{
						break;
					}
				case CENTERINGSCALING:
					{
						CenterTo = point;
						RotationCenter += CenterTo - CenterFrom;
						CenterFrom = point;
						NeedToDrawEightBorders = TRUE;
						InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
						break;
					}
				case SCALING:
					{
						ScaleTo = point;
						ComplexPolygon* ActorPolygon = Polygons.GetAt(Polygons.FindIndex(ScalePolygonID));
						ClearUpPolygon(ActorPolygon);
						Polygons.SetAt(Polygons.FindIndex(ScalePolygonID), DuplicatePolygon(ScaledPolygon));
						ActorPolygon = Polygons.GetAt(Polygons.FindIndex(ScalePolygonID));
						double sx = ScalingFormat == VERTICALSCALING ? 1.0 : 1.0 * (ScaleTo.x - RotationCenter.x) / (ScaleFrom.x - RotationCenter.x);
						double sy = ScalingFormat == HORIZONTALSCALING ? 1.0 : 1.0 * (ScaleTo.y - RotationCenter.y) / (ScaleFrom.y - RotationCenter.y);
						ScalePolygon(ActorPolygon, RotationCenter, sx, sy);
						Scale_1 = BackupScale_1;
						Scale_2 = BackupScale_2;
						Scale_3 = BackupScale_3;
						Scale_4 = BackupScale_4;
						Scale_6 = BackupScale_6;
						Scale_7 = BackupScale_7;
						Scale_8 = BackupScale_8;
						Scale_9 = BackupScale_9;

						Scale_1 -= RotationCenter;
						Scale_2 -= RotationCenter;
						Scale_3 -= RotationCenter;
						Scale_4 -= RotationCenter;
						Scale_6 -= RotationCenter;
						Scale_7 -= RotationCenter;
						Scale_8 -= RotationCenter;
						Scale_9 -= RotationCenter;

						PerformScaling(&Scale_1, sx, sy);
						PerformScaling(&Scale_2, sx, sy);
						PerformScaling(&Scale_3, sx, sy);
						PerformScaling(&Scale_4, sx, sy);
						PerformScaling(&Scale_6, sx, sy);
						PerformScaling(&Scale_7, sx, sy);
						PerformScaling(&Scale_8, sx, sy);
						PerformScaling(&Scale_9, sx, sy);

						Scale_1 += RotationCenter;
						Scale_2 += RotationCenter;
						Scale_3 += RotationCenter;
						Scale_4 += RotationCenter;
						Scale_6 += RotationCenter;
						Scale_7 += RotationCenter;
						Scale_8 += RotationCenter;
						Scale_9 += RotationCenter;

						NeedToDrawAllPolygons = TRUE;
						NeedToDrawEightBordersWhileScaling = TRUE;
						InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
						break;
					}
				default:
					break;
				}
				break;
			}
		default:
			break;
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CArtist2DDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch (curTool)
	{
	case DRAGGER:
		{
			if(Dragging)
			{
				Dragging = FALSE;
				Polygons.GetAt(Polygons.FindIndex(DragPolygonID))->Filled = OriginalFilled;
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
			}
			break;
		}

	case ROTATOR:
		{
			switch (RotationStatus)
			{
			case NONEROTATION:
				{
					break;
				}
			case CENTERINGROTATION:
				{
					RotationStatus = NONEROTATION;
					break;
				}
			case ROTATING:
				{
					ClearUpPolygon(RotatedPolygon);
					RefreshMinMaxXY(Polygons.GetAt(Polygons.FindIndex(RotatePolygonID)));
					RotatedPolygon = NULL;
					RotationStatus = NONEROTATION;
					NeedToDrawAllPolygons = TRUE;
					NeedToDrawFourAnglesWhileRotating = FALSE;
					Polygons.GetAt(Polygons.FindIndex(RotatePolygonID))->Filled = OriginalFilled;
					InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);

					curTool = DRAGGER;
					NewToolBoxItems = TRUE;
					InvalidateRect(CRect(ToolBoxX1, ToolBoxY1, ToolBoxX2, ToolBoxY2), FALSE);
					break;
				}
			default:
				break;
			}
		}

	case SCALER:
		{
			switch (ScalingStatus)
			{
			case NONESCALING:
				{
					break;
				}
			case CENTERINGSCALING:
				{
					ScalingStatus = NONESCALING;
					break;
				}
			case SCALING:
				{
					ClearUpPolygon(ScaledPolygon);
					RefreshMinMaxXY(Polygons.GetAt(Polygons.FindIndex(ScalePolygonID)));
					ScaledPolygon = NULL;
					ScalingStatus = NONESCALING;
					NeedToDrawAllPolygons = TRUE;
					NeedToDrawEightBordersWhileScaling = FALSE;
					Polygons.GetAt(Polygons.FindIndex(ScalePolygonID))->Filled = OriginalFilled;
					InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);

					curTool = DRAGGER;
					NewToolBoxItems = TRUE;
					InvalidateRect(CRect(ToolBoxX1, ToolBoxY1, ToolBoxX2, ToolBoxY2), FALSE);
					break;
				}

			default:
				break;
			}
			break;
		}
	default:
		break;
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CArtist2DDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(HitCanvas(point))
	{
		switch (curTool)
		{
		case POLYGON:
			{
				if(!BeginPolygon)
				{
					if(MyPolygon->GetTail()->GetCount() < 3)
					{
						PolygonCycle* cycle = MyPolygon->GetTail();
						for(int i = 0; i < cycle->GetCount(); i++)
						{
							PolygonPoint* point = cycle->GetPoint(i);
							delete point;
						}
						delete cycle;
						MyPolygon->Remove(MyPolygon->GetCount() - 1);

						NeedToDrawAllPolygons = TRUE;
						InvalidateRect(CRect(CPoint(CanvasX1, CanvasY1), CPoint(CanvasX2, CanvasY2)), FALSE);
						BeginPolygonCycle = TRUE;
						break;
					}
					EndPoint = HeadPoint;
					NeedToDrawPolygonAfterClicked = TRUE;
					InvalidateRect(CRect(CPoint(CanvasX1, CanvasY1), CPoint(CanvasX2, CanvasY2)), FALSE);
					BeginPolygonCycle = TRUE;
				}
				break;
			}
		case OVERTURNER:
			{
				if(OverturnedPolygonID == LB_ERR)
				{
					AfxMessageBox("当前没有选择多边形。");
					break;
				}
				if(!PolygonList->GetCheck(OverturnedPolygonID))
				{
					AfxMessageBox("选定的多边形不可见。");
					break;
				}
				TurnOverPolygon(Polygons.GetAt(Polygons.FindIndex(OverturnedPolygonID)), FALSE);
				RefreshMinMaxXY(Polygons.GetAt(Polygons.FindIndex(OverturnedPolygonID)));
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
				break;
			}
		default:
			break;
		}
	}

	CDialog::OnRButtonDown(nFlags, point);
}
void CArtist2DDlg::OnRButtonDblClk(UINT nFlags, CPoint point)
{
		switch (curTool)
		{
		case POLYGON:
			{
				if(!BeginPolygon){
					if(MyPolygon->GetCount() < 1)
						break;
					MyPolygon->Filled = FALSE;
					FormalizePolygon(MyPolygon);
					InsertPolygon(MyPolygon);
					MyPolygon = NULL;

					ReleaseBitmap(&BackupCanvasBeforePolygon);
					BeginPolygon = TRUE;
					NeedToDrawAllPolygons = TRUE;
					InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
				}
				break;
			}

		default:
			break;
		}
	CDialog::OnRButtonDblClk(nFlags, point);
}

void CArtist2DDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	this->GetClientRect(rect);

	NewToolBoxPanel = TRUE;
	NewToolBoxItems = TRUE;

	NewColorButtonPanel = TRUE;
	NewColorButtons = TRUE;
	NewColorButtonColors = TRUE;

	NewColorSelectorPanel = ChoosingColor;
	NewColorSelectorSliders = ChoosingColor;

	NeedToDrawAllPolygons = TRUE;
	InvalidateRect(rect, FALSE);
}

void CArtist2DDlg::OnClose()
{
	delete[] ToolBoxItemUp;
	delete[] ToolBoxItemDown;
	delete[] Sliders;
	delete[] Statics;
	delete[] Edits;
	PolygonList->ResetContent();
	delete[] PolygonList;
	delete[] LayerUp;
	delete[] LayerDown;

	if(MyPolygon != NULL)
		ClearUpPolygon(MyPolygon);
	MyPolygon = NULL;
	int PolygonAmount = Polygons.GetCount();
	for(int i = PolygonAmount - 1; i >= 0; i--)
	{
		ClearUpPolygon(Polygons.GetAt(Polygons.FindIndex(i)));
	}


	ReleaseBitmap(&BackupCanvasBeforePolygon);
	ReleaseBitmap(&BackupCanvasForEachPolygonLine);
	ReleaseBitmap(&WhiteCanvas);
	CDialog::OnClose();
}

// BOOL CArtist2DDlg::PreTranslateMessage(MSG* pMsg)   
// { 
// 	if(pMsg-> message==WM_NCLBUTTONDOWN) 
// 		return   1; 
// 
// 	return   CDialog::PreTranslateMessage(pMsg); 
// } 

// 改变颜色
void CArtist2DDlg::OnEnChangeR()
{
	int r;
	CString str;
	GetDlgItem(3000)->GetWindowText(str);

	for(int i = 0; i < str.GetLength(); i++)
	{
		char c = str.GetAt(i);
		if(c < '0' || c > '9')
			GetDlgItem(3000)->SetWindowText("0");
	}

	if(str.GetLength() > 3)
	{
		int i = 0;
		for(; i < str.GetLength() && str.GetAt(i) == '0'; i++);
		str = str.Mid(i);
		str = str.GetLength() == 0 ? "0" : str;
		GetDlgItem(3000)->SetWindowText(atoi(str) > 255 ? "255" : str);
	}

	GetDlgItem(3000)->GetWindowText(str);
	r = atoi(str);
	Sliders[0].SetPos(atoi(str));
	COLORREF cr = curColor == 0 ? LineColor : FillColor;
	cr = RGB(r, GetGValue(cr), GetBValue(cr));
	(curColor == 0 ? LineColor : FillColor) = cr;
	NewColorButtonColors = true;
	curColor == 0 ? 
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + 6, ColorX1 + 39, ColorY1 + 17), FALSE) :
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + ColorButtonHeight + 6, ColorX1 + 39, ColorY1 + ColorButtonHeight + 17), FALSE);
}

void CArtist2DDlg::OnEnChangeG()
{
	int g;
	CString str;
	GetDlgItem(3001)->GetWindowText(str);

	for(int i = 0; i < str.GetLength(); i++)
	{
		char c = str.GetAt(i);
		if(c < '0' || c > '9')
			GetDlgItem(3001)->SetWindowText("0");
	}

	if(str.GetLength() > 3)
	{
		int i = 0;
		for(; i < str.GetLength() && str.GetAt(i) == '0'; i++);
		str = str.Mid(i);
		str = str.GetLength() == 0 ? "0" : str;
		GetDlgItem(3001)->SetWindowText(atoi(str) > 255 ? "255" : str);
	}

	GetDlgItem(3001)->GetWindowText(str);
	g = atoi(str);
	Sliders[1].SetPos(atoi(str));
	COLORREF cr = curColor == 0 ? LineColor : FillColor;
	cr = RGB(GetRValue(cr), g, GetBValue(cr));
	(curColor == 0 ? LineColor : FillColor) = cr;
	NewColorButtonColors = true;
	curColor == 0 ? 
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + 6, ColorX1 + 39, ColorY1 + 17), FALSE) :
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + ColorButtonHeight + 6, ColorX1 + 39, ColorY1 + ColorButtonHeight + 17), FALSE);
}

void CArtist2DDlg::OnEnChangeB()
{
	int b;
	CString str;
	GetDlgItem(3002)->GetWindowText(str);

	for(int i = 0; i < str.GetLength(); i++)
	{
		char c = str.GetAt(i);
		if(c < '0' || c > '9')
			GetDlgItem(3002)->SetWindowText("0");
	}

	if(str.GetLength() > 3)
	{
		int i = 0;
		for(; i < str.GetLength() && str.GetAt(i) == '0'; i++);
		str = str.Mid(i);
		str = str.GetLength() == 0 ? "0" : str;
		GetDlgItem(3002)->SetWindowText(atoi(str) > 255 ? "255" : str);
	}

	GetDlgItem(3002)->GetWindowText(str);
	b = atoi(str);
	Sliders[2].SetPos(atoi(str));
	COLORREF cr = curColor == 0 ? LineColor : FillColor;
	cr = RGB(GetRValue(cr), GetGValue(cr), b);
	(curColor == 0 ? LineColor : FillColor) = cr;
	NewColorButtonColors = true;
	curColor == 0 ? 
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + 6, ColorX1 + 39, ColorY1 + 17), FALSE) :
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + ColorButtonHeight + 6, ColorX1 + 39, ColorY1 + ColorButtonHeight + 17), FALSE);
}

void CArtist2DDlg::OnNMCustomdrawR(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int r = Sliders[0].GetPos();
	CString str;
	str.Format("%d", r);
	GetDlgItem(3000)->SetWindowText(str);
	COLORREF cr = (curColor == 0 ? LineColor : FillColor);
	cr = RGB(r, GetGValue(cr), GetBValue(cr));
	(curColor == 0 ? LineColor : FillColor) = cr;
	NewColorButtonColors = true;
	curColor == 0 ? 
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + 6, ColorX1 + 39, ColorY1 + 17), FALSE) :
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + ColorButtonHeight + 6, ColorX1 + 39, ColorY1 + ColorButtonHeight + 17), FALSE);
	*pResult = 0;
}
void CArtist2DDlg::OnNMCustomdrawG(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int g = Sliders[1].GetPos();
	CString str;
	str.Format("%d", g);
	GetDlgItem(3001)->SetWindowText(str);
	COLORREF cr = (curColor == 0 ? LineColor : FillColor);
	cr = RGB(GetRValue(cr), g, GetBValue(cr));
	(curColor == 0 ? LineColor : FillColor) = cr;
	NewColorButtonColors = true;
	curColor == 0 ? 
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + 6, ColorX1 + 39, ColorY1 + 17), FALSE) :
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + ColorButtonHeight + 6, ColorX1 + 39, ColorY1 + ColorButtonHeight + 17), FALSE);
	*pResult = 0;
}

void CArtist2DDlg::OnNMCustomdrawB(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int b = Sliders[2].GetPos();
	CString str;
	str.Format("%d", b);
	GetDlgItem(3002)->SetWindowText(str);
	COLORREF cr = (curColor == 0 ? LineColor : FillColor);
	cr = RGB(GetRValue(cr), GetGValue(cr), b);
	(curColor == 0 ? LineColor : FillColor) = cr;
	NewColorButtonColors = true;
	curColor == 0 ? 
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + 6, ColorX1 + 39, ColorY1 + 17), FALSE) :
		InvalidateRect(CRect(ColorX1 + 25, ColorY1 + ColorButtonHeight + 6, ColorX1 + 39, ColorY1 + ColorButtonHeight + 17), FALSE);
	*pResult = 0;
}

void CArtist2DDlg::OnVisibleChanged()
{
	int index = PolygonList->GetCurSel();
	Polygons.GetAt(Polygons.FindIndex(index))->visible = PolygonList->GetCheck(index);

	NeedToDrawAllPolygons = TRUE;
	InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
}


void CArtist2DDlg::OnSelPolygonChanged()
{
	if(ChoosingColor)
	{
		for(int i = 0; i < 3; i++)
		{
			Statics[i].ShowWindow(FALSE);
			Sliders[i].ShowWindow(FALSE);
			Edits[i].ShowWindow(FALSE);
		}
		curColor = -1;
		ChoosingColor = FALSE;
		RecoverCanvasPart = TRUE;
		NewColorButtons = TRUE;
		NewColorButtonColors = TRUE;
		InvalidateRect(CRect(ColorSelectorPanelX1 - 1, ColorSelectorPanelY1 - 1, ColorSelectorPanelX2 + 1, ColorSelectorPanelY2 + 1), FALSE);
		InvalidateRect(CRect(ColorX1, ColorY1, ColorX2, ColorY2), FALSE);
	}
	else{
		switch (curTool)
		{
		case ROTATOR:
			{
				int index = PolygonList->GetCurSel();
				if(index == LB_ERR)
				{
					return;
				}
				RotatePolygonID = index;
				if(RotatedPolygon != NULL)
					ClearUpPolygon(RotatedPolygon);
				RotatedPolygon = DuplicatePolygon(Polygons.GetAt(Polygons.FindIndex(index)));
				RotateLT.x = RotateLB.x = RotatedPolygon->MinX;
				RotateLT.y = RotateRT.y = RotatedPolygon->MinY;
				RotateRT.x = RotateRB.x = RotatedPolygon->MaxX;
				RotateRB.y = RotateLB.y = RotatedPolygon->MaxY;
				BackupRotateLB = RotateLB;
				BackupRotateLT = RotateLT;
				BackupRotateRB = RotateRB;
				BackupRotateRT = RotateRT;
				RotationCenter = CPoint((RotatedPolygon->MinX + RotatedPolygon->MaxX) >> 1, (RotatedPolygon->MinY + RotatedPolygon->MaxY) >> 1);
				NeedToDrawFourAngles = TRUE;
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				break;
			}
		case SCALER:
			{
				int index = PolygonList->GetCurSel();
				if(index == LB_ERR)
				{
					return;
				}
				ScalePolygonID = index;
				if(ScaledPolygon != NULL)
					ClearUpPolygon(ScaledPolygon);
				ScaledPolygon = DuplicatePolygon(Polygons.GetAt(Polygons.FindIndex(index)));

				Scale_7.x = Scale_4.x = Scale_1.x = ScaledPolygon->MinX;
				Scale_7.y = Scale_8.y = Scale_9.y = ScaledPolygon->MinY;
				Scale_3.x = Scale_6.x = Scale_9.x = ScaledPolygon->MaxX;
				Scale_1.y = Scale_2.y = Scale_3.y = ScaledPolygon->MaxY;
				Scale_2.x = Scale_8.x = (ScaledPolygon->MinX + ScaledPolygon->MaxX) >> 1;
				Scale_4.y = Scale_6.y = (ScaledPolygon->MinY + ScaledPolygon->MaxY) >> 1;
				BackupScale_1 = Scale_1;
				BackupScale_2 = Scale_2;
				BackupScale_3 = Scale_3;
				BackupScale_4 = Scale_4;
				BackupScale_6 = Scale_6;
				BackupScale_7 = Scale_7;
				BackupScale_8 = Scale_8;
				BackupScale_9 = Scale_9;

				RotationCenter = CPoint((ScaledPolygon->MinX + ScaledPolygon->MaxX) >> 1, (ScaledPolygon->MinY + ScaledPolygon->MaxY) >> 1);
				NeedToDrawEightBorders = TRUE;
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				break;
			}
		case OVERTURNER:
			{
				int index = PolygonList->GetCurSel();
				OverturnedPolygonID = index;
				if(index == LB_ERR)
				{
					break;
				}
				NeedToDrawAllPolygons = TRUE;
				InvalidateRect(CRect(CanvasX1 + 1, CanvasY1 + 1, CanvasX2 - 1, CanvasY2 - 1), FALSE);
				break;
			}
		default:
			break;
		}
	}
}

void CArtist2DDlg::PerformLayerUp()
{
	int index = PolygonList->GetCurSel();
	if(index == LB_ERR)
	{
		AfxMessageBox("当前没有选中多边形。");
		return;
	}
	if(index == 0)
		return;
	ComplexPolygon* polygon = Polygons.GetAt(Polygons.FindIndex(index));
	ComplexPolygon* NewPolygon = DuplicatePolygon(polygon);
	ClearUpPolygon(polygon);
	Polygons.RemoveAt(Polygons.FindIndex(index));
	Polygons.InsertBefore(Polygons.FindIndex(index - 1), NewPolygon);
	CString BackupID;
	int BackupCheck = PolygonList->GetCheck(index);
	PolygonList->GetText(index, BackupID);
	PolygonList->DeleteString(index);
	PolygonList->InsertString(index - 1, BackupID);
	PolygonList->SetCheck(index - 1, BackupCheck);
	PolygonList->SetCurSel(index - 1);
	NeedToDrawAllPolygons = TRUE;
	InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
}
void CArtist2DDlg::PerformLayerDown()
{
	int index = PolygonList->GetCurSel();
	if(index == LB_ERR)
	{
		AfxMessageBox("当前没有选中多边形。");
		return;
	}
	if(index == PolygonList->GetCount() - 1)
		return;
	ComplexPolygon* polygon = Polygons.GetAt(Polygons.FindIndex(index));
	ComplexPolygon* NewPolygon = DuplicatePolygon(polygon);
	ClearUpPolygon(polygon);
	Polygons.RemoveAt(Polygons.FindIndex(index));
	Polygons.InsertAfter(Polygons.FindIndex(index), NewPolygon);
	CString BackupID;
	int BackupCheck = PolygonList->GetCheck(index);
	PolygonList->GetText(index, BackupID);
	PolygonList->DeleteString(index);
	PolygonList->InsertString(index + 1, BackupID);
	PolygonList->SetCheck(index + 1, BackupCheck);
	PolygonList->SetCurSel(index + 1);
	NeedToDrawAllPolygons = TRUE;
	InvalidateRect(CRect(CPoint(CanvasX1 + 1, CanvasY1 + 1), CPoint(CanvasX2 - 1, CanvasY2 - 1)), FALSE);
}
// 图形绘制
void CArtist2DDlg::FillPixel(CDC& dc, int x, int y)
{
	dc.SetPixel(x, y, FillColor);
}
void CArtist2DDlg::DrawPixel(CDC& dc, int x, int y, COLORREF color)
{
	dc.SetPixel(x, y, color);
}
void CArtist2DDlg::DrawCirclePixel(CDC& dc, CPoint MidPoint, int x, int y)
{
	dc.SetPixel(MidPoint.x + x, MidPoint.y + y, LineColor);
	dc.SetPixel(MidPoint.x + x, MidPoint.y - y, LineColor);
	dc.SetPixel(MidPoint.x - x, MidPoint.y + y, LineColor);
	dc.SetPixel(MidPoint.x - x, MidPoint.y - y, LineColor);
	dc.SetPixel(MidPoint.x + y, MidPoint.y + x, LineColor);
	dc.SetPixel(MidPoint.x + y, MidPoint.y - x, LineColor);
	dc.SetPixel(MidPoint.x - y, MidPoint.y + x, LineColor);
	dc.SetPixel(MidPoint.x - y, MidPoint.y - x, LineColor);
}
void CArtist2DDlg::DrawLine(CDC& dc, CPoint from, CPoint to, COLORREF color)
{
	/*
	根据8个不同的平面区域，需要变量：
	自变量起点 IndFrom
	自变量终点 IndTo
	自变量步长 IndStep
	因变量起点 Dep
	因变量步长 k（斜率的规范值）
	区域		起点		终点		自变量		因变量
	1			from		to			x++			y+=k
	2			from		to			y++			x+=k
	3			to			from		y--			x+=k
	4			from		to			x--			y+=k
	5			to			from		x++			y+=k
	6			to			from		y++			x+=k
	7			from		to			y--			x+=k
	8			to			from		x--			y+=k
	*/
	
	CPoint FromPoint = from, ToPoint = to;

	int area = GetAreaID(from, to);
	if (area == 0)
	{
		DrawPixel(dc, from.x, from.y, color);
		return;
	}

	if((area == 3) || (area == 5) || (area == 6) || (area == 8))
	{
		FromPoint = to;
		ToPoint = from;
	}

	int IndStep = -1;
	int IndFrom = FromPoint.x, IndTo = ToPoint.x;
	int Dep = FromPoint.y;
	if((area % 4 == 1) || (area % 4 == 2))
	{
		IndStep = 1;
	}
	if(area % 4 > 1)
	{
		IndFrom = FromPoint.y;
		IndTo = ToPoint.y;
		Dep = FromPoint.x;
	}

	float k = 0;
	if(from.x != to.x)
	{
		k = 1.0f * (to.y - from.y) / (to.x - from.x);
		k = k < 0 ? -k : k;
		k = k > 1 ? (1.0f / k) : k;
	}
	k *= 2;
	float e = -1;
	for(int i = IndFrom; (IndStep > 0) ? (i <= IndTo) : (i >= IndTo); i += IndStep)
	{
		if(area % 4 < 2)
			DrawPixel(dc, i, Dep, color);
		else
			DrawPixel(dc, Dep, i, color);
		e += k;
		if(e >= 0)
		{
			Dep++;
			e -= 2;
		}
	}
}
	int CArtist2DDlg::GetAreaID(CPoint from, CPoint to)
	{
		if(from == to)
			return 0;
		if (from.x == to.x)
			if(from.y < to.y)	return 3;
			else	return 7;
		float k = 1.0f * (to.y - from.y) / (to.x - from.x);
		if(k >= 0 && k < 1)
		{
			if(from.x < to.x)
				return 1;
			else
				return 5;
		}
		else if(k >= 1)
		{
			if(from.x < to.x)
				return 2;
			else
				return 6;
		}else if(k < -1)
		{
			if(from.x > to.x)
				return 3;
			else
				return 7;
		}else
		{
			if(from.x > to.x)
				return 4;
			else
				return 8;
		}
	}

void CArtist2DDlg::DrawEllipse(CDC& dc, CPoint from, CPoint to)
{
	CPoint FromPoint, ToPoint, MidPoint;
	int a = to.x - from.x, b = to.y - from.y;
	int doc = (abs(a) > abs(b) ? abs(b) : abs(a));
	int r = doc / 2;
	if(a > 0)
	{
		if(b > 0)
		{
			FromPoint = CPoint(from.x, from.y);
			ToPoint = CPoint(to.x, to.y);
			MidPoint = CPoint(from.x + r, from.y + r);
		}else
		{
			FromPoint = CPoint(from.x, to.y);
			ToPoint = CPoint(to.x, from.y);
			MidPoint = CPoint(from.x + r, from.y - r);
		}
	}else
	{
		if(b > 0)
		{
			FromPoint = CPoint(to.x, from.y);
			ToPoint = CPoint(from.x, to.y);
			MidPoint = CPoint(from.x - r, from.y + r);
		}else
		{
			FromPoint = CPoint(to.x, to.y);
			ToPoint = CPoint(from.x, from.y);
			MidPoint = CPoint(from.x - r, from.y - r);
		}
	}

	int x = 0, y = r, d1 =  3, d2 = 5 - 2 * r;
	float d = 1.25f - r;


	DrawCirclePixel(dc, MidPoint, x, y); //显示圆弧上的八个对称点

	while(x <= y)
	{
		if(d < 0)
			d += d1;
		else
		{
			d += d2;
			y--;
			d2 += 2;
		}
		x++;
		d1 += 2;
		d2 += 2;
		DrawCirclePixel(dc, MidPoint, x, y);
	}

}

void CArtist2DDlg::DrawPolygon(CDC& dc, ComplexPolygon* polygon)
	{
		if(!polygon->visible)
			return;
		int CycleAmount = polygon->GetCount();
		for(int i = 0; i < CycleAmount; i++)
		{
			PolygonCycle* cycle = polygon->GetCycle(i);
			int PointAmount = cycle->GetCount();
			for(int j = 0; j < PointAmount - 1; j++)
			{
				DrawLine(dc, cycle->GetPoint(j)->cord, cycle->GetPoint(j + 1)->cord, cycle->LineColor);
			}
			DrawLine(dc, cycle->GetTail()->cord, cycle->GetHead()->cord, cycle->LineColor);
		}
	}
void CArtist2DDlg::FillPolygon(CDC& dc, ComplexPolygon* polygon)
{
	FormalizePolygon(polygon);
	polygon->CreateMatrix(polygon->width, polygon->height);
	byte** matrix = polygon->GetMatrix();
	DrawPolygonToBuffer(matrix, polygon, polygon->MinX, polygon->MinY);
	for(int i = 0; i < polygon->width; i++)
	{
		for(int j = 0; j < polygon->height; j++)
		{
			if(matrix[i][j] == 2)
				DrawPixel(dc, polygon->MinX + i, polygon->MinY + j, polygon->FilledColor);
		}
	}
	polygon->DeleteMatrix();
}
void CArtist2DDlg::DrawLineToBuffer(byte** matrix, CPoint from, CPoint to, int MinX, int MinY)
{
	/*
	根据8个不同的平面区域，需要变量：
	自变量起点 IndFrom
	自变量终点 IndTo
	自变量步长 IndStep
	因变量起点 Dep
	因变量步长 k（斜率的规范值）
	区域		起点		终点		自变量		因变量
	1			from		to			x++			y+=k
	2			from		to			y++			x+=k
	3*			from		to			y++			x-=k
	4			from		to			x--			y+=k
	5*			from		to			x--			y-=k
	6*			from		to			y--			x-=k
	7			from		to			y--			x+=k
	8*			from		to			x++			y-=k
	*/

	CPoint FromPoint = from, ToPoint = to;
	if(from.y == to.y)
	{
		return;
	}

	int area = GetAreaID(from, to);
	if (area == 0)
	{
		matrix[from.x - MinX][from.y - MinY] = 1;
		return;
	}

	int IndStep = -1;
	int IndFrom, IndTo;
	int Dep;

	if(area == 1 || area == 2 || area == 3 || area == 8)
		IndStep = 1;

	switch (area)
	{
	case 1:
		{
			IndFrom = from.x;
			IndTo = to.x;
			Dep = from.y;
			break;
		}
	case 2:
		{
			IndFrom = from.y;
			IndTo = to.y;
			Dep = from.x;
			break;
		}
	case 3:
		{
			IndFrom = from.y;
			IndTo = to.y;
			Dep = from.x;
			break;
		}
	case 4:
		{
			IndFrom = from.x;
			IndTo = to.x;
			Dep = from.y;
			break;
		}
	case 5:
		{
			IndFrom = from.x;
			IndTo = to.x;
			Dep = from.y;
			break;
		}
	case 6:
		{
			IndFrom = from.y;
			IndTo = to.y;
			Dep = from.x;
			break;
		}
	case 7:
		{
			IndFrom = from.y;
			IndTo = to.y;
			Dep = from.x;
			break;
		}
	case 8:
		{
			IndFrom = from.x;
			IndTo = to.x;
			Dep = from.y;
			break;
		}
	default:
		break;
	}

	float k = 0;
	if(from.x != to.x)
	{
		k = 1.0f * (to.y - from.y) / (to.x - from.x);
		k = k < 0 ? -k : k;
		k = k > 1 ? (1.0f / k) : k;
	}
	k *= 2;
	float e = -1;

	int DDep = 1;
	if(area == 3 || area == 5 || area == 6 || area == 8)
		DDep *= -1;

	int _LastY = -1;
	for(int i = IndFrom; (IndStep > 0) ? (i <= IndTo) : (i >= IndTo); i += IndStep)
	{
		if(area % 4 < 2)
		{
			matrix[i - MinX][Dep - MinY] = 1;
			if(_LastY != Dep - MinY)
				for(int j = i - MinX - 1; j > 0; j--)
				{
					if(matrix[j][Dep - MinY] != 1)
						matrix[j][Dep - MinY] = 2 - matrix[j][Dep - MinY];
				}
			_LastY = Dep - MinY;
		}
		else
		{
			matrix[Dep - MinX][i - MinY] = 1;
			if(_LastY != i - MinY)
				for(int j =Dep - MinX - 1; j > 0; j--)
				{
					if(matrix[j][i - MinY] != 1)
						matrix[j][i - MinY] = 2 - matrix[j][i - MinY];
				}
			_LastY = i - MinY;
		}
		e += k;
		if(e >= 0)
		{
			Dep += DDep;
			e -= 2;
		}
	}
	if(to.y > from.y)
	{
		for(int j = from.x - MinX - 1; j > 0; j--)
		{
			if(matrix[j][from.y - MinY] != 1)
				matrix[j][from.y - MinY] = 2 - matrix[j][from.y - MinY];
		}
	} else if(to.y < from.y)
	{
		for(int j = to.x - MinX - 1; j > 0; j--)
		{
			if(matrix[j][to.y - MinY] != 1)
				matrix[j][to.y - MinY] = 2 - matrix[j][to.y - MinY];
		}
	}
}
void CArtist2DDlg::DrawPolygonToBuffer(byte** matrix, ComplexPolygon* polygon, int MinX, int MinY)
{
	if(!polygon->visible)
		return;
	int CycleAmount = polygon->GetCount();

	int count = 0;
	BOOL NoScan = FALSE;
	PolygonPoint *StartPoint, *EndPoint, *RefPoint;
	for(int i = 0; i < CycleAmount; i++)
	{
		PolygonCycle* cycle = polygon->GetCycle(i);
		int PointAmount = cycle->GetCount();
		for(int j = 0; j < PointAmount - 1; j++)
		{
			StartPoint = cycle->GetPoint(j);
			EndPoint = cycle->GetPoint(j + 1);
			RefPoint = cycle->GetPoint((j + 2) % PointAmount);

			DrawLineToBuffer(matrix, StartPoint->cord, EndPoint->cord, MinX, MinY);

		}
		StartPoint =cycle->GetTail();
		EndPoint = cycle->GetHead();
		RefPoint = cycle->GetPoint(1);	

		DrawLineToBuffer(matrix, StartPoint->cord, EndPoint->cord, MinX, MinY);

	}
}
