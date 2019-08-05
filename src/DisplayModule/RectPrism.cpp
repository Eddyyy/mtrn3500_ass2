
#include "RectPrism.hpp"
#include "Messages.hpp"



#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#elif defined(WIN32)
#include <Windows.h>
#include <tchar.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


using namespace std;

RectPrism::RectPrism() : Shape() {
	x_length = 1.0;
	y_length = 1.0;
	z_length = 1.0;
}

RectPrism::RectPrism(double x_, double y_, double z_, float x_length_, float y_length_, float z_length_) : Shape(x_, y_, z_) {
	x_length = x_length_;
	y_length = y_length_;
	z_length = z_length_;
}

RectPrism::RectPrism(double x_, double y_, double z_, double rotation_, float x_length_, float y_length_, float z_length_) : Shape(x_, y_, z_, rotation_) {
	x_length = x_length_;
	y_length = y_length_;
	z_length = z_length_;
}

/*
RectPrism::RectPrism(ShapeInit shapeStruct) : Shape(shapeStruct) {
	x_length = shapeStruct.params.rect.xlen;
	y_length = shapeStruct.params.rect.ylen;
	z_length = shapeStruct.params.rect.zlen;
}
*/

//RectPrism::~RectPrism() {}

void RectPrism::Rect1() {
	//glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex3f(-x_length/2, 0, -z_length/2);
	glVertex3f(-x_length/2, y_length, -z_length/2);
	glVertex3f(x_length/2, y_length, -z_length/2);
	glVertex3f(x_length/2, 0, -z_length/2);
	glEnd();
}
void RectPrism::Rect2() {
	//glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-x_length/2, 0, -z_length/2);
	glVertex3f(-x_length/2, y_length, -z_length/2);
	glVertex3f(-x_length/2, y_length, z_length/2);
	glVertex3f(-x_length/2, 0, z_length/2);
	glEnd();
}
void RectPrism::Rect3() {
	//glColor3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(-x_length/2, 0, -z_length/2);
	glVertex3f(-x_length/2, 0, z_length/2);
	glVertex3f(x_length/2, 0, z_length/2);
	glVertex3f(x_length/2, 0, -z_length/2);
	glEnd();
}

void RectPrism::draw() {
	glPushMatrix();
	positionInGL();
	setColorInGL();

	//Front and back
	RectPrism::Rect1();
	glPushMatrix();
	glTranslatef(0, 0, z_length);
	RectPrism::Rect1();
	glPopMatrix();
	//Right and left
	RectPrism::Rect2();
	glPushMatrix();
	glTranslatef(x_length, 0, 0);
	RectPrism::Rect2();
	glPopMatrix();
	//Bottom and top
	RectPrism::Rect3();
	glPushMatrix();
	glTranslatef(0, y_length, 0);
	RectPrism::Rect3();
	glPopMatrix();

	glPopMatrix();
}

/*
ShapeInit RectPrism::getServerModel() {
	ShapeInit shapeModel = Shape::getServerModel();

	shapeModel.type = ShapeType::RECTANGULAR_PRISM;
	
	shapeModel.params.rect.xlen = x_length;
	shapeModel.params.rect.ylen = y_length;
	shapeModel.params.rect.zlen = z_length;
	
	return shapeModel;
}
*/
