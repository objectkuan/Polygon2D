// Artist2DDlg.h : 头文件
//

#pragma once
#include <afxmt.h> 
#include <cmath>
#include "PolygonElements.h"
#include "ClippingDialog.h"

// CArtist2DDlg 对话框
class CArtist2DDlg : public CDialog
{
// 构造
public:
	CArtist2DDlg(CWnd* pParent = NULL);	// 标准构造函数

	// 对话框数据
	enum { IDD = IDD_ARTIST2D_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//BOOL PreTranslateMessage(MSG* pMsg);





public:
	// 工具栏相关
	typedef UINT ToolBoxItemIDType;
	ToolBoxItemIDType* ToolBoxItemUp;
	ToolBoxItemIDType* ToolBoxItemDown;
	int ToolBoxX1, ToolBoxY1, ToolBoxX2, ToolBoxY2;
	int ToolItemWidth;
	int curTool;

	enum TOOLS
	{
		DRAGGER = 0,
		CLIPPER,
		FILLER,
		SCALER,
		ROTATOR,
		POLYGON,
		OVERTURNER,
		ERASER
	};

	// 颜色相关
	int curColor;
	COLORREF LineColor, FillColor;
	int ColorX1, ColorY1, ColorX2, ColorY2;
	int ColorButtonWidth, ColorButtonHeight;
	int ColorSelectorPanelX1, ColorSelectorPanelY1, ColorSelectorPanelX2, ColorSelectorPanelY2;
	BOOL ChoosingColor;
	CSliderCtrl* Sliders;
	CEdit* Edits;
	CStatic* Statics;
	CCheckListBox* PolygonList;
	CButton *LayerUp, *LayerDown;

	// 多边形相关
	int PolygonListX1, PolygonListY1, PolygonListX2, PolygonListY2, PolygonListHeight;

	// 界面备份
		CBitmap* BackupBeforeChoosingColor;
		CBitmap* WhiteCanvas;
		CBitmap* BackupCanvasBeforePolygon;
		CBitmap* BackupCanvasForEachPolygonLine;
		CBitmap* BackupCanvasBeforeRotating;
		CBitmap* BackupCanvasBeforeScaling;

	// 画布相关
	int CanvasX1, CanvasY1, CanvasX2, CanvasY2;
	int CanvasWidth, CanvasHeight;
	BOOL MouseInside;

	// 绘图相关
	CRect ClientRect;
	//BOOL UsingTools;
	CPoint BeginPoint, EndPoint, HeadPoint, StoredPoint;

	// 多边形相关
	ComplexPolygon *MyPolygon;
	int PolygonID;
	CList<ComplexPolygon*> Polygons;

private:
	BOOL HitTool(CPoint point);
	BOOL HitColor(CPoint point);
	BOOL HitColorSelector(CPoint point);
	BOOL HitCanvas(CPoint point);

	CBitmap* GetBitmap(CDC& dc, int x1, int y1, int x2, int y2);
	void ReleaseBitmap(CBitmap** bitmap);
	void DrawBitmapAndRelease(CDC& dc, CBitmap** bitmap, int x1, int y1, int x2, int y2);
	bool CopyBitmap( const CBitmap &bmpSrc, CBitmap &bmpDest );

	BOOL IsNumber(CString str);
	void GetClippingInformation(CString* pStrClippedID ,CString* pStrClippingID);

	void PerformLayerUp();
	void PerformLayerDown();

private:
 	void DrawToolBoxPanel(CPaintDC& dc);
	void DrawToolBoxItems(CPaintDC& dc);

	void DrawColorButtonPanel(CPaintDC& dc);
	void DrawColorButtons(CPaintDC& dc);
	void DrawColorButtonColors(CPaintDC& dc);

	void DrawColorSelectorPanel(CPaintDC& dc);
	void DrawColorSelectorSliders(CPaintDC& dc);
	void DrawColorSelectorTextBoxes(CPaintDC& dc);

	void DrawCanvas(CPaintDC& dc);

 	CPoint GetCrossPoint(CPoint a1, CPoint b1, CPoint a2, CPoint b2);

	void TurnOverPolygon(ComplexPolygon* polygon, BOOL horizontal);
	void ScalePolygon(ComplexPolygon* polygon, CPoint center, double sx, double sy);
		void PerformScaling(CPoint* point, double sx, double sy);
	void RotatePolygon(ComplexPolygon* polygon, CPoint center, double angle);
		void PerformRotation(CPoint* point, double angle);
		void RefreshMinMaxXY(ComplexPolygon* polygon);
	void DragPolygon(ComplexPolygon* polygon, CPoint from, CPoint to);
	void InsertPolygon(ComplexPolygon* polygon);
	void RemovePolygon(int index);
	void FormalizePolygon(ComplexPolygon* polygon);
	ComplexPolygon* ClipPolygon(ComplexPolygon* OriPolygon, ComplexPolygon* CliPolygon);
		void SortPolygon(ComplexPolygon* polygon);
			void SortPolygonCycle(PolygonCycle* cycle, BOOL reversed);
			BOOL InsidePolygonCycle(PolygonPoint* point, PolygonCycle* cycle);
				BOOL ReallyBefore(PolygonPoint* p1, PolygonPoint* p2, PolygonCycle* SharedCycle, BOOL IsCli);
			BOOL CheckPolygonCrossed(ComplexPolygon* polygon);
				BOOL CheckSegmentCrossed(PolygonPoint* a1, PolygonPoint* b1, PolygonPoint* a2, PolygonPoint* b2);

	void DrawEllipse(CDC& dc, CPoint from, CPoint to);
	double DistanceBetween(PolygonPoint* p1, PolygonPoint* p2);
	double DistanceBetween(CPoint p1, CPoint p2);
	double GetFromTanTheta(CPoint p);
	ComplexPolygon* DuplicatePolygon(ComplexPolygon* polygon);
	PolygonCycle* DuplicateCycle(PolygonCycle* cycle);
	PolygonPoint* DuplicatePoint(PolygonPoint* point);
	void ClearUpPolygon(ComplexPolygon* polygon);

	void DrawPixel(CDC& dc, int x, int y, COLORREF color);
	void FillPixel(CDC& dc, int x, int y);
	void DrawCirclePixel(CDC& dc, CPoint MidPoint, int x, int y);
	void DrawLine(CDC& dc, CPoint from, CPoint to, COLORREF color);
		int GetAreaID(CPoint from, CPoint to);
	void FillPolygon(CDC& dc, ComplexPolygon* polygon);
	void DrawPolygon(CDC& dc, ComplexPolygon* polygon);

	void DrawLineToBuffer(byte** matrix, CPoint from, CPoint to, int MinX, int MinY);
	void DrawPolygonToBuffer(byte** matrix, ComplexPolygon* polygon, int MinX, int MinY);

private:
	BOOL NewToolBoxPanel;
	BOOL NewToolBoxItems;

	BOOL NewColorButtonPanel;
	BOOL NewColorButtons;
	BOOL NewColorButtonColors;

		BOOL BackupCanvasPart;
		BOOL RecoverCanvasPart;
	BOOL NewColorSelectorPanel;
	BOOL NewColorSelectorSliders;

	BOOL NewCanvas;
	BOOL BackupWhiteCanvas;

	BOOL NeedToDrawAllPolygons;
	BOOL NeedToDrawPolygonWhileMoving;
	BOOL NeedToDrawPolygonAfterClicked;

	BOOL NeedToDrawFourAngles;
	BOOL NeedToDrawFourAnglesWhileRotating;

	BOOL NeedToDrawEightBorders;
	BOOL NeedToDrawEightBordersWhileScaling;

	// 工具参数
	// POLYGON
	BOOL BeginPolygonCycle, BeginPolygon;
	// Dragger
	BOOL Dragging;
	BOOL OriginalFilled;
	int DragPolygonID;
	CPoint DragFrom, DragTo;
	// Rotator
	enum RotationStatusType
	{
		NONEROTATION = 0,
		CENTERINGROTATION,
		ROTATING
	} RotationStatus;
	int RotatePolygonID;
	int CenterRadius;
	CPoint RotationCenter,CenterFrom, CenterTo, RotateFrom, RotateTo;
	CPoint RotateLT, RotateRT, RotateLB, RotateRB;
	CPoint BackupRotateLT, BackupRotateRT, BackupRotateLB, BackupRotateRB;
	ComplexPolygon* RotatedPolygon;
	// Scaler
	enum ScalingStatusType
	{
		NONESCALING = 0,
		CENTERINGSCALING,
		SCALING
	} ScalingStatus;
	enum ScalingFormatType
	{
		HORIZONTALSCALING = 0,
		VERTICALSCALING,
		BOTHSCALING
	} ScalingFormat;
	int ScalePolygonID;
	CPoint Scale_7, Scale_8, Scale_9, Scale_4, Scale_6, Scale_1, Scale_2, Scale_3;
	CPoint BackupScale_7, BackupScale_8, BackupScale_9, BackupScale_4, BackupScale_6, BackupScale_1, BackupScale_2, BackupScale_3;
	ComplexPolygon* ScaledPolygon;
	CPoint ScaleFrom, ScaleTo;
	// Overturner
	int OverturnedPolygonID;

public:
//	afx_msg void OnMove(int x, int y);
	afx_msg void OnClose();
	afx_msg void OnEnChangeR();
	afx_msg void OnEnChangeG();
	afx_msg void OnEnChangeB();
	afx_msg void OnNMCustomdrawR(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawG(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawB(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnVisibleChanged();
	afx_msg void OnSelPolygonChanged();
//	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMove(int x, int y);
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};
