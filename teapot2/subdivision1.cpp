#include <windows.h>
#include <GL/gl.h>
#include "glut.h"
#include <math.h>

using namespace std;

#define X 0.525731112119133606
#define Z 0.850650808352039932

class Point {

public:
	double x;
	double y;
	double z;

	Point() {}

	Point(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

Point vertexList[12] = {
	{ -X, 0.0, Z },
{ X, 0.0, Z },
{ -X, 0.0, -Z },
{ X, 0.0, -Z },
{ 0.0, Z, X },
{ 0.0, Z, -X },
{ 0.0, -Z, X },
{ 0.0, -Z, -X },
{ Z, X, 0.0 },
{ -Z, X, 0.0 },
{ Z, -X, 0.0 },
{ -Z, -X, 0.0 }
};

int faces[20][3] = {
	{ 1,4,0 },
{ 4,9,0 },
{ 4,5,9 },
{ 8,5,4 },
{ 1,8,4 },
{ 1,10,8 },
{ 10,3,8 },
{ 8,3,5 },
{ 3,2,5 },
{ 3,7,2 },
{ 3,10,7 },
{ 10,6,7 },
{ 6,11,7 },
{ 6,0,11 },
{ 6,1,0 },
{ 10,1,6 },
{ 11,0,9 },
{ 2,11,9 },
{ 5,2,9 },
{ 11,2,7 }
};

Point center = { 0.0, 0.0, 0.0 };

int depth;

void init(void) {
	// Define material properties
	GLfloat mat_spec[] = { 3000.0, 3000.0, 3000.0, 3000.0 };
	GLfloat mat_shiny[] = { 100.0 };
	GLfloat mat_surf[] = { 1.0, 1.0, 0.0, 0.0 };

	// Set light propeorties...
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

	// and create two lights at two positions
	GLfloat light_pos0[] = { 1.0, 1.0, 1.0, 0.0 };
	GLfloat light_pos1[] = { -1.0, -1.0, 1.0, 0.0 };


	// Set clear (background) color
	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Set shading model to use
	glShadeModel(GL_SMOOTH);

	// Set material properties, as defined above
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiny);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_surf);

	// Finish setting up the two lights (position,
	// and component values (specular and diffuse))
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);

	// Enable lighting
	glEnable(GL_LIGHTING);

	// Activate individual lights
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);
}

Point crossNormal(Point vertex1, Point vertex2, Point vertex3) {
	Point vector1, vector2, normal;

	vector1.x = vertex1.x - vertex2.x;
	vector1.y = vertex1.y - vertex2.y;
	vector1.z = vertex1.z - vertex2.z;

	vector2.x = vertex2.x - vertex3.x;
	vector2.y = vertex2.y - vertex3.y;
	vector2.z = vertex2.z - vertex3.z;

	normal.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
	normal.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
	normal.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);

	return normal;
}

Point circleNormal(Point vertex1, Point center) {
	Point normal;

	normal.x = vertex1.x - center.x;
	normal.y = vertex1.y - center.y;
	normal.z = vertex1.z - center.z;

	return normal;
}

Point parametric(Point p1, Point p2, float t) {
	Point middlePoint;

	middlePoint.x = p1.x + (p2.x - p1.x) * t;
	middlePoint.y = p1.y + (p2.y - p1.y) * t;
	middlePoint.z = p1.z + (p2.z - p1.z) * t;

	return middlePoint;
}

Point deform(Point origen, Point point) {
	Point final, u, uNormalizado;
	float magnitud;

	// Calculamos el vector desde el origen hasta el punto dado
	u.x = point.x - origen.x;
	u.y = point.y - origen.y;
	u.z = point.z - origen.z;

	// Sacamos la magnitud
	magnitud = sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2));

	// Sacamos el vector normalizado
	uNormalizado.x = u.x / magnitud;
	uNormalizado.y = u.y / magnitud;
	uNormalizado.z = u.z / magnitud;

	// final = punto origen + vector normalizado
	final.x = origen.x + uNormalizado.x;
	final.y = origen.y + uNormalizado.y;
	final.z = origen.z + uNormalizado.z;

	return final;
}

void subdivide(Point v0, Point v1, Point v2, int depth) {
	Point p1, p2, p3, normal;

	if (depth == 0) {
		// draw triangle
		glBegin(GL_POLYGON);

		normal = circleNormal(v0, center);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(v0.x, v0.y, v0.z);

		normal = circleNormal(v1, center);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(v1.x, v1.y, v1.z);

		normal = circleNormal(v2, center);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(v2.x, v2.y, v2.z);

		glEnd();
	}
	else {
		// Calcular puntos
		p1 = parametric(v0, v1, 0.5);
		p2 = parametric(v0, v2, 0.5);
		p3 = parametric(v1, v2, 0.5);

		// Deform
		p1 = deform(center, p1);
		p2 = deform(center, p2);
		p3 = deform(center, p3);

		// Recursively call subdivide
		subdivide(p2, p3, v2, depth - 1);
		subdivide(v0, p1, p2, depth - 1);
		subdivide(p1, v1, p3, depth - 1);
		subdivide(p2, p1, p3, depth - 1);
	}
}

void display(void) {
	// Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glRotated(0.01, 0.0, 1.0, 0.0);

	// Get model - from library
	// glutSolidTeapot(0.80);

	glShadeModel(GL_FLAT);

	Point normal;

	for (int i = 0; i < sizeof(faces) / sizeof(faces[0]); i++) {
		subdivide(vertexList[faces[i][0]], vertexList[faces[i][1]], vertexList[faces[i][2]], depth);
	}

	glFlush();
}

void reshape(int w, int h) {
	// Set the viewport size, based on function input
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// Set the projection parameters bsaed on input size
	glMatrixMode(GL_PROJECTION);

	// first set as identity
	glLoadIdentity();

	// then set perspective projection parameters based on aspect ratio
	gluPerspective(20.0, (GLfloat)w / (GLfloat)h, 0.10, 20.0);

	// Set the modal view matrix to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set the "look at" point
	gluLookAt(6.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {

	switch (key) {
		// Use Esc key to exit
	case 27:
		exit(0);
		break;
	case 'a':
		depth += 1;
		break;
	case 'A':
		depth -= depth;
		break;
	}
}

int main(int argc, char * argv[]) {
	// insert code here...
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("OpenGL Teapot");

	// Additional initialization
	init();

	// Register callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(display);

	// Do main loop
	glutMainLoop();

	// Exit
	return 0;
}
