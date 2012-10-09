#ifndef __AABOX_H_
#define __AABOX_H_

class AxisAlignedBox
{
public:
	float position[2];
	float width;
	float height;

	float getTop() { return position[1]; } // y value of uppermost side
	float getBottom() { return position[1] + height; } // y value of lowest side
	float getLeft() { return position[0]; } // x value of left most side
	float getRight() { return position[0] + width; } // x value of right most side

	bool intersects(AxisAlignedBox box)
	{
		bool areBottomsInBounds = (getBottom() >= box.getTop() && getBottom() <= box.getBottom()) ||
			(box.getBottom() >= getTop() &&box.getBottom() <= getBottom());

		bool areTopsInBounds = (getTop() >= box.getTop() && getTop() <= box.getBottom()) ||
			(box.getTop() >= getTop() && box.getTop() <= getBottom());

		bool areLeftsInBounds = (getLeft() >= box.getLeft() && getLeft() <= box.getRight()) ||
			(box.getLeft() >= getLeft() && box.getRight() <= getRight());

		bool areRightsInBounds = (getRight() >= box.getLeft() && getRight() <= box.getRight()) ||
			(box.getRight() >= getLeft() && box.getRight() <= getRight());

		bool intersects = (areBottomsInBounds || areTopsInBounds) && (areLeftsInBounds || areRightsInBounds);

		return intersects;
	}
};

#endif