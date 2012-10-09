#ifndef __BBOX_H_
#define __BBOX_H_

// dot product which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y)
#define norm2(v)   dot(v,v)        // norm2 = squared length of vector
#define norm(v)    sqrt(norm2(v))  // norm = length of vector
#define d(u,v)     norm(u-v)       // distance = norm of difference


class BBox {
public:
	BBox() { minX = 0.0f; minY = 0.0f; maxX = 1.0f; maxY = 1.0f; }
	void setMinPoint(float x, float y) { minX = x; minY = y; }
	void setMaxPoint(float x, float y) { maxX = x; maxY = y; }
	
	float getTop() { return maxY; } // y value of uppermost side
	float getBottom() { return minY; } // y value of lowest side
	float getLeft() { return minX; } // x value of left most side
	float getRight() { return maxX; } // x value of right most side

	bool operator== (const BBox &other) const { return (minX == other.minX && maxX == other.maxX && minY == other.minY && maxY == other.maxY); }
	bool operator!= (const BBox &other) const { return !(*this == other); } 

	float minX, maxX;
	float minY, maxY;
};
#endif