//
//  main.cpp
//  teapot
//
//  Created by Eduardo Torres González on 1/8/18.
//  Copyright © 2018 Eduardo Torres González. All rights reserved.
//

#include <Windows.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include "glut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb.h>
#include <stb_image.h>

#include "shader.h"

#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;


// --------------------------------------- Variables para terreno -------------------------------- //

struct Vertex {
	float x, y, z;
	float nX, nY, nZ;
};

vector<Vertex> vertex;

float eyeX, eyeY, eyeZ, upX, upY, upZ,
lookX, lookY, lookZ, fov, midSteps,
startX, startY, stepX, stepY,
numX, numY, nearP, farP, nn, mm,
onumX, onumY, field_of_view, oeyeX,
oeyeY, oeyeZ;


// ---------------------------------------- Variables para skybox -------------------------------//

// build and compile shaders
// -------------------------
Shader skyboxShader;

unsigned int cubemapTexture;

// skybox VAO
unsigned int skyboxVAO, skyboxVBO;

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};


// ---------------------------------------- Variables para avioneta -------------------------------//

// Global variables for each of the your angles
static float LA1 = 0.0; //LA1
static float LB1 = 0.0; // LB1
static float LA2 = 0.0; // LA2
static float LB2 = 0.0; // LB2
static float LA3 = 0.0; //LA3
static float LB3 = 0.0; //LB3
static float LA0 = 0.0; //LA0
static float LB0 = 0.0; //LB0
static float angleInc = 3.0;
float hight = 5.0;

typedef struct treenode
{
	//GLfloat m[16];
	void(*transform)();
	void(*draw)();
	struct treenode *sibling;
	struct treenode *child;
} treenode;

treenode cuerpo, cabina, pico, ala1, ala2, ala3, ala4, fuga2, fuga4, llanta1, llanta2, llanta3, rueda1, rueda2, rueda3, alat1, alat2, bandera;

void traverse(treenode *root)
{
	// Check to see if this node is null (error checking)
	//if(root == NULL)
	//	return;

	// Apply transformation for current node and draw
	// Continue with depth-first search (go to children)
	glPushMatrix();

	root->transform();
	root->draw();

	// Check for children
	// NOTE that transformation is still applied
	if (root->child != NULL)
		traverse(root->child);

	glPopMatrix();

	// Check for siblings
	if (root->sibling != NULL)
		traverse(root->sibling);
}

// -----------------------------------------Funciones para crear la avioneta ---------------------//


void glutSolidCylinder(float r, float h, int n, int m)
{
	glPushMatrix();
	glRotatef(90, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0F, 0.0F, -h / 2);
	GLUquadricObj * qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, r, r, h, n, m);
	gluDeleteQuadric(qobj);
	glPopMatrix();
}

void RenderCuerpo()
{
	glPushMatrix();
	glScaled(2.0, 2.0, hight * 2);
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();
}

void RenderPico()
{
	glPushMatrix();
	glTranslated(0.0, 0.0, hight * 2 - 1.0);
	glScaled(0.5, 0.5, 1.5);
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();
}

void RenderCabina()
{
	glPushMatrix();
	glTranslated(0.0, 1.0, hight - 2.0);
	glScaled(1.5, 1.5, hight);
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();
}

void TransformNothign()
{

}

void RenderAla1()
{
	glPushMatrix();
	glTranslated(hight / 2 + 1, 0.0, hight - 2.0);
	glScaled(hight + 1, 0.1, hight / 2 + 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

void RenderAla3()
{
	glPushMatrix();
	//glTranslated(hight*1.25, 0.0, hight - 2.0);
	glScaled(hight / 2, 0.1, hight / 2);
	glutSolidCube(1.0);
	glPopMatrix();
}

void RenderAla2()
{
	glPushMatrix();
	glTranslated(-hight + 1, 0.0, hight - 2.0);
	glScaled(hight + 1, 0.1, hight / 2 + 1.0);
	glutSolidCube(1.0);
	glPopMatrix();
}

void RenderAla4()
{
	glPushMatrix();
	glScaled(hight / 2, 0.1, hight / 2);
	glutSolidCube(1.0);
	glPopMatrix();
}

void RenderFuga4()
{
	glPushMatrix();
	glTranslated(-hight, -0.75, hight - 2.0);
	glRotated(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.7, hight / 2, 15, 55);
	glPopMatrix();
}

void TransformAla3()
{
	glTranslated(hight*1.5, 0.0, hight - 2.0);
	glRotated(LA3, 1.0, 0.0, 0.0);
}

void TransformAla4()
{
	glTranslated(-hight * 1.65, 0.0, hight - 2.0);
	glRotated(LA3, 1.0, 0.0, 0.0);
}

void RenderFuga2()
{
	glPushMatrix();
	glTranslated(hight / 2 + hight / 4 + 1, -0.75, hight - 2.0);
	glRotated(90, 1.0, 0.0, 0.0);
	glutSolidCylinder(0.7, hight / 2, 15, 55);
	glPopMatrix();
}

void RenderBandera()
{
	glPushMatrix();
	glTranslated(0.0, 2.0, -hight * 1.5);
	glRotated(90, 0.0, 0.0, 1.0);
	glScaled(hight / 2, 0.1, hight / 3);
	glutSolidCube(1.0);
	glPopMatrix();
}

void RenderAlat1()
{
	glPushMatrix();
	glTranslated(hight / 2, 0.0, -hight);
	glScaled(hight / 2, 0.1, hight / 2);
	glutSolidCube(1.0);
	glPopMatrix();
}

void RenderAlat2()
{
	glPushMatrix();
	glTranslated(-hight + 2.5, 0.0, -hight);
	glScaled(hight / 2, 0.1, hight / 2);
	glutSolidCube(1.0);
	glPopMatrix();
}

void RenderLlantas()
{
	glPushMatrix();
	glutSolidCylinder(0.2, hight / 2, 15, 55);
	glPopMatrix();
}

void RenderRuedas()
{
	glPushMatrix();
	glRotated(90, 0.0, 1.0, 0.0);
	glutSolidTorus(0.2, 0.4, 15, 15);
	glPopMatrix();
}

void TransformLlanta1()
{
	glTranslated(hight / 4, -1.0, -hight);
	glRotated(LB3, 1.0, 0.0, 0.0);
}

void TransformLlanta2()
{
	glTranslated(-hight / 4, -1.0, -hight);
	glRotated(LB3, 1.0, 0.0, 0.0);
}

void TransformLlanta3()
{
	glTranslated(0.0, -1.0, hight*1.5);
	glRotated(LB3, 1.0, 0.0, 0.0);
}

void TransformRuedas()
{
	glTranslated(0.0, -1.6, 0.0);
	glRotated(LA0, 1.0, 0.0, 0.0);
}

void CreateInsect()
{

	cuerpo.draw = RenderCuerpo;
	cuerpo.transform = TransformNothign;
	cuerpo.child = &cabina;
	cuerpo.sibling = &pico;

	pico.draw = RenderPico;
	pico.transform = TransformNothign;
	pico.child = NULL;
	pico.sibling = NULL;

	cabina.draw = RenderCabina;
	cabina.transform = TransformNothign;
	cabina.child = NULL;
	cabina.sibling = &ala1;

	ala1.draw = RenderAla1;
	ala1.transform = TransformNothign;
	ala1.child = &fuga2;
	ala1.sibling = &ala3;

	fuga2.draw = RenderFuga2;
	fuga2.transform = TransformNothign;
	fuga2.child = NULL;
	fuga2.sibling = NULL;

	ala3.draw = RenderAla3;
	ala3.transform = TransformAla3;
	ala3.child = NULL;
	ala3.sibling = &ala2;

	ala2.draw = RenderAla2;
	ala2.transform = TransformNothign;
	ala2.child = &fuga4;
	ala2.sibling = &ala4;

	fuga4.draw = RenderFuga4;
	fuga4.transform = TransformNothign;
	fuga4.child = NULL;
	fuga4.sibling = NULL;

	ala4.draw = RenderAla4;
	ala4.transform = TransformAla4;
	ala4.child = NULL;
	ala4.sibling = &bandera;

	bandera.draw = RenderBandera;
	bandera.transform = TransformNothign;
	bandera.child = &alat1;
	bandera.sibling = &llanta1;

	alat1.draw = RenderAlat1;
	alat1.transform = TransformNothign;
	alat1.child = NULL;
	alat1.sibling = &alat2;

	alat2.draw = RenderAlat2;
	alat2.transform = TransformNothign;
	alat2.child = NULL;
	alat2.sibling = NULL;

	llanta1.draw = RenderLlantas;
	llanta1.transform = TransformLlanta1;
	llanta1.child = &rueda1;
	llanta1.sibling = &llanta2;

	llanta2.draw = RenderLlantas;
	llanta2.transform = TransformLlanta2;
	llanta2.child = &rueda2;
	llanta2.sibling = &llanta3;

	llanta3.draw = RenderLlantas;
	llanta3.transform = TransformLlanta3;
	llanta3.child = &rueda3;
	llanta3.sibling = NULL;

	rueda1.draw = RenderRuedas;
	rueda1.transform = TransformRuedas;
	rueda1.child = NULL;
	rueda1.sibling = NULL;

	rueda2.draw = RenderRuedas;
	rueda2.transform = TransformRuedas;
	rueda2.child = NULL;
	rueda2.sibling = NULL;

	rueda3.draw = RenderRuedas;
	rueda3.transform = TransformRuedas;
	rueda3.child = NULL;
	rueda3.sibling = NULL;


}

void setMaterialAvioneta()
{
	// Define material properties
	GLfloat mat_specular[] = { 0.775, 0.775, 0.775, 1.0 };
	GLfloat mat_shininess[] = { 76.0 };
	GLfloat mat_surface[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat diff_surface[] = { 0.4, 0.4, 0.4, 1.0 };

	// Set material properties, as defined above
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_surface);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff_surface);

	//Set shading model to use
	glShadeModel(GL_SMOOTH);
	//Enable depth testing (for hidden surface removal)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LESS);
}

// ----------------------------------------- Funciones para crear el skybox ---------------------//

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// ----------------------------------------- Funciones para crear el terreno ---------------------//

//"findNomrla" es usada para encontrar la normal entre dos vertices utilizando el producto cruz
struct Vertex findNormal(struct Vertex p1, struct Vertex p2, struct Vertex p3) {

	//Usando los puntos p1, p2 y p3 encontramos los dos vectores
	struct Vertex vector1 = { (p2.x - p1.x),(p2.y - p1.y),(p2.z - p1.z) };
	struct Vertex vector2 = { (p3.x - p1.x),(p3.y - p1.y),(p3.z - p1.z) };

	//Haciendo uso del producto punto calculamos la normal 
	float xComp = (vector1.y*vector2.z) - (vector1.z*vector2.y);
	float yComp = (vector1.z*vector2.x) - (vector1.x*vector2.z);
	float zComp = (vector1.x*vector2.y) - (vector1.y*vector2.x);
	struct Vertex normal = { xComp, yComp, zComp };
	return normal;
}

//"encontrarNorTerr" nos ayuda a encontrar las normales del terreno
void encontrarNorTerr()
{
	for (int i = 0; i < (numY - 1); i++) {

		for (int j = 0; j < (numX - 1); j++) {

			if (i == 0 && j == 0) {


				Vertex normal = findNormal(vertex.at(i*numX + j), vertex.at((i + 1)*numX + j), vertex.at(i*numX + (j + 1)));
				vertex.at(i*numX + j).nX = normal.x;
				vertex.at(i*numX + j).nY = normal.y;
				vertex.at(i*numX + j).nZ = normal.z;

			}
			else if (i == 0) {
				Vertex normal1 = findNormal(vertex.at(i*numX + j - 1), vertex.at((i + 1)*numX + j - 1), vertex.at(i*numX + j));
				Vertex normal2 = findNormal(vertex.at((i + 1)*numX + j - 1), vertex.at((i + 1)*numX + j), vertex.at(i*numX + j));
				Vertex normal3 = findNormal(vertex.at(i*numX + j), vertex.at((i + 1)*numX + j), vertex.at(i*numX + j + 1));

				vertex.at(i*numX + j).nX = (normal1.x + normal2.x + normal3.x) / 3;
				vertex.at(i*numX + j).nY = (normal1.y + normal2.y + normal3.y) / 3;
				vertex.at(i*numX + j).nZ = (normal1.z + normal2.z + normal3.z) / 3;
			}
			else if (j == 0) {
				Vertex normal1 = findNormal(vertex.at((i - 1)*numX + j), vertex.at(i*numX + j), vertex.at((i - 1)*numX + j + 1));
				Vertex normal2 = findNormal(vertex.at((i - 1)*numX + j + 1), vertex.at(i*numX + j), vertex.at(i*numX + j + 1));
				Vertex normal3 = findNormal(vertex.at(i*numX + j), vertex.at((i + 1)*numX + j), vertex.at(i*numX + j + 1));

				vertex.at(i*numX + j).nX = (normal1.x + normal2.x + normal3.x) / 3;
				vertex.at(i*numX + j).nY = (normal1.y + normal2.y + normal3.y) / 3;
				vertex.at(i*numX + j).nZ = (normal1.z + normal2.z + normal3.z) / 3;
			}
			else {
				Vertex normal1 = findNormal(vertex.at((i - 1)*numX + j), vertex.at(i*numX + j - 1), vertex.at(i*numX + j));
				Vertex normal2 = findNormal(vertex.at((i + 1)*numX + j - 1), vertex.at(i*numX + j - 1), vertex.at(i*numX + j));
				Vertex normal3 = findNormal(vertex.at((i + 1)*numX + j - 1), vertex.at((i + 1)*numX + j), vertex.at(i*numX + j));
				Vertex normal4 = findNormal(vertex.at(i*numX + j + 1), vertex.at((i + 1)*numX + j), vertex.at(i*numX + j));
				Vertex normal5 = findNormal(vertex.at((i - 1)*numX + j + 1), vertex.at(i*numX + j + 1), vertex.at(i*numX + j));
				Vertex normal6 = findNormal(vertex.at((i - 1)*numX + j), vertex.at((i - 1)*numX + j + 1), vertex.at(i*numX + j + 1));

				vertex.at(i*numX + j).nX = (normal1.x + normal2.x + normal3.x + normal4.x + normal5.x + normal6.x) / 6;
				vertex.at(i*numX + j).nY = (normal1.y + normal2.y + normal3.y + normal4.y + normal5.y + normal6.y) / 6;
				vertex.at(i*numX + j).nZ = (normal1.z + normal2.z + normal3.z + normal4.z + normal5.z + normal6.z) / 6;
			}
		}
	}

}

//"dibujarTerreno" con ayuda de GL_TRIANGLES dibujamos la maya que usara el terreno
void dibujarTerreno()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < (numY - 1); i++) {
		for (int j = 0; j < (numX - 1); j++) {

			int index = (i*numX + j);
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = ((i + 1)*numX + j);
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = (i*numX + (j + 1));
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = (i*numX + (j + 1));
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = ((i + 1)*numX + (j + 1));
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

			index = ((i + 1)*numX + j);
			glNormal3f(vertex.at(index).nX, vertex.at(index).nY, vertex.at(index).nZ);
			glVertex3f(vertex.at(index).x, vertex.at(index).y, vertex.at(index).z);

		}
	}
	glEnd();
}

//Leemos el archivo que contiene el Height Map para crear "montañas" dentro de la maya. El archivo contiene
//Otros parametros como la posición de la camara y el punto a donde ve la misma
int leerArchivo()
{
	int in;
	ifstream readFile;
	readFile.open("parametros.txt");

	if (!readFile) {
		cout << "No es posible abrir el archivo" << endl;
		return 1;
	}
	readFile >> eyeX;
	readFile >> eyeY;
	readFile >> eyeZ;
	readFile >> upX;
	readFile >> upY;
	readFile >> upZ;
	readFile >> lookX;
	readFile >> lookY;
	readFile >> lookZ;
	readFile >> fov;
	readFile >> nearP;
	readFile >> farP;
	readFile >> midSteps;
	readFile >> startX;
	readFile >> stepX;
	readFile >> numX;
	readFile >> startY;
	readFile >> stepY;
	readFile >> numY;

	onumX = numX;
	onumY = numY;
	oeyeX = eyeX;
	oeyeY = eyeY;
	oeyeZ = eyeZ;
	field_of_view = fov;

	for (int i = 0; i < numY; i++) {
		for (int j = 0; j < numX; j++) {
			Vertex v;
			v.x = startX + (j*stepX);
			v.y = startY + (i*stepY);
			readFile >> v.z;
			vertex.push_back(v);
		}
	}
	readFile.close();
}

void asigMater()
{
	GLfloat ambient[4] = { 0.7, 0.2, 0.4, 1.0 };
	GLfloat specular[4] = { 0.7, 0.8, 0.1, 1.0 };
	GLfloat diffuse[4] = { 0.7, 0.8, 0.1, 1.0 };

	//Color del "cielo"
	glClearColor(0.09, 0.61, 0.85, 0.0);



	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 20.0);

	//Set shading model to use
	glShadeModel(GL_SMOOTH);
	//Enable depth testing (for hidden surface removal)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LESS);

}

void asigLuces()
{

	glShadeModel(GL_SMOOTH);

	GLfloat lightP[4] = { 00.0, 0.9, 00.0,1.0 };
	GLfloat lightA[4] = { 0.0,0.9,0.4,1.0 };
	GLfloat lightS[4] = { 0.0,0.9,0.4,1.0 };
	GLfloat lightD[4] = { 0.9,0.9,0.9,1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, lightP);
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightA);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightS);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightD);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(field_of_view, 1.0, nearP, farP);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(oeyeX, oeyeY, oeyeZ, lookX, lookY, lookZ, upX, upY, upZ);




	//Se asignan los materiales

	//static float r = 0;
	//r += 0.1;
	glPushMatrix();
	//glRotatef(r, 0, 0, 1);
	glTranslated(-(onumX*stepX) / 2, -(onumY*stepY) / 2, 0);

	glPushMatrix();
	glTranslated(840, 890, 70);
	glRotated(90, 1, 0, 0);
	setMaterialAvioneta();
	traverse(&cuerpo);
	glPopMatrix();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	asigMater();
	encontrarNorTerr();
	dibujarTerreno();
	glPopMatrix();
	

	// Skybox
	// draw scene as normal
	//shader.use();
	//glm::mat4 model;
	//glm::mat4 view = camera.GetViewMatrix();
	//glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	//shader.setMat4("model", model);
	//shader.setMat4("view", view);
	//shader.setMat4("projection", projection);


	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	skyboxShader.use();
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	//skyboxShader.setMat4("view", view);
	//skyboxShader.setMat4("projection", projection);
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default

	glutSwapBuffers();
	glutPostRedisplay();
}

void processSpecialKeys(int key, int x, int y) {

	switch (key) {
	case GLUT_KEY_UP:
		oeyeX -= 1;
		break;
	case GLUT_KEY_DOWN:
		oeyeX += 1;
		break;
	case GLUT_KEY_LEFT:
		oeyeY -= 1;
		break;
	case GLUT_KEY_RIGHT:
		oeyeY += 1;
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	case 'r':
		field_of_view = fov;
		oeyeX = eyeX;
		oeyeY = eyeY;
		oeyeZ = eyeZ;
		break;
	}

	glutPostRedisplay();
}

void init(void)
{
	CreateInsect();	
}


int main(int argc, char** argv)
{

	// Create tree structure
	init();
	leerArchivo();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Project terrain");

	// Skybox

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	// build and compile shaders
	// -------------------------
	Shader skyboxShaderLocal("6.1.skybox.vs", "6.1.skybox.fs");
	skyboxShader = skyboxShaderLocal;

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	// load textures
	// -------------
	vector<std::string> faces
	{
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/right.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/left.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/top.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/bottom.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/front.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/back.jpg"
	};
	cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// Fin Skybox

	//Asignar las luces
	asigLuces();

	glEnable(GL_NORMALIZE);

	glEnable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	//midPointAl();
	glutSpecialFunc(processSpecialKeys);
	glutMainLoop();
	return 0;
}