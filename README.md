Polygon2D
=======================================

A 2D polygon drawing panel.

Each polygon can be a convex one or a concave one, placed in a certain layer.

Each polygon has its line color and fill color. 

Users can draw, erase, scale, rotate, fill, drag, mirror any polygon.

And two polygon can be clipped to get a new polygon.

Each polygon can contains a close ring like "回".

In the code, each simple polygon is stored in #PolygonCycle#, and complex polygons with inner-rings is stored in #ComplexPolygon#.


### Tool 1: drag ###

Drag a polygon to a new position

### Tool 2: clip ###

Clip two polygons, and get a new polygon which is the intersection of them.

The clipping is based on the [Weiler-Atherton algorithm](http://en.wikipedia.org/wiki/Weiler–Atherton_clipping_algorithm).

And a detail issue in the inner-ring case is fixed with W-A algorithm.

### Tool 3: fill ###

Filling is implemented using [Scanline Fill Algorithm](http://en.wikipedia.org/wiki/Flood_fill#Scanline_fill)

### Tool 4: scale ###

Using matrix operations.

### Tool 5: rotate ###

Using matrix operations.

### Tool 6: draw ###

Left-click to pick a vertex, and right-click to close a polygon cycle (that is a simple polygon).

And one or more polygon cycles form a complex polygon (with inner-rings).

Double right-click to finish a complex polygon.

And the complex polygon lies on a new layer.

### Tool 7: mirror ###

Left-click to mirror a polygon horizontally, and right-click vertically.

### Tool 8: erase ###

Erase a polygon.








