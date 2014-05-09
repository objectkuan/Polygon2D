#ifndef __POLYGON_OBJ_
#define __POLYGON_OBJ_

enum PointKind
{
	VERTEX = 0,	// 多边形顶点
	UNKNOWN,	// 未定义
	INPOINT,		// 入点
	OUTPOINT,	// 出点
	DONE			// 已完成遍历的
};

class PolygonPoint
{
public:
	// 坐标
	CPoint cord;							// 坐标
	// 裁剪相关
	PointKind kind;						// 顶点类型
	// 正则化时辅助变量
	int OriCycle;	// 位于被裁矩形的环ID
	int OriEdge;	// 位于该环的边ID
	int CliCycle;	// 位于裁剪举行的环ID
	int CliEdge;	// 位于该环的边ID
	int LaterOriEdge;	// 排序后应该位于被裁环的ID
	int LaterCliEdge;		// 排序后应该位于裁剪环的ID
	BOOL OriInserted, CliInserted;	// 是否已经插入了多边形中
	int CrossPointID;  // 交点的编号

	PolygonPoint();
	PolygonPoint(CPoint Coordinate, PointKind PointKind);
	PolygonPoint(int x, int y, PointKind PointKind);
protected:
private:
};

class PolygonCycle
{
public:
	PolygonCycle();
	PolygonPoint* GetPoint(int index);
	PolygonPoint* GetTail();
	PolygonPoint* GetHead();
	void SetPoint(int index, PolygonPoint* p);
	void InsertAfter(int index, PolygonPoint* p);
	void InsertToTail(PolygonPoint* p);
	int GetCount();
	void Remove(int index);
	BOOL Reversed;
	COLORREF LineColor;
protected:
private:
	CList<PolygonPoint*> points;
};

class ComplexPolygon
{
public:
	ComplexPolygon();
	PolygonCycle* GetCycle(int index);
	PolygonCycle* GetTail();
	void SetCycle(int index, PolygonCycle* p);
	void InsertAfter(int index, PolygonCycle* p);
	void InsertToTail(PolygonCycle* c);
	int GetCount();
	void Remove(int index);

	BOOL Filled;
	COLORREF FilledColor;
	BOOL visible;

	int MinX, MaxX, MinY, MaxY;
	int width, height;

	void CreateMatrix(int Width, int Height);
	byte** GetMatrix();
	void DeleteMatrix();
protected:
private:
	CList<PolygonCycle*> cycles;
	byte** Matrix;
};


#endif