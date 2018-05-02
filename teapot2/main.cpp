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
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include "shader.h"
//
//#include <stdlib.h>
//#include <fstream>
//#include <iostream>
//#include <vector>

#include "shader_m.h"
#include "camera.h"
#include "model.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<std::string> faces);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// ---------------------------------------- Variables para skybox -------------------------------//

// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
//float cubeVertices[] = {
//	// positions          // texture Coords
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//};
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


// -----------------------------------------Funciones para crear la avioneta ---------------------//

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
	glutSolidCube(1);
	/*glPushMatrix();
	glScaled(2.0, 2.0, hight * 2);
	glutSolidSphere(1.0, 15, 15);
	glPopMatrix();*/
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
	/*cuerpo.child = &cabina;
	cuerpo.sibling = &pico;*/

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


void init(void)
{
	CreateInsect();
}

int main()
{

	init();

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// Initialize Glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	// build and compile shaders
	// -------------------------
	/*Shader shader("6.1.cubemaps.vs", "6.1.cubemaps.fs");*/
	Shader skyboxShader("6.1.skybox.vs", "6.1.skybox.fs");
	

	//// cube VAO
	//unsigned int cubeVAO, cubeVBO;
	//glGenVertexArrays(1, &cubeVAO);
	//glGenBuffers(1, &cubeVBO);
	//glBindVertexArray(cubeVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------
	unsigned int cubeTexture = loadTexture("C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/marble.jpg");

	vector<std::string> faces
	{
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/right.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/left.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/top.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/bottom.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/front.jpg",
		"C:/Users/Eduardo Torres/source/repos/teapot2/teapot2/skybox/back.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// shader configuration
	// --------------------
	/*shader.use();
	shader.setInt("texture1", 0);*/

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw scene as normal
		//shader.use();
		glm::mat4 model;
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		/*shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);*/
		
		
		//// cubes
		//glBindVertexArray(cubeVAO);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, cubeTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		// Avioneta
		setMaterialAvioneta();
		//traverse(&cuerpo);
		glPushMatrix();
		//glTranslated(-0.5, -0.5, -0.5);
		glutSolidCube(10);
		glPopMatrix();

							  // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
							  // -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &skyboxVAO);
	//glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVAO);

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

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