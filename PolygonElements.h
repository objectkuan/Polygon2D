#ifndef __POLYGON_OBJ_
#define __POLYGON_OBJ_

enum PointKind
{
	VERTEX = 0,	// ����ζ���
	UNKNOWN,	// δ����
	INPOINT,		// ���
	OUTPOINT,	// ����
	DONE			// ����ɱ�����
};

class PolygonPoint
{
public:
	// ����
	CPoint cord;							// ����
	// �ü����
	PointKind kind;						// ��������
	// ����ʱ��������
	int OriCycle;	// λ�ڱ��þ��εĻ�ID
	int OriEdge;	// λ�ڸû��ı�ID
	int CliCycle;	// λ�ڲü����еĻ�ID
	int CliEdge;	// λ�ڸû��ı�ID
	int LaterOriEdge;	// �����Ӧ��λ�ڱ��û���ID
	int LaterCliEdge;		// �����Ӧ��λ�ڲü�����ID
	BOOL OriInserted, CliInserted;	// �Ƿ��Ѿ������˶������
	int CrossPointID;  // ����ı��

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