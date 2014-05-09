#include "stdafx.h"
#include "PolygonElements.h"

PolygonPoint::PolygonPoint()
{
	cord = CPoint(-1, -1);
	kind = UNKNOWN;
	CliCycle = CliEdge = OriCycle = OriEdge = LaterCliEdge = LaterOriEdge = -1;
	CrossPointID = -1;
	OriInserted = CliInserted = FALSE;
}

PolygonPoint::PolygonPoint(CPoint Coordinate, PointKind PointKind)
{
	cord = Coordinate;
	kind = PointKind;
	CliCycle = CliEdge = OriCycle = OriEdge = LaterCliEdge = LaterOriEdge = -1;
	CrossPointID = -1;
	OriInserted = CliInserted = FALSE;
}

PolygonPoint::PolygonPoint(int x, int y, PointKind PointKind)
{
	cord = CPoint(x, y);
	kind = PointKind;
	CliCycle = CliEdge = OriCycle = OriEdge = LaterCliEdge = LaterOriEdge = -1;
	CrossPointID = -1;
	OriInserted = CliInserted = FALSE;
}

PolygonCycle::PolygonCycle()
{
	Reversed = FALSE;
	LineColor = RGB(0, 0, 0);
}

int PolygonCycle::GetCount()
{
	return points.GetCount();
}

PolygonPoint* PolygonCycle::GetPoint(int index)
{
	return points.GetAt(points.FindIndex(index));
}

PolygonPoint* PolygonCycle::GetTail()
{
	return points.GetTail();
}

PolygonPoint* PolygonCycle::GetHead()
{
	return points.GetHead();
}

void PolygonCycle::InsertAfter(int index, PolygonPoint* p)
{
	points.InsertAfter(points.FindIndex(index), p);
}

void PolygonCycle::InsertToTail(PolygonPoint* p)
{
	points.AddTail(p);
}

void PolygonCycle::SetPoint(int index, PolygonPoint* p)
{
	points.SetAt(points.FindIndex(index), p);
}

void PolygonCycle::Remove(int index)
{
	points.RemoveAt(points.FindIndex(index));
}


ComplexPolygon::ComplexPolygon()
{
	Filled = FALSE;
	FilledColor = RGB(0, 0, 0);
	visible = TRUE;
}

int ComplexPolygon::GetCount()
{
	return cycles.GetCount();
}

PolygonCycle* ComplexPolygon::GetCycle(int index)
{
	return cycles.GetAt(cycles.FindIndex(index));
}

PolygonCycle* ComplexPolygon::GetTail()
{
	return cycles.GetTail();
}

void ComplexPolygon::InsertAfter(int index, PolygonCycle* c)
{
	cycles.InsertAfter(cycles.FindIndex(index), c);
}

void ComplexPolygon::InsertToTail(PolygonCycle* c)
{
	cycles.AddTail(c);
}

void ComplexPolygon::SetCycle(int index, PolygonCycle* c)
{
	cycles.SetAt(cycles.FindIndex(index), c);
}

void ComplexPolygon::Remove(int index)
{
	cycles.RemoveAt(cycles.FindIndex(index));
}

void ComplexPolygon::CreateMatrix(int Width, int Height)
{
	Matrix = new byte*[Width];
	for(int i = 0; i < Width; i++)
	{
		Matrix[i] = new byte[Height];
		memset(Matrix[i], 0, Height * sizeof(byte));
	}
}

byte** ComplexPolygon::GetMatrix()
{
	return Matrix;
}

void ComplexPolygon::DeleteMatrix()
{
	for(int i = 0; i < width; i++)
		delete[] Matrix[i];
	delete[] Matrix;
}