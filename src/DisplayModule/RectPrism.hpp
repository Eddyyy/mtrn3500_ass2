#ifndef RECT_PRISM_H
#define RECT_PRISM_H

#include "Messages.hpp"
#include "Shape.hpp"

class RectPrism : public Shape {

private:
	float x_length;
	float y_length;
	float z_length;

public:
	RectPrism();
	RectPrism(double x_, double y_, double z_, float x_length_, float y_length_, float z_length_);
	RectPrism(double x_, double y_, double z_, double rotation_, float x_length_, float y_length_, float z_length_);
	//RectPrism(ShapeInit shapeStruct);
	~RectPrism() = default;
	void Rect1();
	void Rect2();
	void Rect3();
	void draw();
	//ShapeInit getServerModel();
	/*double GetX();
	double GetY();
	double GetZ();
	void SetX(double xx);
	void SetY(double yy);
	void SetZ(double zz);*/
};

#endif
