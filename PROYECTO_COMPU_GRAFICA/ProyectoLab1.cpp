/*-------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------*/
/*---------------- Alumnos:                              ------------------*/
/*----------------          Cuevas Salgado Carlos        ------------------*/
/*----------------          Galindo Chavez Lisset America------------------*/
/*----------------------  Version: Visual Studio 2017 ---------------------*/
/* -------------------------- Primer Avance -------------------------------*/
/*----------------------  Instrucciones de uso ----------------------------*/
/*-------------     Tecla     ----    Movimiento   ------------------------*/
/*-------------       D       ----     Derecha     ------------------------*/
/*-------------       A       ----    Izquierda    ------------------------*/
/*-------------       W       ----     Aleja       ------------------------*/
/*-------------       S       ----     Acerca      ------------------------*/
/*------------- Mouse + botDer---- Rota la camara  ------------------------*/
/*------------------- Fecha de entrega: 03/05/2019 ------------------------*/
/*----------------------       2019-2      --------------------------------*/
/*------------- Computación gráfica e interacción humano computadora ---------------*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <string>
#include <iostream>
#include <fstream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

#include "Cylinder.h"

const float toRadians = 3.14159265f / 180.0f;

float movCoche;
float movOffset;
bool avanza;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
Texture rojo;
Texture montRusa;
Texture montRusa2;
Texture montRusa3;
Texture montRusa4;
Texture montRusa5;

//Texturas y modelos Carlos
//Texture pisoTexture;
/*Texture laminaTexture;
Texture metalAzulTexture;
Texture CirculoTexture;
Texture CircuitoCafeTexture;
*/
//Texturas Juego Giratorio
Texture MetalPisoTexture;//Suelo del juego
Texture rojoMetalTexture;//Estructura del guego giratorio
Texture laminaCirculoTexture;
Texture amarillo2Texture;//Estructura del juego giratorio
Texture circuitoAzulTexture; //Carrito del juego
Texture amarilloTexture; //Asientos del carrusel
Texture FloresTexture; //Carrito del juego
Texture azulTexture; //Carrito del juego
Texture CajaMetalTexture;
Texture coloresTexture;
Texture RayasTexture;


//Para los keyframes
float reproduciranimacion,habilitaranimacion, guardoFrame,reinicioFrame,ciclo,ciclo2,contador=0;

//Para cambio de hora
bool dia = true, noche = false, horario = true;
float nocheCont = 0.0f, diaCont = 0.0f;
std::string rtSkybox = "Textures/Skybox/sgod_ft.tga";
std::string lfSkybox = "Textures/Skybox/sgod_bk.tga";
std::string dnSkybox = "Textures/Skybox/sgod_dn.tga";
std::string upSkybox = "Textures/Skybox/sgod_up.tga";
std::string bkSkybox = "Textures/Skybox/sgod_rt.tga";
std::string ftSkybox = "Textures/Skybox/sgod_lf.tga";


//******Carrusel************
Model caballo;
Model T1_a;
Model T2_a;
Model techo;
Model columna;
//*******************

Model Carrusel;
Model Obj1;
Model Obj2;

////Variables para movimiento del juego giratorio
float GiratorioMov;
float MovimientoGira;
float MovY;
int estado;
glm::vec3 PosJuegoGira = glm::vec3(-30, -2, 70);//Vector para mover el juego giratorio
glm::vec3 vectorPos;

//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional

DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;

Model Tienda;

Model reja; 
Model faro;

Model Helados;
Model Cafe;

// -- RUEDA 

Model Rueda;
Model BaseRueda;
Model CabinaRueda;

// --------

// -- CARRITOS

Model Tarima;
Model Carro;

// -----------

/***Casa de terror**/
Model casaTerror1;
Model dragon;
Model dragonAlas;
Model zombie;
Model vamp;
/**************/
//Squartle
Model sq1;
Model sq2;
Model sq3;
Model sq4;

Skybox skybox;

/*----Para la montaña rusa----*/
Cylinder cylinder = Cylinder(20, 20, 0.5, 0.5);
float escala = 4.0f;
//float escalax = 30;
//float escalay = 15;
float escalaz = 3.25;
float transx = 30;
float transy = 15;
int	mov = 0;
/*----------------------------*/

float gira1;
float movDragon = true;
float giro;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

//void my_input(GLFWwindow *window);
void inputKeyframes(bool* keys);

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
	  2, 1, 0,
	  0, 3, 2,
	  // right
	  8, 9, 11,
	  8, 11, 10,
	  // back
	  5, 6, 7,
	  7, 4, 5,
	  // left
	  12, 13, 14,
	  12, 14, 15,
	  // bottom
	  22, 21, 20,
	  21, 22, 23,
	  // top
	  16, 17, 18,
	  19, 18, 17,
	};

	GLfloat cubo_vertices[] = {

		//x     y       z       S			T		NX		NY		NZ
	   // front (6)
	   -0.5f, -0.5f,  0.5f,    0.66f,  0.0f,		0.0f,	0.0f,	-1.0f,
	   0.5f, -0.5f,  0.5f,     1.0f,   0.0f,		0.0f,	0.0f,	-1.0f,
	   0.5f,  0.5f,  0.5f,     1.0f,   0.5f,		0.0f,	0.0f,	-1.0f,
	   -0.5f,  0.5f,  0.5f,    0.66f,  0.5f,		0.0f,	0.0f,	-1.0f,
	   // back (1)
	   -0.5f, -0.5f, -0.5f,    0.0f,   0.5f,		0.0f,	0.0f,	1.0f,
	   0.5f, -0.5f, -0.5f,     0.33f,  0.5f,		0.0f,	0.0f,	1.0f,
	   0.5f,  0.5f, -0.5f,     0.33f,  1.0f,		0.0f,	0.0f,	1.0f,
	   -0.5f,  0.5f, -0.5f,    0.0f,   1.0f,		0.0f,	0.0f,	1.0f,
	   // right (3)
	   0.5f, -0.5f,  0.5f,     0.66f,  0.5f,		-1.0f,	0.0f,	0.0f,
	   0.5f,  0.5f, 0.5f,      0.66,   1.0f,		-1.0f,	0.0f,	0.0f,
	   0.5f,  -0.5f,  -0.5f,   1.0f,   0.5f,		-1.0f,	0.0f,	0.0f,
	   0.5f, 0.5f, -0.5f,      1.0f,   1.0f,		-1.0f,	0.0f,	0.0f,
	   // left (4)
	   -0.5f, 0.5f, 0.5f,      0.33f,  0.5f,		1.0f,	0.0f,	0.0f,
	   -0.5f, -0.5f,  0.5f,    0.33f,  0.0f,		1.0f,	0.0f,	0.0f,
	   -0.5f, -0.5f,  -0.5f,   0.0f,   0.0f,		1.0f,	0.0f,	0.0f,
	   -0.5f,  0.5f, -0.5f,    0.0f,   0.5f,		1.0f,	0.0f,	0.0f,
	   // top (5)
	   0.5f,  0.5f,  0.5f,     0.66f,  0.5f,		0.0f,	-1.0f,	0.0f,
	   -0.5f,  0.5f,  0.5f,    0.33f,  0.5f,		0.0f,	-1.0f,	0.0f,
	   0.5f,  0.5f, -0.5f,     0.66f,  0.0f,		0.0f,	-1.0f,	0.0f,
	   -0.5f,  0.5f, -0.5f,    0.33,   0.0f,		0.0f,	-1.0f,	0.0f,
	   // bottom (2)
	   0.5f, -0.5f,  0.5f,     0.66f,  1.0f,		0.0f,	1.0f,	0.0f,
	   -0.5f, -0.5f,  0.5f,    0.33f,  1.0f,		0.0f,	1.0f,	0.0f,
	   0.5f, -0.5f, -0.5f,     0.66f,  0.5f,		0.0f,	1.0f,	0.0f,
	   -0.5f, -0.5f, -0.5f,    0.33f,  0.5f,		0.0f,	1.0f,	0.0f,
	};
	calcAverageNormals(cubo_indices, 36, cubo_vertices, 192, 8, 5);
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}
//Objetops mios
void creaCarro() {
	unsigned Carro_indices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,

		24, 25, 26,
		26, 27, 24,

		28, 29, 30,
		30, 31, 28,

		32, 33, 34,
		34, 35, 32,

		36, 37, 38,
		38, 39, 36,

		40, 41, 42,
		42, 43, 40,

		44, 45, 46,
		46, 47, 44,

	};

	GLfloat Carro_vertices[] = {

			 4.02f, -0.9f,  0.8f,  0.0f, 0.0f,  4.0f, 0.0f, 0.0f,
			 4.02f, -0.9f, -1.2f,  1.0f, 0.0f,  4.0f, 0.0f, 0.0f,
			 4.02f, -2.9f, -1.2f,  1.0f, 1.0f,  4.0f, 0.0f, 0.0f,
			 4.02f, -2.9f,  0.8f,  0.0f, 1.0f,  4.0f, 0.0f, 0.0f,

			 6.02f, -0.9f,  0.8f,  0.0f, 0.0f,  -4.0f, 0.0f, 0.0f,
			 6.02f, -0.9f, -1.2f,  1.0f, 0.0f,  -4.0f, 0.0f, 0.0f,
			 6.02f, -2.9f, -1.2f,  1.0f, 1.0f,  -4.0f, 0.0f, 0.0f,
			 6.02f, -2.9f,  0.8f,  0.0f, 1.0f,  -4.0f, 0.0f, 0.0f,

			 4.02f,  -0.9f, -1.2f,  0.0f, 0.0f,  0.80f, 0.0f, 0.0f,
			 4.02f,  -1.5f, -1.9f,  1.0f, 0.0f,  0.80f, 0.0f, 0.0f,
			 4.02f,  -2.9f, -2.2f,  1.0f, 1.0f,  0.80f, 0.0f, 0.0f,
			 4.02f,  -2.9f, -1.2f,  0.0f, 1.0f,  0.80f, 0.0f, 0.0f,

			 6.02f,  -0.9f, -1.2f,  0.0f, 0.0f,  -0.80f, 0.0f, 0.0f,
			 6.02f,  -1.5f, -1.9f,  1.0f, 0.0f,  -0.80f, 0.0f, 0.0f,
			 6.02f,  -2.9f, -2.2f,  1.0f, 1.0f,  -0.80f, 0.0f, 0.0f,
			 6.02f,  -2.9f, -1.2f,  0.0f, 1.0f,  -0.80f, 0.0f, 0.0f,

			 4.02f,  -2.9f, -2.2f,  0.0f, 0.0f,  0.0f, -0.6f, 2.8f,
			 4.02f,  -1.5f, -1.9f,  1.0f, 0.0f,  0.0f, -0.6f, 2.8f,
			 6.02f,  -1.5f, -1.9f,  1.0f, 1.0f,  0.0f, -0.6f, 2.8f,
			 6.02f,  -2.9f, -2.2f,  0.0f, 1.0f,  0.0f, -0.6f, 2.8f,

			 4.02f,  -0.9f, -1.2f,  0.0f, 0.0f,  0.0f, -1.4f, 1.2f,
			 4.02f,  -1.5f, -1.9f,  1.0f, 0.0f,  0.0f, -1.4f, 1.2f,
			 6.02f,  -1.5f, -1.9f,  1.0f, 1.0f,  0.0f, -1.4f, 1.2f,
			 6.02f,  -0.9f, -1.2f,  0.0f, 1.0f,  0.0f, -1.4f, 1.2f,

			 4.02f,  -2.9f, 0.8f,  0.0f, 0.0f,  0.0f, 0.0f, -4.0f,
			 4.02f,  -0.9f, 0.8f,  1.0f, 0.0f,  0.0f, 0.0f, -4.0f,
			 6.02f,  -0.9f, 0.8f,  1.0f, 1.0f,  0.0f, 0.0f, -4.0f,
			 6.02f,  -2.9f, 0.8f,  0.0f, 1.0f,  0.0f, 0.0f, -4.0f,

			 4.02f,  -2.9f,  0.8f,  0.0f, 0.0f,  0.0f, 6.0f, 0.0f,
			 4.02f,  -2.9f, -2.2f,  1.0f, 0.0f,  0.0f, 6.0f, 0.0f,
			 6.02f,  -2.9f, -2.2f,  1.0f, 1.0f,  0.0f, 6.0f, 0.0f,
			 6.02f,  -2.9f,  0.8f,  0.0f, 1.0f,  0.0f, 6.0f, 0.0f,

			 4.02f,  -0.9f,  0.8f,  0.0f, 0.0f,  0.0f, -0.4f, 0.0f,
			 4.02f,  -0.9f, -1.2f,  1.0f, 0.0f,  0.0f, -0.4f, 0.0f,
			 4.22f,  -0.9f, -1.2f,  1.0f, 1.0f,  0.0f, -0.4f, 0.0f,
			 4.22f,  -0.9f,  0.8f,  0.0f, 1.0f,  0.0f, -0.4f, 0.0f,

			 6.02f,  -0.9f,  0.8f,  0.0f, 0.0f,  0.0f, -0.4f, 0.0f,
			 6.02f,  -0.9f, -1.2f,  1.0f, 0.0f,  0.0f, -0.4f, 0.0f,
			 5.82f,  -0.9f, -1.2f,  1.0f, 1.0f,  0.0f, -0.4f, 0.0f,
			 5.82f,  -0.9f,  0.8f,  0.0f, 1.0f,  0.0f, -0.4f, 0.0f,

			 5.82f,  -0.9f, -1.2f,  0.0f, 0.0f,  0.0f, -0.4f, 0.0f,
			 4.22f,  -0.9f, -1.2f,  1.0f, 0.0f,  0.0f, -0.4f, 0.0f,
			 4.22f,  -0.9f, -1.0f,  1.0f, 1.0f,  0.0f, -0.4f, 0.0f,
			 5.82f,  -0.9f, -1.0f,  0.0f, 1.0f,  0.0f, -0.4f, 0.0f,

			 5.82f,  -0.9f, 0.8f,  0.0f, 0.0f,  0.0f, -0.4f, 0.0f,
			 4.22f,  -0.9f, 0.8f,  1.0f, 0.0f,  0.0f, -0.4f, 0.0f,
			 4.22f,  -0.9f, 0.6f,  1.0f, 1.0f,  0.0f, -0.4f, 0.0f,
			 5.82f,  -0.9f, 0.6f,  0.0f, 1.0f,  0.0f, -0.4f, 0.0f,

	};
	//obj 6
	Mesh *Carro = new Mesh();
	Carro->CreateMesh(Carro_vertices, Carro_indices, 8 * 4 * 12, 6 * 12);
	meshList.push_back(Carro);
}

void creaAsientos() {
	unsigned Asientos_indices[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20,

		24, 25, 26,
		26, 27, 24,

	};

	GLfloat Asientos_vertices[] = {

			 5.82f, -0.2f, 0.6f,  0.0f, 0.0f,  0.0f, 0.48f, 0.0f,
			 4.22f, -0.2f, 0.6f,  1.0f, 0.0f,  0.0f, 0.48f, 0.0f,
			 4.22f, -0.2f, 0.3f,  1.0f, 1.0f,  0.0f, 0.48f, 0.0f,
			 5.82f, -0.2f, 0.3f,  0.0f, 1.0f,  0.0f, 0.48f, 0.0f,

			 5.82f, -0.2f, 0.6f,  0.0f, 0.0f,  -0.81f, 0.0f, 0.0f,
			 5.82f, -0.2f, 0.3f,  1.0f, 0.0f,  -0.81f, 0.0f, 0.0f,
			 5.82f, -2.9f, 0.3f,  1.0f, 1.0f,  -0.81f, 0.0f, 0.0f,
			 5.82f, -2.9f, 0.6f,  0.0f, 1.0f,  -0.81f, 0.0f, 0.0f,

			 4.22f, -0.2f,  0.6f,  0.0f, 0.0f,  4.22f, 0.0f, 0.0f,
			 4.22f, -0.2f,  0.3f,  1.0f, 0.0f,  4.22f, 0.0f, 0.0f,
			 4.22f, -2.9f,  0.3f,  1.0f, 1.0f,  4.22f, 0.0f, 0.0f,
			 4.22f, -2.9f,  0.6f,  0.0f, 1.0f,  4.22f, 0.0f, 0.0f,

			 4.22f, -0.2f,  0.6f,  0.0f, 0.0f,  0.0f, 0.0f, -4.32f,
			 4.22f, -2.9f,  0.6f,  1.0f, 0.0f,  0.0f, 0.0f, -4.32f,
			 5.82f, -2.9f,  0.6f,  1.0f, 1.0f,  0.0f, 0.0f, -4.32f,
			 5.82f, -0.2f,  0.6f,  0.0f, 1.0f,  0.0f, 0.0f, -4.32f,

			 4.22f, -0.2f,  0.3f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 4.22f, -2.9f,  0.3f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 5.82f, -2.9f,  0.3f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
			 5.82f, -0.2f,  0.3f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,

			 4.22f, -1.4f,  0.3f,  0.0f, 0.0f,   1.0f, -1.4f, 1.0f,
			 5.82f, -1.4f,  0.3f,  1.0f, 0.0f,   1.0f, -1.4f, 1.0f,
			 5.82f, -1.4f,  -0.5f,  1.0f, 1.0f,  1.0f, -1.4f, 1.0f,
			 4.22f, -1.4f,  -0.5f,  0.0f, 1.0f,  1.0f, -1.4f, 1.0f,

			 4.22f, -1.4f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 2.4f,
			 5.82f, -1.4f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 2.4f,
			 5.82f, -2.9f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 2.4f,
			 4.22f, -2.9f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 2.4f,

	};
	//7
	calcAverageNormals(Asientos_indices, 42, Asientos_vertices, 224, 8, 5);
	Mesh *Asientos = new Mesh();
	Asientos->CreateMesh(Asientos_vertices, Asientos_indices, 224, 42);
	meshList.push_back(Asientos);
}

void creaHexagono() {
	unsigned Hexagono_indices[] = {
		
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		8, 9, 6,

		10, 11, 12,
		13, 14, 15,
		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		23, 24, 25,
		26, 27, 28,
		28, 29, 26,

		30, 31, 32,
		33, 34, 35,
		36, 37, 38,
		38, 39, 36,

		40, 41, 42,
		43, 44, 45, 
		46, 47, 48,
		48, 49, 46,

		50, 51, 52, 
		53, 54, 55,
		56, 57, 58,
		58, 59, 56,
	};

	GLfloat Hexagono_vertices[] = {

			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,    1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f, 0.0f, 0.866f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
			
			 0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 1.0f, 1.0f, 0.0f,    1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.5f, 1.0f, 0.866f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 1.0f,  0.0f, 0.0f,    0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			 0.5f,  0.0f, 0.866f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			 0.5f,  1.0f, 0.866f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  0.0f,    0.0f, 1.0f, -1.0f, 0.0f, 0.0f,

			 //2
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f, 0.0f, 0.866f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			-0.5f, 0.0f, 0.866f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,

			 0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.5f, 1.0f, 0.866f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.5f, 1.0f, 0.866f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 0.5f, 0.0f, 0.866f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			-0.5f, 0.0f, 0.866f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			-0.5f, 1.0f, 0.866f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			 0.5f, 1.0f, 0.866f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,

			 //3
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			-0.5f, 0.0f, 0.866f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  0.0f, 1.0f, 0.0f,

			 0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.5f, 1.0f, 0.866f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,    1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			-0.5f,  0.0f, 0.866f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			-1.0f,  0.0f, 0.0f,    1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, 0.0f,    1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			-0.5f,  1.0f, 0.866f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			 
			//4
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,    1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			-0.5f, 0.0f, -0.866f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f,

			 0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f,    1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.5f, 1.0f, -0.866f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 -1.0f, 0.0f,  0.0f,    0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			 -0.5f, 0.0f, -0.866f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			 -0.5f, 1.0f, -0.866f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			 -1.0f, 1.0f,  0.0f,    0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

			 //5
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		    -0.5f, 0.0f,-0.866f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			 0.5f, 0.0f,-0.866f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f,

			  0.0f, 1.0f, 0.0f,     0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 -0.5f, 1.0f, -0.866f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			  0.5f, 1.0f, -0.866f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

		   -0.5f, 0.0f, -0.866f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			0.5f, 0.0f, -0.866f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			0.5f, 1.0f, -0.866f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
		   -0.5f, 1.0f, -0.866f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

			 //6
			0.0f, 0.0f, 0.0f,     0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			0.5f, 0.0f, -0.866f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
			1.0f, 0.0f, 0.0f,     1.0f, 1.0f,  0.0f, 1.0f, 0.0f,

			 0.0f, 1.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			0.5f, 1.0f, -0.866f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,     1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			0.5f,  0.0f, -0.866f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			1.0f,  0.0f, 0.0f,     1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f,     1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			0.5f,  1.0f, -0.886f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,


	};
	//8 en lista meshList es el 7
	Mesh *Hexagono = new Mesh();
	Hexagono->CreateMesh(Hexagono_vertices, Hexagono_indices, 480, 72);
	meshList.push_back(Hexagono);
}

void creaBaseTriangulo() {
	unsigned BaseTriangulo_indices[] = {
		//1
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,

		12, 13, 14, 
		14, 15, 12,

		16, 17, 18,
		19, 20, 21,
		22, 23, 24,

		25, 26, 27,
		27, 28, 25,
		
		//2
		29, 30, 31,
		32, 33, 34,
		35, 36, 37, 
		38, 39, 40,

		41, 42, 43,
		43, 44, 41,

		45, 46, 47, 
		48, 49, 50, 
		51, 52, 53,

		54, 55, 56,
		56, 57, 54,
		
		//3
		58, 59, 60,
		61, 62, 63,
		64, 65, 66,
		67, 68, 69,

		70, 71, 72,
		72, 73, 70,

		74, 75, 76,
		77, 78, 79,
		80, 81, 82,

		83, 84, 85,
		85, 86, 83,
		
		//4
		87, 88, 89, 
		90, 91, 92, 
		93, 94, 95, 
		96, 97, 98,

		99, 100, 101,
		101, 102, 99,

		103, 104, 105,
		106, 107, 108,
		109, 110, 111,
		
		112, 113, 114,
		114, 115, 112,
		
		//5

		116, 117, 118,
		119, 120, 121,
		122, 123, 124, 
		125, 126, 127,

		128, 129, 130,
		130, 131, 128,

		132, 133, 134,
		135, 136, 137,
		138, 139, 140,

		141, 142, 143, 
		143, 144, 141,

		//6
		145, 146, 147, 
		148, 149, 150, 
		151, 152, 153, 
		154, 155, 156,

		157, 158, 159,
		159, 160, 157,

		161, 162, 163,
		164, 165, 166,
		167, 168, 169,

		170, 171, 172,
		172, 173, 170,
	
	};

	GLfloat BaseTriangulo_vertices[] = {
			//10 y 15 grados
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 1.0f, 0.0f, 0.0f,    1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.5f, 0.0f, 0.866f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.9848f, 0.0f, 0.1736f,    1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.9848f, 0.5f, 0.1736f,    1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 0.9659f, 0.0f, 0.2588f,    1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 0.9695f, 0.5f, 0.2588f,    1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			 0.0f, 0.0f, 0.0f,        0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.9848f, 0.5f, 0.1736f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.9659f, 0.5f, 0.2588f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 0.9848f,  0.0f, 0.1736f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			 0.9848f,  0.5f, 0.1736f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			 0.9659f,  0.5f, 0.2588f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			 0.9659f,  0.0f, 0.2588f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			
			 //50 y 55 grados
			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 0.6427f, 0.0f, 0.7660f,    1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 0.6427f, 0.5f, 0.7660f,    1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.5735f, 0.0f, 0.8191f,    1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.5735f, 0.5f, 0.8191f,    1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,        0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.6427f, 0.5f, 0.7660f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.5727f, 0.5f, 0.8191f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 0.6427f,  0.0f, 0.7660f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			 0.6427f,  0.5f, 0.7660f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			 0.5735f,  0.5f, 0.8191f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			 0.5735f,  0.0f, 0.8191f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,

			 //2
			 //70 y 75 grados
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.5f, 0.0f, 0.866f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.5f, 0.0f, 0.866f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 0.3420f, 0.0f, 0.9396f,    1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 0.3420f, 0.5f, 0.9396f,    1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.2588f, 0.0f, 0.9659f,    1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 0.2588f, 0.5f, 0.9659f,    1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,        0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.3420f, 0.5f, 0.9396f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.2588f, 0.5f, 0.9659f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 0.3420f,  0.0f, 0.9396f,   0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			 0.3420f,  0.5f, 0.9396f,   1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,
			 0.2588f,  0.5f, 0.9659f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			 0.2588f,  0.0f, 0.9659f,   0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
			 //110 y 115
			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
		    -0.3420f, 0.0f, 0.9396f,    1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
		    -0.3420f, 0.5f, 0.9396f,    1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			-0.4426f, 0.0f, 0.9063f,    1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			-0.4426f, 0.5f, 0.9063f,    1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,        0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.3420f, 0.5f, 0.9396f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.4426f, 0.5f, 0.9063f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 -0.3420f,  0.0f, 0.9360f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			 -0.3420f,  0.5f, 0.9360f,   1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			 -0.4426f,  0.5f, 0.9063f,   1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			 -0.4426f,  0.0f, 0.9063f,   0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

			 //3
			 //130 y 135
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.5f, 0.0f, 0.866f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,    1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			  0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 -0.6427f, 0.0f, 0.7660f,    1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			 -0.6427f, 0.5f, 0.7660f,    1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			  0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 -0.7071f, 0.0f, 0.7071f,    1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			 -0.7071f, 0.5f, 0.7071f,    1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

			  0.0f, 0.0f, 0.0f,        0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 -0.6427f, 0.5f, 0.7660f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 -0.7071f, 0.5f, 0.7071f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			-0.6427f,  0.0f, 0.7660f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			-0.6427f,  0.5f, 0.7660f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			-0.7071f,  0.5f, 0.7071f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			-0.7071f,  0.0f, 0.7071f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			//170 y 175 
			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		   -0.9848f, 0.0f, 0.1763f,    1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
		   -0.9848f, 0.5f, 0.1736f,    1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			 0.0f, 0.0f, 0.0f,          0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			-0.9961f, 0.0f, 0.0871f,    1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			-0.9961f, 0.5f, 0.0871f,    1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,        0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.9848f, 0.5f, 0.1763f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.9961f, 0.5f, 0.0871f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 -0.9848f,  0.0f, 0.1736f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			 -0.9848f,  0.5f, 0.1736f,   1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
			 -0.9961f,  0.5f, 0.0871f,   1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			 -0.9961f,  0.0f, 0.0871f,   0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

			//4
			//190 y 195
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-1.0f, 0.0f, 0.0f,    1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.5f, 0.0f, -0.866f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 0.0f, 0.0f,0.0f,        0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
			-0.9848f, 0.0f, -0.1736f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			-0.9848f, 0.5f, -0.1736f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			 0.0f, 0.0f, 0.0f,        0.0f, 0.0f,   0.0f, 0.0f, 1.0f,
			-0.9659f, 0.0f, -0.2581f, 1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
			-0.9659f, 0.5f, -0.2581f, 1.0f, 1.0f,   0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,        0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.9848f, 0.5f, -0.1736f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.9659f, 0.5f, -0.2581f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			-0.9848f, 0.0f, -0.1736f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.9848f, 0.5f, -0.1736f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.9659f, 0.5f, -0.2581f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			-0.9659f, 0.0f, -0.2581f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			//230 y 235
			 0.0f, 0.0f, 0.0f,         0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			-0.6427f, 0.0f, -0.7660f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
			-0.6427f, 0.5f, -0.7660f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,         0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
			-0.5735f, 0.0f, -0.8191f,  1.0f, 0.0f,   0.0f, 0.0f, -1.0f,
			-0.5735f, 0.5f, -0.8191f,  1.0f, 1.0f,   0.0f, 0.0f, -1.0f,

			 0.0f, 0.0f, 0.0f,         0.0f, 0.0f,   0.0f, -1.0f, 0.0f,
			-0.6427f, 0.5f, -0.7660f,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f,
			-0.5735f, 0.5f, -0.8191f,  1.0f, 1.0f,   0.0f, -1.0f, 0.0f,

			-0.6427f, 0.0f, -0.7660f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.6427f, 0.5f, -0.7660f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.5735f, 0.5f, -0.8191f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			-0.5735f, 0.0f, -0.8191f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

			 //5
			//250 y 255 grados	
			 0.0f, 0.0f, 0.0f,    0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			-0.5f, 0.0f,-0.866f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			 0.5f, 0.0f,-0.866f,  1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			 0.0f, 0.0f, 0.0f,   0.0f,  0.0f,     0.0f, 0.0f, 1.0f,
			-0.3420f, 0.0f, -0.9396f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.3420f, 0.5f, -0.9396f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f, 0.0f, -1.0f,
			-0.2581f, 0.0f, -0.9659f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			-0.2581f, 0.5f, -0.9659f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			 0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f,
			-0.3420f, 0.5f, -0.9396f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			-0.2581f, 0.5f, -0.9659f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

			-0.3420f, 0.0f, -0.9396f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.3420f, 0.5f, -0.9396f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			-0.2581f, 0.5f, -0.9659f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			-0.2581f, 0.0f, -0.9659f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			//290 y 295
			0.0f, 0.0f, 0.0f,        0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
			0.3420f, 0.0f, -0.9396f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,
			0.3420f, 0.5f, -0.9396f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,

			 0.0f, 0.0f, 0.0f,       0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			0.4226f, 0.0f, -0.9063f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
			0.4226f, 0.5f, -0.9063f, 1.0f, 1.0f,  0.0f, 0.0f, -1.0f,

			0.0f, 0.0f, 0.0f,        0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			0.3420f, 0.5f, -0.9396f, 1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
			0.4226f, 0.5f, -0.9063f, 1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

			0.3420f, 0.0f, -0.9396f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.3420f, 0.5f, -0.9396f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.4226f, 0.5f, -0.9063f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			0.4226f, 0.0f, -0.9063f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		   //6
			//310 y 315
		  0.0f, 0.0f, 0.0f,     0.0f,  0.0f,  0.0f, -1.0f, 0.0f,
		  0.5f, 0.0f, -0.866f,  1.0f, 0.0f,  0.0f, -1.0f, 0.0f,
		  1.0f, 0.0f, 0.0f,     1.0f, 1.0f,  0.0f, -1.0f, 0.0f,

		 0.0f, 0.0f, 0.0f,         0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		 0.6427f, 0.0f, -0.7660f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		 0.6427f, 0.5f, -0.7660f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,

		 0.0f, 0.0f, 0.0f,        0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		 0.7071f, 0.0f, -0.7071f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		 0.7071f, 0.5f, -0.7071f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		 0.6427f, 0.5f, -0.7660f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		 0.7071f, 0.5f, -0.7071f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		 0.6427f, 0.0f, -0.7660f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 0.6427f, 0.5f, -0.7660f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		 0.7071f, 0.5f, -0.7071f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		 0.7071f, 0.0f, -0.7071f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		
		//350 y 355 grados
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.9848f, 0.0f, -0.1736f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.9848f, 0.5f, -0.1736f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.9961f, 0.0f, -0.0871f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.9961f, 0.5f, -0.0871f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, 0.0f,        0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.9848f, 0.5f, -0.1736f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.9961f, 0.5f, -0.0871f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		0.9848f, 0.0f, -0.1736f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.9848f, 0.5f, -0.1736f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.9961f, 0.5f, -0.0871f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		0.9961f, 0.0f, -0.0871f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,

	};
	//9
	Mesh *BaseTriangulo = new Mesh();
	BaseTriangulo->CreateMesh(BaseTriangulo_vertices, BaseTriangulo_indices, 1392, 198);
	meshList.push_back(BaseTriangulo);
}
//-------------------------------------------------------//

void creaBrazos()
{
	unsigned int brazo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat brazo_vertices[] = {
		//1 er
		0.9961f, 0.0f,  -0.0871f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.9961f, 0.15f, -0.0871f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.9848f, 0.15f,  0.1736f, 1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,
		0.9848f, 0.0f,   0.1736f, 0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,

		4.0057f, -2.0f,  -0.3502f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		4.0057f, -1.85f, -0.3502f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		4.0208f, -1.85f, -0.0925f, 1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
		4.0208f, -2.0f,  -0.0925f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

		0.9848f,  0.0f,   0.1736f,  0.0f, 0.0f,  0.0f, 0.0f, -0.3f,
		0.9848f,  0.15f,  0.1736f,  1.0f, 0.0f,  0.0f, 0.0f, -0.3f,
		4.0208f, -1.85f, -0.0925f,  1.0f, 1.0f,  0.0f, 0.0f, -0.3f,
		4.0208f, -2.0f,  -0.0925f,  0.0f, 1.0f,  0.0f, 0.0f, -0.3f,

		0.9961f,  0.0f,  -0.0871f,  0.0f, 0.0f,  0.0f, 0.0f, 0.3f,
		0.9961f,  0.15f, -0.0871f,  1.0f, 0.0f,  0.0f, 0.0f, 0.3f,
		4.0057f, -1.85f, -0.3502f,  1.0f, 1.0f,  0.0f, 0.0f, 0.3f,
		4.0057f, -2.0f,  -0.3502f,  0.0f, 1.0f,  0.0f, 0.0f, 0.3f,

		0.9848f,  0.15f,  0.1736f,  0.0f, 0.0f,  -0.5f, -0.5f, -0.02f,
		0.9961f,  0.15f, -0.0871f,  1.0f, 0.0f,  -0.5f, -0.5f, -0.02f,
		4.0057f, -1.85f, -0.3502f,  1.0f, 1.0f, -0.5f, -0.5f, -0.02f,
		4.0208f, -1.85f, -0.0925f,  0.0f, 1.0f,  -0.5f, -0.5f, -0.02f,

		4.0208f, -2.0f, -0.0925f,  0.0f, 0.0f,  0.5f, 0.5f, -0.02f,
		4.0057f, -2.0f, -0.3502f,  1.0f, 0.0f,  0.5f, 0.5f, -0.02f,
		0.9961f,  0.0f, -0.0871f,  1.0f, 1.0f,  0.5f, 0.5f, -0.02f,
		0.9848f,  0.0f,  0.1736f,  0.0f, 1.0f,  0.5f, 0.5f, -0.02f,

	};
	//calcAverageNormals(brazo_indices, 6*6, brazo_vertices, 32*6, 8, 5);
	Mesh *brazo = new Mesh();
	brazo->CreateMesh(brazo_vertices, brazo_indices, 32 * 6, 6 * 6);
	meshList.push_back(brazo);

}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}
//RECORRIDO

//Recorrido
float rX, rY, rZ;
bool recorrido = false;
int cont = 0;
bool recorridoMont = false;

//Recorrido
typedef struct _frameR
{
}FRAMER;

FRAMER KeyFrameR[12];
int FrameRIndex = 12;
bool playR = false;
int playRIndex = 0;

///////////////////////////////KEYFRAMES/////////////////////
bool animacion = false;

//NEW// Keyframes
float posXdragon = -60.0, posYdragon = 10.0, posZDragon = 20;
float movAvion_x = 0.0f, movAvion_y = 0.0f, movAvion_z = 0.0f;
float sqX = 0.0f, sqY = 0.0f, sqZ = 0.0f;
float posXsq = 0.0, posYsq = 0.0, posZsq = 50.0f;

float giroDragon = 0.0f, giroDragon2 = 0.0f;
float rotAlas = 0.0f;
//bool recorridoSq = 0;

#define MAX_FRAMES 20
int i_max_steps = 90;
int i_curr_steps = 5;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float movAvion_x;		//Variable para PosicionX
	float movAvion_y;		//Variable para PosicionY
	float movAvion_z;		//Variable para PosicionZ
	float movAvion_xInc;		//Variable para IncrementoX
	float movAvion_yInc;		//Variable para IncrementoY
	float movAvion_zInc;		//Variable para IncrementoZ
	float giroDragon;
	float giroDragonInc;
	float giroDragon2;
	float giroDragon2Inc; 
	float rotAlas;
	float rotAlasInc;
	float sqX;
	float sqY;
	float sqZ;
	float sqX_inc;
	float sqY_inc;
	float sqZ_inc;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 20;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{

	//printf("frameindex %d\n", FrameIndex);
	KeyFrame[FrameIndex].movAvion_x = movAvion_x;
	KeyFrame[FrameIndex].movAvion_y = movAvion_y;
	KeyFrame[FrameIndex].movAvion_z = movAvion_z;
	KeyFrame[FrameIndex].giroDragon = giroDragon;
	KeyFrame[FrameIndex].giroDragon2 = giroDragon2;
	KeyFrame[FrameIndex].rotAlas = rotAlas;
	KeyFrame[FrameIndex].sqX = sqX;
	KeyFrame[FrameIndex].sqY = sqY;
	KeyFrame[FrameIndex].sqZ = sqZ;

	FrameIndex++;
}

void resetElements(void)
{
	
	movAvion_x = KeyFrame[0].movAvion_x;
	movAvion_y = KeyFrame[0].movAvion_y;
	movAvion_z = KeyFrame[0].movAvion_z;
	giroDragon= KeyFrame[0].giroDragon;
	giroDragon2 = KeyFrame[0].giroDragon2;
	rotAlas = KeyFrame[0].rotAlas;
	sqX = KeyFrame[0].sqX;
	sqY = KeyFrame[0].sqY;
	sqZ = KeyFrame[0].sqZ;

}

void interpolation(void)
{
	KeyFrame[playIndex].movAvion_xInc = (KeyFrame[playIndex + 1].movAvion_x - KeyFrame[playIndex].movAvion_x) / i_max_steps;
	KeyFrame[playIndex].movAvion_yInc = (KeyFrame[playIndex + 1].movAvion_y - KeyFrame[playIndex].movAvion_y) / i_max_steps;
	KeyFrame[playIndex].movAvion_zInc = (KeyFrame[playIndex + 1].movAvion_z - KeyFrame[playIndex].movAvion_z) / i_max_steps;
	KeyFrame[playIndex].giroDragonInc = (KeyFrame[playIndex + 1].giroDragon - KeyFrame[playIndex].giroDragon) / i_max_steps;
	KeyFrame[playIndex].giroDragon2Inc = (KeyFrame[playIndex + 1].giroDragon2 - KeyFrame[playIndex].giroDragon2) / i_max_steps;
	KeyFrame[playIndex].rotAlasInc = (KeyFrame[playIndex + 1].rotAlas - KeyFrame[playIndex].rotAlas) / i_max_steps;
	KeyFrame[playIndex].sqX_inc = (KeyFrame[playIndex + 1].sqX - KeyFrame[playIndex].sqX) / i_max_steps;
	KeyFrame[playIndex].sqY_inc = (KeyFrame[playIndex + 1].sqY - KeyFrame[playIndex].sqY) / i_max_steps;
	KeyFrame[playIndex].sqZ_inc = (KeyFrame[playIndex + 1].sqZ - KeyFrame[playIndex].sqZ) / i_max_steps;

}

void animate(void)
{
	//Movimiento del objeto
	if (play)
	{	
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			//printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				//printf("Frame index= %d\n", FrameIndex);
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				//printf("entro aquí\n");
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//printf("se quedó aqui\n");
			//printf("max steps: %f", i_max_steps);
			//Draw animation
			movAvion_x += KeyFrame[playIndex].movAvion_xInc;
			movAvion_y += KeyFrame[playIndex].movAvion_yInc;
			movAvion_z += KeyFrame[playIndex].movAvion_zInc;
			giroDragon += KeyFrame[playIndex].giroDragonInc;
			giroDragon2 += KeyFrame[playIndex].giroDragon2Inc;
			rotAlas += KeyFrame[playIndex].rotAlasInc;
			sqX += KeyFrame[playIndex].sqX_inc;
			sqY += KeyFrame[playIndex].sqY_inc;
			sqZ += KeyFrame[playIndex].sqZ_inc;

			i_curr_steps++;
		}

	}
}

/* FIN KEYFRAMES*/

float movRecorrido() {
	float Reco[] = { -0.365465, 0.278234, 128.682175
   , -0.616343, 0.469231, 127.777542
   , -0.752767, 0.573092, 127.285614
   , -1.026232, 0.781285, 126.299538
   , -1.026232, 0.781285, 126.299538
   , -1.112031, 0.846605, 125.990158
   , -1.461327, 1.112529, 124.730644
   , -1.948754, 1.483614, 122.973038
   , -2.299326, 1.750510, 121.708916
   , -2.740139, 2.098551, 119.849342
   , -2.972240, 2.258693, 118.655281
   , -3.365858, 2.560009, 116.442383
   , -3.471861, 2.661280, 115.773102
   , -3.711913, 2.890615, 114.257477
   , -3.885397, 3.056355, 113.162148
   , -4.042408, 3.206356, 112.170815
   , -4.353299, 3.529828, 110.029060
   , -4.473593, 3.701820, 108.884537
   , -4.551463, 3.813154, 108.143661
   , -4.631977, 3.928271, 107.377617
   , -4.722271, 4.057369, 106.518532
   , -4.857934, 4.251336, 105.227783
   , -5.022285, 4.486320, 103.664093
   , -5.056556, 4.535318, 103.338038
   , -5.160316, 4.683670, 102.350815
   , -5.324276, 4.918094, 100.790833
   , -5.324276, 4.918094, 100.790834
   , -5.459138, 5.110917, 99.507698
   , -5.523116, 5.110917, 99.193237
   , -5.611937, 5.107028, 98.756668
   , -5.893230, 5.048222, 93.967636
   , -5.879997, 4.992981, 93.097544
   , -6.538341, 4.893899, 91.116644
   , -6.621172, 4.892251, 90.946815
   , -7.259467, 4.871477, 89.638123
   , -7.957078, 4.669010, 88.20780
   , -8.667078, 4.441386, 86.75208
 , -8.816249, 4.393562, 86.446236
   , -9.614968, 4.137494, 84.80861
   , -9.693785, 4.112225, 84.64701
   , -10.312939, 3.913725, 83.377563
   , -10.393941, 3.887756, 83.211487
   , -10.469992, 3.863375, 83.055557
   , -11.253526, 3.611041, 80.757599
   , -11.253526, 3.611041, 80.757599
   , -11.643114, 3.477711, 79.251167
   , -11.643114, 3.477711, 79.251167
   , -12.449708, 3.184769, 76.132294
   , -12.600256, 3.126873, 75.550171
   , -12.853838, 3.029352, 74.569641
   , -12.853838, 3.029352, 74.569641
   , -13.017173, 2.966538, 73.938072
   , -13.017173, 2.966538, 73.938072
   , -13.425824, 2.809383, 72.357941
   , -13.425824, 2.809383, 72.357941
   , -13.425824, 2.809383, 72.357941
   , -13.702333, 2.703045, 71.288765
   , -13.702333, 2.703045, 71.288765
   , -14.645091, 2.432866, 67.958191
   , -14.645091, 2.432866, 67.958191
   , -15.074125, 2.340474, 66.509796
   , -15.074125, 2.340474, 66.509796
   , -15.074125, 2.340474, 66.509796
   , -15.427739, 2.268700, 65.390961
   , -15.427739, 2.268700, 65.390961
   , -15.550605, 2.245618, 65.034134
   , -15.951004, 2.170397, 63.871288
   , -15.951004, 2.170397, 63.871288
   , -17.042057, 2.234251, 61.740860
   , -17.042057, 2.234251, 61.740860
 , -17.042057, 2.234251, 61.740860
   , -17.272730, 2.219050, 61.371708
   , -17.272730, 2.219050, 61.371708
   , -17.272730, 2.219050, 61.371708
   , -18.400919, 2.226977, 59.709740
   , -18.400919, 2.226977, 59.709740
 , -18.896406, 2.253896, 59.119240
   , -18.896406, 2.253896, 59.119240
   , -18.896406, 2.253896, 59.119240
   , -20.449671, 2.297379, 57.250931
   , -20.833660, 2.281226, 56.768192
   , -20.833660, 2.281226, 56.768192
   , -21.622967, 2.248024, 55.775898
   , -21.622967, 2.248024, 55.775898
   , -22.006920, 2.231874, 55.293201
  , 23.018387, 2.189327, 54.021610
   , -23.392927, 2.173572, 53.550747
   , -24.149048, 2.141765, 52.600174
   , -24.149048, 2.141765, 52.600174
   , -24.525429, 2.125933, 52.126999
   , -26.731903, 2.033118, 49.353085
   , -28.306932, 1.966865, 47.373001
   , -28.306932, 1.966865, 47.373001
   , -28.306932, 1.966865, 47.373001
   , -28.851433, 1.943961, 46.688473
   , -29.641132, 1.910742, 45.695686
   , -30.737514, 1.864623, 44.317345
   , -30.737514, 1.864623, 44.317345
   , -30.737514, 1.864623, 44.317345
   , -32.676460, 1.783062, 41.879753
   , -32.676460, 1.783062, 41.879753
   , -32.676460, 1.783062, 41.879753
   , -33.209923, 1.760622, 41.209099
   , -33.209923, 1.760622, 41.209099
   , -34.832409, 1.692373, 39.169357
   , -34.832409, 1.692373, 39.169357
   , -34.832409, 1.692373, 39.169357
   , -35.643803, 1.658242, 38.149292
   , -35.643803, 1.658242, 38.149292
   , -35.643803, 1.658242, 38.149292
   , -36.411278, 1.625958, 37.184441
   , -36.411278, 1.625958, 37.184441
   , -36.411278, 1.625958, 37.184441
   , -36.411278, 1.625958, 37.184441
   , -36.411278, 1.625958, 37.184441
   , -37.660675, 1.843564, 36.167011
   , -39.870731, 2.300558, 34.871082
   , -39.870731, 2.300558, 34.871082
   , -40.227180, 2.373503, 34.661118
   , -40.967140, 2.524931, 34.225246
   , -40.967140, 2.524931, 34.225246
   , -41.689522, 2.672762, 33.799732
   , -43.774357, 3.009296, 32.571671
   , -43.774357, 3.009296, 32.571671
   , -43.774357, 3.009296, 32.571671
   , -43.774357, 3.009296, 32.571671
   , -46.176857, 3.185511, 30.981495
   , -46.691605, 2.931363, 29.902300
   , -46.691605, 2.931363, 29.902300
   , -46.691605, 2.931363, 29.902300
   , -46.691605, 2.931363, 29.902300
   , -47.295647, 2.674525, 28.576847
   , -47.295647, 2.674525, 28.576847
   , -47.295647, 2.674525, 28.576847
   , -47.295647, 2.674525, 28.576847
   , -47.295647, 2.674525, 28.576847
   , -47.295647, 2.674525, 28.576847
   , -47.720703, 2.493792, 27.644146
   , -47.820267, 2.451457, 27.425671
   , -48.290329, 2.251586, 26.394209
   , -49.011494, 1.944947, 24.811756
   , -49.011494, 1.944947, 24.811756
   , -49.011494, 1.944947, 24.81175
   , -50.083939, 1.393499, 21.702005
   , -50.083939, 1.393499, 21.702005
   , -50.083939, 1.393499, 21.702005
 , -50.171345, 1.335428, 21.323404
   , -50.586834, 1.047418, 21.395555
   , -50.586834, 1.047418, 21.395555
   , -50.586834, 1.047418, 21.395555
 , -50.590492, 1.014438, 21.814585
   , -50.151554, 0.824581, 25.674416
   , -48.056660, 0.737987, 26.804007
   , -48.056660, 0.737987, 26.804007
   , -45.987484, 0.663889, 27.273815
   , -45.987484, 0.663889, 27.273815
 , -45.387203, 0.642187, 27.434660
   , -45.387203, 0.642187, 27.434660
   , -44.083401, 0.594939, 27.796236
   , -44.083401, 0.594939, 27.796236
   , -44.083401, 0.594939, 27.796236
   , -43.260284, 0.565072, 28.028509
   , -43.260284, 0.565072, 28.028509
   , -42.035446, 0.520346, 28.40297
   , -42.035446, 0.520346, 28.402979
   , -40.453758, 0.462588, 28.886549
   , -40.453758, 0.462588, 28.886549
   , -40.135807, 0.450978, 28.983755
   , -38.838341, 0.403599, 29.380432
   , -38.838341, 0.403599, 29.380432
   , -38.838341, 0.403599, 29.380432
   , -38.489403, 0.390857, 29.487114
   , -37.827835, 0.366699, 29.689375
   , -35.511848, 0.299735, 30.397444
   , -33.529907, 0.354005, 31.003386
   , -31.900461, 0.398623, 31.501556
   , -31.900461, 0.398623, 31.501556
   , -31.900461, 0.398623, 31.501556
   , -31.900461, 0.398623, 31.501556
   , -31.900461, 0.398623, 31.501556
   , -30.721281, 0.430912, 31.862070
   , -30.721281, 0.430912, 31.862070
   , -30.325680, 0.441744, 31.983017
   , -30.325680, 0.441744, 31.983017
   , -29.542755, 0.463183, 32.222382
   , -29.542755, 0.463183, 32.222382
   , -29.542755, 0.463183, 32.22238
   , -27.303205, 0.524507, 32.907078
   , -27.303205, 0.524507, 32.907078
   , -27.303205, 0.524507, 32.907078
   , -27.303205, 0.524507, 32.907078
   , -26.135111, 0.556492, 33.264202
   , -26.135111, 0.556492, 33.264202
 , -26.135111, 0.556492, 33.264202
   , -25.318893, 0.578842, 33.513744
   , -24.543846, 0.600065, 33.750698
   , -23.577600, 0.626523, 34.046108
   , -23.577600, 0.626523, 34.046108
   , -23.577600, 0.626523, 34.046108
   , -22.804379, 0.647696, 34.282509
   , -22.804379, 0.647696, 34.282509
   , -22.804379, 0.647696, 34.282509
   , -21.213114, 0.691269, 34.769005
   , -20.431503, 0.712671, 35.007969
   , -20.431503, 0.712671, 35.007969
   , -20.431503, 0.712671, 35.007969
   , -20.431503, 0.712671, 35.007969
   , -19.465805, 0.739114, 35.303211
   , -19.465805, 0.739114, 35.303211
   , -19.465805, 0.739114, 35.303211
   , -19.465805, 0.739114, 35.303211
   , -19.465805, 0.739114, 35.303211
   , -18.698195, 0.760133, 35.537891
  , -18.698195, 0.760133, 35.537891
   , -18.698195, 0.760133, 35.537891
   , -18.698195, 0.760133, 35.537891
   , -18.102203, 0.776453, 35.720104
   , -18.102203, 0.776453, 35.720104
   , -18.102203, 0.776453, 35.720104
   , -16.813656, 0.811736, 36.114052
   , -16.813656, 0.811736, 36.114052
   , -16.813656, 0.811736, 36.114052
   , -16.813656, 0.811736, 36.114052
   , -16.636860, 0.816577, 36.168102
   , -15.666639, 0.843144, 36.464733
   , -15.666639, 0.843144, 36.464733
  , -15.666639, 0.843144, 36.464733
   , -15.666639, 0.843144, 36.464733
   , -15.666639, 0.843144, 36.464733
   , -15.666639, 0.843144, 36.464733
   , -15.453632, 0.848977, 36.529858
   , -14.295932, 0.880678, 36.883801
   , -13.293729, 0.908120, 37.1902
   , -12.338167, 0.934286, 37.482346
   , -12.338167, 0.934286, 37.482346
   , -12.338167, 0.934286, 37.482346
   , -12.338167, 0.934286, 37.482346
   , -12.338167, 0.934286, 37.482346
   , -11.549665, 0.955877, 37.723415
   , -11.549665, 0.955877, 37.723415
   , -11.549665, 0.955877, 37.723415
   , -11.549665, 0.955877, 37.723415
   , -11.549665, 0.955877, 37.723415
   , -11.549665, 0.955877, 37.723415
   , -10.394517, 0.987508, 38.076576
   , -10.394517, 0.987508, 38.076576
   , -8.492987, 1.039576, 38.657940
   , -8.492987, 1.039576, 38.657940
   , -7.888827, 1.056120, 38.842651
   , -7.888827, 1.056120, 38.842651
   , -7.888827, 1.056120, 38.842651
   , -7.888827, 1.056120, 38.842651
   , -7.888827, 1.056120, 38.842651
   , -4.855048, 1.139192, 39.770176
   , -4.855048, 1.139192, 39.770176
   , -4.855048, 1.139192, 39.770176
   , -4.070666, 1.160670, 40.009987
   , -4.070666, 1.160670, 40.009987
   , -4.070666, 1.160670, 40.009987
  , -4.070666, 1.160670, 40.009987
   , -3.853246, 1.166624, 40.076458
   , -3.134651, 1.186301, 40.296154
   , -3.134651, 1.186301, 40.296154
   , -3.134651, 1.186301, 40.296154
   , -3.134651, 1.186301, 40.296154
   , -2.142440, 1.213470, 40.599503
   , -2.142440, 1.213470, 40.599503
   , -2.142440, 1.213470, 40.599503
   , -2.142440, 1.213470, 40.599503
  ,0.808083, 1.294262, 41.501568
  , 0.808083, 1.294262, 41.501568
  ,0.808083, 1.294262, 41.501568
  ,0.808083, 1.294262, 41.501568
  ,0.808083, 1.294262, 41.501568
  ,1.886541, 1.323793, 41.831284
  ,1.886541, 1.323793, 41.831284
  ,1.886541, 1.323793, 41.831284
  ,1.886541, 1.323793, 41.831284
  ,1.886541, 1.323793, 41.831284
  ,1.886541, 1.323793, 41.831284
  ,2.723546, 1.346712, 42.087185
  ,2.723546, 1.346712, 42.087185
  ,2.723546, 1.346712, 42.087185
  ,8.069444, 1.419798, 43.264549
  ,8.069444, 1.419798, 43.264549
  ,8.069444, 1.419798, 43.264549
  ,8.069444, 1.419798, 43.264549
  ,9.295653, 1.331232, 43.581669
  ,9.295653, 1.331232, 43.581669
  ,10.257877, 1.322559, 43.830521
  ,10.257877, 1.322559, 43.830521
  ,10.736016, 1.318249, 43.954174
  ,11.155621, 1.314466, 44.062691
  ,13.894497, 1.289778, 44.771011
  ,14.273293, 1.286364, 44.868977
  ,15.287108, 1.277225, 45.131168
  ,15.287108, 1.277225, 45.131168
  ,16.115681, 1.269756, 45.345455
  ,16.115681, 1.269756, 45.345455
  ,16.115681, 1.269756, 45.345455
  ,16.632633, 1.265097, 45.479149
  ,16.632633, 1.265097, 45.47914
  ,18.173334, 1.251209, 45.877602
  ,18.173334, 1.251209, 45.877602
  ,19.277113, 1.241259, 46.163055
  ,19.277113, 1.241259, 46.163055
  ,20.139109, 1.233489, 46.385983
  ,22.902950, 1.208576, 47.100761
  ,22.902950, 1.208576, 47.100761
  ,22.902950, 1.208576, 47.100761
  ,25.273653, 1.187134, 47.744896
  ,25.273653, 1.187134, 47.744896
  ,25.273653, 1.187134, 47.744896
  ,25.746710, 1.182828, 47.885021
  ,25.746710, 1.182828, 47.885021
  ,26.849874, 1.172788, 48.211796
  ,27.330099, 1.168417, 48.354046
  ,27.972424, 1.162571, 48.544312
  ,31.076468, 1.141430, 49.471207
  ,31.076468, 1.141430, 49.471207
  ,31.890194, 1.141430, 49.719986
  ,31.890194, 1.141430, 49.719986
  ,31.890194, 1.141430, 49.719986
  ,32.734402, 1.145116, 49.978085
  ,32.734402, 1.145116, 49.978085
  ,32.734402, 1.145116, 49.978085
  ,33.514885, 1.166487, 50.216702
  ,33.514885, 1.166487, 50.216702
  ,36.297146, 1.242672, 51.067326
  ,36.297146, 1.242672, 51.067326
  ,36.297146, 1.242672, 51.067326
  ,36.696686, 1.253613, 51.189476
  ,37.485477, 1.275212, 51.430634
  ,37.485477, 1.275212, 51.430634
  ,37.485477, 1.275212, 51.4306
  ,40.300705, 1.352299, 52.291332
  ,40.300705, 1.352299, 52.291332
  ,40.300705, 1.352299, 52.291332
  ,40.300705, 1.352299, 52.291332
  ,41.620834, 1.388448, 52.694931
  ,41.620834, 1.388448, 52.694931
  ,41.620834, 1.388448, 52.694931
  ,43.107544, 1.429157, 53.149464
  ,43.107544, 1.429157, 53.149464
  ,43.107544, 1.429157, 53.149464
  ,43.107544, 1.429157, 53.149464
  ,43.107544, 1.429157, 53.149464
  ,44.128456, 1.477251, 52.840004
  ,44.128456, 1.477251, 52.840004
  ,44.128456, 1.477251, 52.840004
  ,44.170410, 1.491267, 52.440838
  ,44.287254, 1.525532, 51.466602
  ,44.27254, 1.525532, 51.466602
  ,44.287254, 1.525532, 51.466602
  ,44.330669, 1.534242, 51.136845
  ,43.080681, 1.677424, 49.432529
  ,43.080681, 1.677424, 49.432529
  ,43.080681, 1.677424, 49.432529
  ,42.424671, 1.739571, 49.072498
  ,42.424671, 1.739571, 49.072498
  ,42.424671, 1.739571, 49.072498
  ,42.424671, 1.739571, 49.072498
  ,42.424671, 1.739571, 49.072498
  ,42.424671, 1.739571, 49.072498
  ,41.370743, 1.848176, 48.670864
  ,41.370743, 1.848176, 48.670864
  ,41.370743, 1.848176, 48.670864
  ,40.741127, 1.915918, 48.356949
  ,40.741127, 1.915918, 48.356949
  ,40.741127, 1.915918, 48.356949
  ,38.627777, 2.145332, 47.256809
  ,38.627777, 2.145332, 47.256809
  ,38.627777, 2.145332, 47.256809
  ,38.627777, 2.145332, 47.256809
  ,35.436291, 2.856925, 43.045685
  ,35.436291, 2.856925, 43.045685
  ,35.436291, 2.856925, 43.045685
 ,35.317387, 3.013900, 42.00209
  ,35.205242, 3.161955, 41.017799
  ,35.205242, 3.161955, 41.017799
  ,35.083351, 3.322877, 39.947968
  ,35.083351, 3.322877, 39.947968
  ,35.083351, 3.322877, 39.947968
  ,35.021698, 3.404274, 39.406826
  ,34.926216, 3.530328, 38.568802
  ,34.926216, 3.530328, 38.568802
  ,34.804630, 3.690845, 37.501667
  ,34.804630, 3.690845, 37.501667
  ,34.674629, 3.862468, 36.360687
  ,34.610649, 3.946933, 35.799156
  ,34.362431, 4.274630, 33.620579
  ,34.362431, 4.274630, 33.6205
  ,34.300579, 4.356287, 33.077713
  ,34.300579, 4.356287, 33.0777
  ,34.172699, 4.525114, 31.955324
  ,34.172699, 4.525114, 31.955324
  ,33.975170, 4.785894, 30.221622
  ,33.913326, 4.867539, 29.67883
  ,33.770241, 5.045179, 28.422983
  ,33.705772, 5.125218, 27.85713
  ,33.644604, 5.201159, 27.32026
  ,33.644604, 5.201159, 27.320267
  ,33.202419, 5.750134, 23.439217
  ,33.202419, 5.750134, 23.439217
  ,33.141884, 5.825289, 22.907902
  ,32.892712, 6.134635, 20.720934
  ,32.892712, 6.134635, 20.720934
  ,32.892712, 6.134635, 20.720934
  ,32.714088, 6.356394, 19.153185
  ,32.714088, 6.356394, 19.153185
	};
	return Reco[cont];
}

/*Para la animacion de la montaña Rusa*/
float movX() {
	float x[] = {
		15.5995, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
		26.2792, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
		39.8715, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
		54.111, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68,
		69.3215, 70, 71, 72, 73, 74, 75, 76,
		77.4121, 78, 79,
		80.0011, 80.01945867018736, 80.04976851035141, 80.08007835051546, 80.11038819067952, 80.14069803084355, 80.1710078710076, 80.20131771117165, 80.2316275513357, 80.26193739149976, 80.29224723166381, 80.32255707182786,
		80.3248, 80.30961487618185, 80.1993780426563, 80.08914120913076, 79.97890437560523, 79.86866754207968, 79.75843070855413, 79.64819387502858, 79.53795704150303, 79.4277202079775, 79.31748337445195, 79.2072465409264, 79.09700970740086, 78.98677287387531, 78.87653604034978, 78.76629920682423, 78.65606237329868, 78.54582553977313, 78.4355887062476, 78.32535187272205, 78.2151150391965, 78.10487820567096,
		78.0594, 78.0, 77.5, 77,
		76.7649, 76.0, 75.5, 75.0, 74.5, 74,
		73.8522, 73.0, 72.5, 72.0, 71.5, 71.0, 70.5, 70.0, 69.5, 69.0, 68.5, 68,
		67.7033, 66, 65, 64, 63, 62, 61, 60, 59.2891, 58, 57, 56, 55,
		54.7583, 53, 52, 51, 50, 49, 48, 47.3149, 46, 45, 44, 43, 42, 41, 40, 39,
		38.2533, 37, 36, 35, 34, 33, 32, 31.4572, 30, 29, 28, 27, 26,
		25.9555, 24, 23, 22, 21.1012, 20, 19, 18, 17, 16, 15, 14, 13,
		12.6869, 11, 10, 9, 8, 7.8325, 6, 5, 4, 3, 2.3308, 1, 0, -1, -2,
		-3.1708, -4, -5, -6, -7, -8, -9, -10.6142, -11, -12, -13, -14, -15,
		-16.1159, -17, -18, -19, -20, -21, -22.912, -23, -24, -25, -26, -27, -28,
		-29.0609, -30, -31, -32, -33, -34.8862, -35, -36, -37, -38, -39, -40, -41,
		-42.3296, -43, -44, -45, -46, -47, -48, -49, -50.4202, -51, -52, -53, -54, -55, -56, -57, -58,
		-59.8054, -60, -61, -62, -63, -64, -65, -66, -67, -68, -69, -70.1615, -71, -72, -73, -74, -75, -76, -77, -78, -79,
		-80.5175, -81, -82, -83, -84, -85, -86, -87, -88.6081, -89, -90, -91, -92, -93, -94, -95,
		-96.6988, -97, -98, -99, -100, -101, -102, -103, -104, -105,
		-106.7312, -107, -108, -109, -110, -111, -112, -113, -114, -115, -116, -117,
		-118.0581, -119, -120, -121, -122, -123,
		-124.8543, -125, -125.27452292508683, -125.51452688024521, -125.75453083540361, -125.994534790562, -126.23453874572039, -126.47454270087879, -126.71454665603717
		- 126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961, -126.7961,
		-126.7961, -125, -124,
		-123.5598, -122, -121, -120, -119,
		-118.3818, -117, -116, -115, -114, -113, -112, -111, -110,
		-109.3202, -108, -107, -106, -105, -104, -103, -102, -101, -100, -99, -98,
		-97.9933, -96, -95, -94, -93, -92, -91, -90, -89, -88, -87, -86, -85,
		-84.7246, -83, -82, -81, -80, -79, -78, -77, -76, -75, -74, -73, -72, -71, -70, -69, -68,
		-67.2488, -66, -65, -64, -63, -62, -61, -60, -59, -58, -57, -56, -55, -54, -53, -52,
		-51.7148, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40, -39,
		-38.7697, -37, -36, -35, -34, -33, -32, -31, -30, -29, -28,
		-27.4428, -26, -25, -24, -23, -22, -21, -20,
		-19.6758, -18, -17, -16, -15, -14, -13, -12, -11, -10,
		-9.6433, -8, -7, -6, -5, -4, -3, -2, -1,
		-0.9054, 1, 2, 3, 4, 5,
		6.2143, 7, 8, 9, 10, 11,
		12.6869, 13, 14, 15, 16, 17, 18, 19,
		20.7775, 21, 22, 23, 24, 25, 26, 27,
		28.2209, 29, 30, 31, 32, 33,
		34.3698, 35, 36, 37, 38, 39, 40, 41,
		42.1369, 43, 44, 45, 46, 47, 48,
		49.9039, 50, 51, 52, 53, 54, 55, 56, 57,
		58.3182, 59, 60, 61, 62, 63,
		64.4671, 65, 66, 67, 68,
		69.9687, 70, 71, 72,
		73.8522, 74, 74, 74.5, 75.0,
		75.794, 76,
		76.1176, 75, 74,
		73.8522, 72, 71, 70, 69,
		68.027, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58,
		57.3473, 56, 55, 54,
		53.1779, 52, 51, 50,
		49.7567, 48, 47, 46,
		45.3731, 44, 43, 42, 41, 40,
		39.5479, 38, 37, 36, 35, 34, 33, 32,
		31.1336, 30, 29, 28, 27, 26,
		25.3083, 24, 23, 22, 21, 20, 19,
		18.8358, 17, 16, 15, 14, 13,
		12.6869, 11, 10, 9, 8,
		7.5089, 6, 5, 4, 3, 2, 1,
		0.0654, -1, -2, -3, -4, -5, -6, -7, -8, -9, -10,
		-11.2615, -12, -13, -14, -15, -16, -17, -18, -19, -20,
		-21.6175, -22, -23, -24, -25, -26, -27,
		-28.09, -29, -30, -31,
		-32.2972, -32.2309, -32.16196, -32.09299, -32.0240,
		-31.6499, -30,
		-29.0609, -28, -27,
		-26.0803, -25, -24, -23, -22,
		-21.6175, -20, -19, -18, -17, -16, -15,
		-14.8214, -13, -12, -11, -10, -9, -8, -7,
		-6.7307, -5, -4, -3, -2, -1, 0, 1,
		2.9781, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14

	};
	return x[mov];
}

float movY() {
	float y[] = {
		-59.02, -59.032135340880366, -59.062435817485515, -59.09273629409066, -59.12303677069581, -59.15333724730095, -59.1836377239061, -59.21393820051125, -59.2442386771164, -59.27453915372155, -59.3048396303267,
		-59.3436, -59.360760515880315, -59.38456811282859, -59.40837570977686, -59.432183306725136, -59.4559909036734, -59.479798500621676, -59.50360609756995, -59.52741369451822, -59.5512212914665, -59.575028888414764, -59.59883648536304, -59.62264408231131,
		-59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672,
		-59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672, -59.6672,
		-59.6672, -59.45007932909797, -59.130078340296144, -58.81007735149433, -58.490076362692506, -58.17007537389069, -57.85007438508887, -57.53007339628705,
		-57.0782, -55.60844999999999, -53.10844999999998,
		-50.6057, -50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40,
		-28.2755, -28, -26, -24, -22, -20, -18, -16, -14, -12, -10, -8, -6, -4, -2, 0, 2, 4, 6, 8, 10, 12,
		12.8251, 13.240899999999977, 16.74089999999998, 20.240899999999982,
		21.8866, 23.841300676348396, 25.119049843787543, 26.396799011226687, 27.67454817866583, 28.952297346104977,
		29.33, 29.643970609377284, 29.82818241961977, 30.01239422986225, 30.196606040104733, 30.380817850347217, 30.565029660589698, 30.74924147083218, 30.933453281074662, 31.117665091317146, 31.30187690155963, 31.48608871180211,
		31.5954, 29.36798, 28.0603, 26.7526, 25.4449, 24.1372, 22.8295, 21.5217,
		20.5921, 19.487169336541008, 18.630035938465614, 17.77290254039022, 16.915769142314826,
		16.7086, 16.63213471, 16.58864652, 16.545158332, 16.501670, 16.458182, 16.414694,
		16.3849, 17.183234215811776, 17.79037864946588, 18.397523083119978,
		21.8866, 23.19959500301644, 24.247225261547065, 25.294855520077693, 26.34248577860832, 27.39011603713895, 28.437746295669577,
		29.0064, 30.034997742515948, 30.740870472762964, 31.446743203009976, 32.15261593325699, 32.858488663504005,
		32.8899, 32.8899, 32.8899, 32.8899,
		32.8899, 32.25459281104786, 31.677670191222084, 31.10074757139631, 30.52382495157054, 29.946902331744766, 29.369979711918994, 28.79305709209322, 28.21613447226745,
		28.0355, 27.248275366677653, 26.781605953361897, 26.314936540046144, 25.848267126730388,
		25.7701, 25.877884321209805, 25.93670250468037, 25.995520688150933, 26.054338871621496,
		26.0937, 26.563408346662786, 26.91636021521012, 27.269312083757452, 27.622263952304785,
		28.0355, 28.540234309589703, 29.148934634709942, 29.757634959830185, 30.366335284950424, 30.975035610070666, 31.583735935190905,
		32.5663, 33.020176165548826, 34.196630715960524, 35.373085266372215, 36.549539816783906, 37.7259943671956,
		39.0388, 39.796609292094, 40.65376265946646, 41.51091602683893, 42.36806939421139, 43.22522276158385,
		44.8641, 44.89651984745239, 45.26492720486591, 45.63333456227943, 46.001741919692954, 46.37014927710647, 46.738556634519995,
		47.1294, 46.920712902339794, 46.69849258750622, 46.47627227267265, 46.25405195783908,
		45.8349, 45.77552628637451, 45.253788908294595, 44.73205153021468, 44.21031415213478, 43.688576774054866, 43.166839395974954, 42.64510201789505,
		41.9514, 41.62960700566089, 41.14960552245816, 40.66960403925543, 40.189602556052705, 39.70960107284998, 39.22959958964724, 38.749598106444516,
		38.0679, 37.98792822529089, 37.849998293057155, 37.71206836082342, 37.5741384285897, 37.43620849635597, 37.298278564122235, 37.16034863188851, 37.02241869965478,
		36.7734, 36.81596880872143, 37.034719110475955, 37.25346941223047, 37.472219713985, 37.690970015739516, 37.90972031749404, 38.12847061924856, 38.347220921003085, 38.565971222757604, 38.78472152451212,
		39.0388, 39.5628625, 40.1878625, 40.8128625, 41.4378625, 42.0628625, 42.6878625, 43.3128625, 43.9378625, 44.5628625,
		45.5113, 45.97450143129063, 46.934504397696095, 47.89450736410155, 48.85451033050701, 49.81451329691247, 50.77451626331793, 51.73451922972339,
		53.2783, 53.54479316251994, 54.22479612888873, 54.90479909525752, 55.58480206162631, 56.26480502799511, 56.9448079943639, 57.62481096073269,
		58.78, 58.916026968621665, 59.36764373031378, 59.8192604920059, 60.27087725369802, 60.72249401539013, 61.174110777082255, 61.62572753877437, 62.077344300466486, 62.52896106215861,
		63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108, 63.3108,
		63.3108, 62.81742578941173, 62.2936183676172, 61.76981094582266, 61.24600352402813, 60.722196102233596,
		59.7509, 59.14382667112986, 58, 57, 56, 55, 54, 53, 52,
		51.6602, 50, 49, 48, 47, 46, 45, 44, 43, 42,
		42.5987, 36.13278439885055, 32.53280911843774,
		30.9481, 28.608400000000007, 27.108400000000003, 25.608400000000003, 24.108400000000003,
		23.1811, 21.601917427385896, 20.4590728966187, 19.316228365851508, 18.173383835084316, 17.03053930431712, 15.887694773549928, 14.744850242782734, 13.60200571201554,
		12.8251, 11.844376336861806, 11.10151641932038, 10.35865650177895, 9.615796584237522, 8.872936666696095, 8.130076749154664, 7.387216831613237, 6.644356914071809, 5.901496996530381, 5.158637078988952, 4.415777161447524,
		4.4108, 3.4384490100763436, 2.950639352762515, 2.4628296954486864, 1.9750200381348582, 1.48721038082103, 0.9994007235072013, 0.5115910661933731, 0.02378140887954494, -0.4640282484342837, -0.9518379057481123, -1.439647563061941, -1.9274572203757687,
		-2.0618, -2.3811645223680737, -2.5663463166206975, -2.751528110873321, -2.936709905125945, -3.1218916993785686, -3.3070734936311923, -3.492255287883816, -3.6774370821364397, -3.8626188763890634, -4.047800670641687, -4.232982464894311, -4.418164259146934, -4.603346053399559, -4.7885278476521815, -4.973709641904806, -5.158891436157429,
		-5.298, -5.115881323548345, -4.970046380842024, -4.824211438135702, -4.67837649542938, -4.532541552723059, -4.386706610016738, -4.240871667310416, -4.095036724604094, -3.9492017818977727, -3.803366839191451, -3.6575318964851298, -3.5116969537788085, -3.365862011072487, -3.220027068366165, -3.0741921256598435,
		-3.0326, -2.1752066233555567, -1.675210485820891, -1.1752143482862252, -0.6752182107515594, -0.17522207321689365, 0.3247740643177721, 0.8247702018524379, 1.3247663393871032, 1.824762476921769, 2.3247586144564347, 2.8247547519911005, 3.3247508895257663,
		3.4399, 4.299462768277287, 4.785173774819236, 5.270884781361184, 5.756595787903133, 6.242306794445081, 6.728017800987031, 7.213728807528979, 7.699439814070928, 8.185150820612877, 8.670861827154825,
		8.9415, 9.36232131067336, 9.653991196086004, 9.945661081498647, 10.23733096691129, 10.529000852323934, 10.820670737736577, 11.112340623149219,
		11.2069, 10.99067043608273, 10.861639785696486, 10.732609135310241, 10.603578484923997, 10.474547834537752, 10.345517184151507, 10.216486533765263, 10.087455883379018, 9.958425232992774,
		9.9124, 9.303781515009327, 8.93341792650408, 8.563054337998832, 8.192690749493584, 7.822327160988338, 7.45196357248309, 7.081599983977843, 6.711236395472596,
		6.6762, 6.502967220528955, 6.412050468418613, 6.321133716308271, 6.23021696419793, 6.139300212087588,
		6.0289, 6.303893786113772, 6.653892241139573, 7.003890696165374, 7.353889151191176, 7.703887606216976,
		8.2943, 8.369442297233826, 8.60943685882382, 8.849431420413813, 9.089425982003807, 9.3294205435938, 9.569415105183793, 9.809409666773787,
		10.236, 10.226326866754441, 10.182852110594622, 10.139377354434801, 10.095902598274982, 10.052427842115163, 10.008953085955342, 9.965478329795523,
		9.9124, 9.215316528159509, 8.320587389614403, 7.425858251069297, 6.531129112524191, 5.636399973979084,
		4.4108, 3.885631981048266, 3.0522965019119086, 2.2189610227755514, 1.3856255436391942, 0.5522900645028366, -0.2810454146335202, -1.1143808937698783,
		-2.0618, -2.3135291657010435, -2.6051861761297803, -2.896843186558517, -3.188500196987254, -3.4801572074159908, -3.7718142178447276,
		-4.3271, -4.301227933399094, -4.03200767740632, -3.7627874214135457, -3.4935671654207714, -3.224346909427997, -2.9551266534352227, -2.685906397442449, -2.4166861414496745,
		-2.0618, -1.8464900681422685, -1.5306937468490296, -1.2148974255557903, -0.8991011042625512, -0.5833047829693121,
		-0.12, -0.3707776101497738, -0.8413679838592405, -1.3119583575687073, -1.7825487312781738,
		-2.709, -2.7716000000000034, -4.771600000000005, -6.7716000000000065,
		-10.476, -11.116453980842534, -11.116453980842534, -13.283077732001251, -15.44970148315997,
		-18.8903, -24.452745611866604,
		-27.6282, -31.300286095170815, -34.5859751567052,
		-35.0716, -37.95285652681453, -39.50844260111241, -41.064028675410285, -42.61961474970816,
		-44.1332, -44.60001730760228, -45.054561910915105, -45.50910651422793, -45.963651117540756, -46.41819572085358, -46.87274032416641, -47.32728492747923, -47.781829530792066, -48.23637413410489, -48.69091873741772,
		-48.9876, -49.34156757806878, -49.604291231352235, -49.86701488463568,
		-50.083, -50.178507266456215, -50.25958992751082, -50.34067258856541,
		-50.3604, -50.1993010402409, -50.107595592663564, -50.015890145086225,
		-49.9584, -49.72956571448191, -49.56291049234361, -49.396255270205316, -49.22960004806702, -49.062944825928724,
		-48.9876, -47.91597409291326, -47.22366457221635, -46.53135505151944, -45.83904553082253, -45.14673601012562, -44.45442648942871, -43.7621169687318,
		-43.1623, -41.96572762776166, -40.910176840677735, -39.854626053593805, -38.79907526650988, -37.74352447942595,
		-37.0134, -35.24718994669757, -33.897186084202396, -32.547182221707224, -31.197178359212053, -29.84717449671688, -28.49717063422171,
		-28.2755, -25.9566041649726, -24.69345140594253, -23.43029864691246, -22.167145887882388, -20.903993128852317,
		-20.5085, -18.927023105446118, -17.989518277327154, -17.05201344920819, -16.114508621089225,
		-15.6541, -15.326068565862833, -15.108670836300128, -14.891273106737422, -14.673875377174715, -14.45647764761201, -14.239079918049304,
		-14.0359, -13.944577850073719, -13.858861546407224, -13.773145242740732, -13.68742893907424, -13.601712635407745, -13.515996331741253, -13.43028002807476, -13.344563724408268, -13.258847420741773, -13.17313111707528,
		-13.065, -13.088083473348783, -13.11934071552723, -13.150597957705678, -13.181855199884124, -13.213112442062572, -13.24436968424102, -13.275626926419466, -13.306884168597914, -13.33814141077636,
		-13.3887, -13.599076477404404, -14.149080339899575, -14.699084202394747, -15.249088064889918, -15.79909192738509, -16.349095789880263,
		-16.9486, -18.698563396082907, -20.621600095075113, -22.54463679406732,
		-25.0392, -26, -27, -28, -29, -30, -31, -32, -33,
		-34.4244, -39.99282843182696,
		-43.1623, -44.78233836475877, -46.30937990002013,
		-47.7138, -48.805525598279104, -49.81610191807833, -50.826678237877566, -51.83725455767679,
		-52.2238, -53.14808911434499, -53.719519787819486, -54.290950461293974, -54.86238113476847, -55.433811808242965, -56.00524248171746,
		-56.1073, -56.4715935073603, -56.671600923282284, -56.87160833920427, -57.07161575512625, -57.27162317104824, -57.47163058697022, -57.67163800289221,
		-57.7255, -57.84087049274884, -57.907531666117336, -57.97419283948582, -58.04085401285432, -58.107515186222805, -58.1741763595913, -58.24083753295979,
		-58.3727, -58.37382316145594, -58.42510907268608, -58.47639498391621, -58.52768089514634, -58.57896680637647, -58.63025271760661, -58.68153862883674, -58.73282454006687, -58.784110451297, -58.83539636252714, -58.88668227375727, -58.937968184987405,

	};

	return y[mov];
}

void crearTubo(float posx, float posy, glm::mat4 projection, GLuint uniformModel) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / escala + transx, posy / escala + transy, -10.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, escalaz));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	montRusa.UseTexture();

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cylinder.render();
}

void crearTubo2(float posx, float posy, float rot, float scale, glm::mat4 projection, GLuint uniformModel) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / escala + transx, posy / escala + transy, -8.5f));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, scale / escala, 1.0f));
	montRusa.UseTexture();
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cylinder.render();
}

void crearTubo3(float posx, float posy, float rot, float scale, glm::mat4 projection, GLuint uniformModel) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / escala + transx, posy / escala + transy, -11.5f));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, scale / escala, 1.0f));
	montRusa.UseTexture();
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cylinder.render();
}

/*void crearTubo4(float posx, float posy, float rot, float scale, glm::mat4 projection, GLuint uniformModel) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / escala + transx, posy / escala + transy, -8.0f));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, scale / escala, 1.0f));
	montRusa.UseTexture();
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

	cylinder.render();
}*/

/*Se utiliza para crear la proteccion de carrito para la montaña rusa*/
void crearTubo5(float posx, float posy, float posz, float rot, float scale, glm::mat4 projection, GLuint uniformModel, float rotx, float rotz) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / 30 + movX() / escala + transx, posy / 30 + movY() / escala + transy-0.6, posz-9));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(rotx, 0.0f, rotz));
	model = glm::scale(model, glm::vec3(0.2f, scale / 30, 0.2f));
	montRusa3.UseTexture();

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cylinder.render();
}

/*Para la base de la montaña Rusa*/
void crearTubo6(float posx, float posy, float posz, float rot, float scale, glm::mat4 projection, GLuint uniformModel) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / escala + transx, posy / escala + transy, posz));
	model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(1.0f, scale / escala + 2.0, 1.0f));
	montRusa2.UseTexture();

	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cylinder.render();
}

void crearTubo7(float posx, float posy, glm::mat4 projection, GLuint uniformModel) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(posx / escala + transx, posy / escala + transy, -10.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 8.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cylinder.render();
}

void tubosCarrito(glm::mat4 projection, GLuint uniformModel) {
	float z = 6.25, tx = 60.5;
	crearTubo5(2.03665, 4.48065 - 6, 1.7f, 90.0f + (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293, -7.33195 - 6, 1.7f, 90.0f + (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(2.03665, 4.48065 - 6, 2.0f, 90.0f + (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293, -7.33195 - 6, 2.0f, 90.0f + (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(4.03665, -16.7006 - 6, 1.755f, 90.0f + 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(4.03665, -16.7006 - 6, 1.945f, 90.0f - 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(2.03665, 4.48065 - 6, 2.3f, 90.0f + (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293, -7.33195 - 6, 2.3f, 90.0f + (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(2.03665, 4.48065 - 6, 2.6f, 90.0f + (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293, -7.33195 - 6, 2.6f, 90.0f + (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(4.03665, -16.7006 - 6, 2.355f, 90.0f + 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(4.03665, -16.7006 - 6, 2.545f, 90.0f - 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);

	crearTubo5(2.03665, 4.48065 - 6, 1.7f - z, 90.0f + (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293, -7.33195 - 6, 1.7f - z, 90.0f + (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(2.03665, 4.48065 - 6, 2.0f - z, 90.0f + (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293, -7.33195 - 6, 2.0f - z, 90.0f + (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(4.03665, -16.7006 - 6, 1.755f - z, 90.0f + 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(4.03665, -16.7006 - 6, 1.945f - z, 90.0f - 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(2.03665, 4.48065 - 6, 2.3f - z, 90.0f + (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293, -7.33195 - 6, 2.3f - z, 90.0f + (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(2.03665, 4.48065 - 6, 2.6f - z, 90.0f + (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293, -7.33195 - 6, 2.6f - z, 90.0f + (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(4.03665, -16.7006 - 6, 2.355f - z, 90.0f + 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(4.03665, -16.7006 - 6, 2.545f - z, 90.0f - 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);

	crearTubo5(2.03665 - tx, 4.48065 - 6, 1.7f, 90.0f - (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293 - tx, -7.33195 - 6, 1.7f, 90.0f - (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(2.03665 - tx, 4.48065 - 6, 2.0f, 90.0f - (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293 - tx, -7.33195 - 6, 2.0f, 90.0f - (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(-1.03665 - tx, -16.7006 - 6, 1.755f, 90.0f + 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(-1.03665 - tx, -16.7006 - 6, 1.945f, 90.0f - 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	
	crearTubo5(2.03665 - tx, 4.48065 - 6, 2.3f, 90.0f - (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293 - tx, -7.33195 - 6, 2.3f, 90.0f - (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(2.03665 - tx, 4.48065 - 6, 2.3f, 90.0f - (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293 - tx, -7.33195 - 6, 2.6f, 90.0f - (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(-1.03665 - tx, -16.7006 - 6, 2.355f, 90.0f + 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(-1.03665 - tx, -16.7006 - 6, 2.545f, 90.0f - 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);

	tx = 60.5;
	crearTubo5(2.03665 - tx, 4.48065 - 6, 1.7f - z, 90.0f - (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293 - tx, -7.33195 - 6, 1.7f - z, 90.0f - (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(2.03665 - tx, 4.48065 - 6, 2.0f - z, 90.0f - (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293 - tx, -7.33195 - 6, 2.0f - z, 90.0f - (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(-1.03665 - tx, -16.7006 - 6, 1.755f - z, 90.0f + 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(-1.03665 - tx, -16.7006 - 6, 1.845f - z, 90.0f - 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(2.03665 - tx, 4.48065 - 6, 2.3f - z, 90.0f - (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293 - tx, -7.33195 - 6, 2.3f - z, 90.0f - (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(2.03665 - tx, 4.48065 - 6, 2.6f - z, 90.0f - (42.39700276303266f), 12.686281760232191, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(1.6293 - tx, -7.33195 - 6, 2.6f - z, 90.0f - (-60.42210691949359f), 17.3295989477541, projection, uniformModel, 0.0f, 1.0f);
	crearTubo5(-1.03665 - tx, -16.7006 - 6, 2.355f - z, 90.0f + 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);
	crearTubo5(-1.03665 - tx, -16.7006 - 6, 2.545f - z, 90.0f - 45.0f, 10.008012151815949, projection, uniformModel, 1.0f, 0.0f);

}

void asientoCarrito(float tx, float ty, float tz, float ex, float ey, float ez, glm::mat4 projection, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess) {
	float x = movX() / escala + transx;
	float y = movY() / escala + transy;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x + tx, y + ty, tz));
	model = glm::scale(model, glm::vec3(ex, ey, ez));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	montRusa4.UseTexture();
	Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
	meshList[4]->RenderMesh();
}

void crearCarrito(glm::mat4 projection, GLuint uniformModel, GLuint uniformSpecularIntensity, GLuint uniformShininess) {
	float x = movX() / escala + transx;
	float y = movY() / escala + transy;
	float z = 6.25;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y - 1.18, -10.f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 5.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	montRusa5.UseTexture();
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cylinder.render();
	
	tubosCarrito(projection, uniformModel);
	asientoCarrito(0.4, -0.975, -6.85f, 0.35, 1.1, 1.3, projection, uniformModel, uniformSpecularIntensity, uniformShininess);
	asientoCarrito(0.2, -1.65, -6.85f, 0.75, 0.25, 1.3, projection, uniformModel, uniformSpecularIntensity, uniformShininess);
	asientoCarrito(0.4, -0.975, -13.15f, 0.35, 1.1, 1.3, projection, uniformModel, uniformSpecularIntensity, uniformShininess);
	asientoCarrito(0.2, -1.65, -13.15f, 0.75, 0.25, 1.3, projection, uniformModel, uniformSpecularIntensity, uniformShininess);

	//Carrito 2
	float tx = 2.2;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x - 2, y - 1.1, -10.f));
	model = glm::scale(model, glm::vec3(1.5f, 1.5f, 5.0f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	montRusa5.UseTexture();
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	cylinder.render();
	
	asientoCarrito(-tx - 0.1, -0.975, -6.85f, 0.35, 1.1, 1.3, projection, uniformModel, uniformSpecularIntensity, uniformShininess);
	asientoCarrito(-tx + 0.1, -1.65, -6.85f, 0.75, 0.25, 1.3, projection, uniformModel, uniformSpecularIntensity, uniformShininess);
	asientoCarrito(-tx - 0.1, -0.975, -13.15f, 0.35, 1.1, 1.3, projection, uniformModel, uniformSpecularIntensity, uniformShininess);
	asientoCarrito(-tx + 0.1, -1.65, -13.15f, 0.75, 0.25, 1.3, projection, uniformModel, uniformSpecularIntensity, uniformShininess);
}

void tubosCentrales(glm::mat4 projection, GLuint uniformModel) {
	crearTubo(67.7033f, 31.5954, projection, uniformModel);
	crearTubo(59.2891, 20.5921, projection, uniformModel);
	crearTubo(54.7583, 16.7086, projection, uniformModel);
	crearTubo(47.3149, 16.3849, projection, uniformModel);
	crearTubo(38.2533, 21.8866, projection, uniformModel);
	crearTubo(31.4572, 29.0064, projection, uniformModel);
	crearTubo(25.9555, 32.8899, projection, uniformModel);
	crearTubo(21.1012, 32.8899, projection, uniformModel);
	crearTubo(12.6869, 28.0355, projection, uniformModel);
	crearTubo(7.8325, 25.7701, projection, uniformModel);
	crearTubo(2.3308, 26.0937, projection, uniformModel);
	crearTubo(-3.1708, 28.0355, projection, uniformModel);
	crearTubo(-10.6142, 32.5663, projection, uniformModel);
	crearTubo(-16.1159, 39.0388, projection, uniformModel);
	crearTubo(-22.9120, 44.8641, projection, uniformModel);
	crearTubo(-29.0609, 47.1294, projection, uniformModel);
	crearTubo(-34.8862, 45.8349, projection, uniformModel);
	crearTubo(-42.3296, 41.9514, projection, uniformModel);
	crearTubo(-50.4202, 38.0679, projection, uniformModel);
	crearTubo(-59.8054, 36.7734, projection, uniformModel);
	crearTubo(-70.1615, 39.0388, projection, uniformModel);
	crearTubo(-80.5175, 45.5113, projection, uniformModel);
	crearTubo(-88.6081, 53.2783, projection, uniformModel);
	crearTubo(-96.6988, 58.7800, projection, uniformModel);
	crearTubo(-106.7312, 63.3108, projection, uniformModel);
	crearTubo(-118.0581, 63.3108, projection, uniformModel);
	crearTubo(-124.8543, 59.7509, projection, uniformModel);
	crearTubo(-126.7961, 51.6602, projection, uniformModel);
	crearTubo(-126.7961, 42.5987, projection, uniformModel);
	crearTubo(-123.5598, 30.9481, projection, uniformModel);
	crearTubo(-118.3818, 23.1811, projection, uniformModel);
	crearTubo(-109.3202, 12.8251, projection, uniformModel);
	crearTubo(-97.9933, 4.4108, projection, uniformModel);
	crearTubo(-84.7246, -2.0618, projection, uniformModel);
	crearTubo(-67.2488, -5.2980, projection, uniformModel);
	crearTubo(-51.7148, -3.0326, projection, uniformModel);
	crearTubo(-38.7697, 3.4399, projection, uniformModel);
	crearTubo(-27.4428, 8.9415, projection, uniformModel);
	crearTubo(-19.6758, 11.2069, projection, uniformModel);
	crearTubo(-9.6433, 9.9124, projection, uniformModel);
	crearTubo(-0.9054, 6.6762, projection, uniformModel);
	crearTubo(6.2143, 6.0289, projection, uniformModel);
	crearTubo(12.6869, 8.2943, projection, uniformModel);
	crearTubo(20.7775, 10.2360, projection, uniformModel);
	crearTubo(28.2209, 9.9124, projection, uniformModel);
	crearTubo(34.3698, 4.4108, projection, uniformModel);
	crearTubo(42.1369, -2.0618, projection, uniformModel);
	crearTubo(49.9039, -4.3271, projection, uniformModel);
	crearTubo(58.3182, -2.0618, projection, uniformModel);
	crearTubo(64.4671, -0.1200, projection, uniformModel);
	crearTubo(69.9687, -2.7090, projection, uniformModel);
	crearTubo(73.8522, -10.4760, projection, uniformModel);
	crearTubo(75.7940, -18.8903, projection, uniformModel);
	crearTubo(76.1176, -27.6282, projection, uniformModel);
	crearTubo(73.8522, -35.0716, projection, uniformModel);
	crearTubo(68.0270, -44.1332, projection, uniformModel);
	crearTubo(57.3473, -48.9876, projection, uniformModel);
	crearTubo(53.1779, -50.0830, projection, uniformModel);
	crearTubo(49.7567, -50.3604, projection, uniformModel);
	crearTubo(45.3731, -49.9584, projection, uniformModel);
	crearTubo(39.5479, -48.9876, projection, uniformModel);
	crearTubo(31.1336, -43.1623, projection, uniformModel);
	crearTubo(25.3083, -37.0134, projection, uniformModel);
	crearTubo(18.8358, -28.2755, projection, uniformModel);
	crearTubo(12.6869, -20.5085, projection, uniformModel);
	crearTubo(7.5089, -15.6541, projection, uniformModel);
	crearTubo(0.0654, -14.0359, projection, uniformModel);
	crearTubo(-11.2615, -13.0650, projection, uniformModel);
	crearTubo(-21.6175, -13.3887, projection, uniformModel);
	crearTubo(-28.0900, -16.9486, projection, uniformModel);
	crearTubo(-32.2972, -25.0392, projection, uniformModel);
	crearTubo(-31.6499, -34.4244, projection, uniformModel);
	crearTubo(-29.0609, -43.1623, projection, uniformModel);
	crearTubo(-26.0803, -47.7138, projection, uniformModel);
	crearTubo(-21.6175, -52.2238, projection, uniformModel);
	crearTubo(-14.8214, -56.1073, projection, uniformModel);
	crearTubo(-6.7307, -57.7255, projection, uniformModel);
	crearTubo(2.9781, -58.3727, projection, uniformModel);
	crearTubo(15.5995, -59.0200, projection, uniformModel);
	crearTubo(26.2792, -59.3436, projection, uniformModel);
	crearTubo(39.8715, -59.6672, projection, uniformModel);
	crearTubo(54.1110, -59.6672, projection, uniformModel);
	crearTubo(69.3215, -59.6672, projection, uniformModel);
	crearTubo(77.4121, -57.0782, projection, uniformModel);
	crearTubo(80.0011, -50.6057, projection, uniformModel);
	crearTubo(80.3248, -39.9260, projection, uniformModel);
	crearTubo(80.3248, -28.2755, projection, uniformModel);
	crearTubo(78.0594, 12.8251, projection, uniformModel);
	crearTubo(76.7649, 21.8866, projection, uniformModel);
	crearTubo(73.8522, 29.3300, projection, uniformModel);
}

void tubosFrontales(glm::mat4 projection, GLuint uniformModel) {
	crearTubo2(63.4962, 26.09375, 90.0f + (52.59493328717978f), 13.851764238897516, projection, uniformModel);
	crearTubo2(57.0237, 18.65035, 90.0f + (40.60098221686725f), 5.967388112901655, projection, uniformModel);
	crearTubo2(51.0366, 16.54675, 90.0f + 2.49f, 7.4504352389642285, projection, uniformModel);
	crearTubo2(42.7841, 19.13575, 90.0f - 31.26379769313227f, 10.601004549098166, projection, uniformModel);
	crearTubo2(34.85525, 25.4465, 90.0f - 46.33252609109743f, 9.842689025362937, projection, uniformModel);
	crearTubo2(28.70635, 30.94815, 90.0f - 35.21722498224841f, 6.734261291337008, projection, uniformModel);
	crearTubo2(23.52835, 32.8899, 90.0f - 0.0f, 4.854300000000002, projection, uniformModel);
	crearTubo2(16.89405, 30.4627, 90.0f + 29.981619719268817f, 9.714198054909112, projection, uniformModel);
	crearTubo2(10.2597, 26.9028, 90.0f + 25.01702270630387f, 5.35698016796777, projection, uniformModel);
	crearTubo2(5.08165, 25.9319, 90.0f - 3.3661554196722325f, 5.511208565278581, projection, uniformModel);
	crearTubo2(-0.42, 27.0646, 90.0f - 19.4405795787848f, 5.834225724121411, projection, uniformModel);
	crearTubo2(-6.8925, 30.3009, 90.0f - 31.32888822534934f, 8.713917155906406, projection, uniformModel);
	crearTubo2(-13.36505, 35.80255, 90.0f - 49.6350779928133f, 8.49481954722995, projection, uniformModel);
	crearTubo2(-19.51395, 41.95145, 90.0f - 40.601641789061766f, 8.951038783292137, projection, uniformModel);
	crearTubo2(-25.98645, 45.99675, 90.0f + (-20.224168528517907f), 6.552904340824761, projection, uniformModel);
	crearTubo2(-31.97355, 46.48215, 90.0f + (12.528703566628387f), 5.96739895934569, projection, uniformModel);
	crearTubo2(-38.6079, 43.89315, 90.0f + (27.55273265698377f), 8.395580730956015, projection, uniformModel);
	crearTubo2(-46.3749, 40.00965, 90.0f + (25.64107489225664f), 8.974373549724795, projection, uniformModel);
	crearTubo2(-55.1128, 37.42065, 90.0f + (7.853251326826877f), 9.474054532775286, projection, uniformModel);
	crearTubo2(-64.98345, 37.9061, 90.0f + (-12.33910377804815f), 10.600983179403697, projection, uniformModel);
	crearTubo2(-75.3395, 42.27505, 90.0f + (-32.005383208083494f), 12.212288575447268, projection, uniformModel);
	crearTubo2(-84.5628, 49.3948, 90.0f + (-43.83094912039673f), 11.215350969095883, projection, uniformModel);
	crearTubo2(-92.65345, 56.02915, 90.0f + (-34.215818352471494f), 9.784075295090497, projection, uniformModel);
	crearTubo2(-101.715, 61.0454, 90.0f + (-24.304732888795808f), 11.008051526042195, projection, uniformModel);
	crearTubo2(-112.39465, 63.3108, 90.0f + (-0.0f), 11.326899999999995, projection, uniformModel);
	crearTubo2(-121.4562, 61.53085, 90.0f + (27.645880857366844f), 7.672106780409145, projection, uniformModel);
	crearTubo2(-125.8252, 55.70555, 90.0f + (76.50405244757053f), 8.320457543308539, projection, uniformModel);
	crearTubo2(-126.7961, 47.129456, 90.0f + (-90.0f), 9.061500000000002, projection, uniformModel);
	crearTubo2(-125.17795, 36.7734, 90.0f + (-74.47578754643733f), 12.091737594324483, projection, uniformModel);
	crearTubo2(-120.9708, 27.0646, 90.0f + (-56.30993247402022f), 9.334772252176267, projection, uniformModel);
	crearTubo2(-113.851, 18.0031, 90.0f + (-48.813761484042274f), 13.760789605251581, projection, uniformModel);
	crearTubo2(-103.65675, 8.61795, 90.0f + (-36.60717725733272f), 14.110248335872758, projection, uniformModel);
	crearTubo2(-91.35895, 1.1745, 90.0f + (-26.00356704117095f), 14.763229675447047, projection, uniformModel);
	crearTubo2(-75.9867, -3.6799, 90.0f + (-10.491289167940792f), 17.77291692660492, projection, uniformModel);
	crearTubo2(-59.4818, -4.1653, 90.0f + (8.297235259864937f), 15.698318163421208, projection, uniformModel);
	crearTubo2(-45.24225, 0.203651, 90.0f + (26.564874134434728f), 14.47303942715558, projection, uniformModel);
	crearTubo2(-33.10625, 6.1907, 90.0f + (25.906355980592835f), 12.592309802812192, projection, uniformModel);
	crearTubo2(-23.5593, 10.0742, 90.0f + (16.2603746701528f), 8.090632000035596, projection, uniformModel);
	crearTubo2(-14.65955, 10.55965, 90.0f + (-7.352288755200629f), 10.115670343580794, projection, uniformModel);
	crearTubo2(-5.27435, 8.2943, 90.0f + (-20.32279512906578f), 9.31793361480967, projection, uniformModel);
	crearTubo2(2.65445, 6.35255, 90.0f + (-5.194864263944052f), 7.149064650707811, projection, uniformModel);
	crearTubo2(9.4506, 7.1616, 90.0f + (19.28996735901617f), 6.857593449600231, projection, uniformModel);
	crearTubo2(16.7322, 9.26515, 90.0f + (13.495438653029938f), 8.320336967334917, projection, uniformModel);
	crearTubo2(24.4992, 10.0742, 90.0f + (-2.4893524957693316f), 7.450430894921448, projection, uniformModel);
	crearTubo2(31.29535, 7.1616, 90.0f + (-41.819924742711514f), 8.250852911669192, projection, uniformModel);
	crearTubo2(38.25335, 1.1745, 90.0f + (-39.805643650163205f), 10.110509046037198, projection, uniformModel);
	crearTubo2(46.0204, -3.19445, 90.0f + (-16.259694821024816f), 8.090604000320374, projection, uniformModel);
	crearTubo2(54.11105, -3.19445, 90.0f + (15.067926505352949f), 8.713898586740607, projection, uniformModel);
	crearTubo2(61.39265, -1.0909, 90.0f + (17.5259251347696f), 6.448221340028586, projection, uniformModel);
	crearTubo2(67.2179, -1.4145, 90.0f + (-25.20122395391477f), 6.080339099096363, projection, uniformModel);
	crearTubo2(71.91045, -6.5925, 90.0f + (-63.43494882292203f), 8.683769990620434, projection, uniformModel);
	crearTubo2(74.8231, -14.68315, 90.0f + (-77.00513455287557f), 8.63545202812221, projection, uniformModel);
	crearTubo2(75.9558, -23.25925, 90.0f + (-87.87907337083882f), 8.743890059350015, projection, uniformModel);
	crearTubo2(74.9849, -31.3499, 90.0f + (73.07236441472665f), 7.780503885996071, projection, uniformModel);
	crearTubo2(70.9396, -39.6024, 90.0f + (57.265285042836226f), 10.77244399382053, projection, uniformModel);
	crearTubo2(62.68715, -46.5604, 90.0f + (24.443914359742816f), 11.73120588217597, projection, uniformModel);
	crearTubo2(55.2626, -49.5353, 90.0f + (14.720291560195822f), 4.310892891269738, projection, uniformModel);
	crearTubo2(51.4673, -50.2217, 90.0f + (4.635553361462997f), 3.4324277414098603, projection, uniformModel);
	crearTubo2(47.5649, -50.1594, 90.0f + (-5.239679526304752f), 4.40199420263135, projection, uniformModel);
	crearTubo2(42.4605, -49.473, 90.0f + (-9.461684205915338f), 5.90554042234917, projection, uniformModel);
	crearTubo2(35.34075, -46.07495, 90.0f + (-34.695224348247514f), 10.233990647836254, projection, uniformModel);
	crearTubo2(28.22095, -40.08785, 90.0f + (-46.54802847077413f), 8.47012935556477, projection, uniformModel);
	crearTubo2(22.07205, -32.64445, 90.0f + (-53.471223040193884f), 10.87401272116232, projection, uniformModel);
	crearTubo2(15.76135, -24.392, 90.0f + (-51.632401246860944f), 9.906324253223291, projection, uniformModel);
	crearTubo2(10.0979, -18.0813, 90.0f + (-43.15253696336998f), 7.097667459102323, projection, uniformModel);
	crearTubo2(3.78715, -14.845, 90.0f + (-12.26512525240675f), 7.617365915459227, projection, uniformModel);
	crearTubo2(-5.59805, -13.55045, 90.0f + (-4.899207230654043f), 11.368434827187075, projection, uniformModel);
	crearTubo2(-16.4395, -13.22685, 90.0f + (1.7903251495862291f), 10.361057749573641, projection, uniformModel);
	crearTubo2(-24.85375, -15.16865, 90.0f + (28.810963650313397f), 7.3868900262559745, projection, uniformModel);
	crearTubo2(-30.1936, -20.9939, 90.0f + (62.52507782499277f), 9.11911948600302, projection, uniformModel);
	crearTubo2(-31.97355, -29.7318, 90.0f + (-86.05454139824778f), 9.40749575232431, projection, uniformModel);
	crearTubo2(-30.3554, -38.79335, 90.0f + (-73.49568326976372f), 9.113386714608355, projection, uniformModel);
	crearTubo2(-27.5706, -45.438, 90.0f + (-56.78077326733756f), 5.440600022975405, projection, uniformModel);
	crearTubo2(-23.8489, -49.9688, 90.0f + (-45.30139263653787f), 6.3448155087441265, projection, uniformModel);
	crearTubo2(-18.21945, -54.16555, 90.0f + (-29.744972088854233f), 7.82742278531063, projection, uniformModel);
	crearTubo2(-10.77605, -56.9164, 90.0f + (-11.310341032121075f), 8.250939202902904, projection, uniformModel);
	crearTubo2(-1.8763, -58.0491, 90.0f + (-3.813761484042297f), 9.73034764435475, projection, uniformModel);
	crearTubo2(9.2888, -58.69635, 90.0f + (-2.9358940250233654f), 12.637987784849297, projection, uniformModel);
	crearTubo2(20.93935, -59.1818, 90.0f + (-1.7355584070412438f), 10.684601492334657, projection, uniformModel);
	crearTubo2(33.07535, -59.5054, 90.0f + (-1.3638171931305818f), 13.596151523501051, projection, uniformModel);
	crearTubo2(46.99125, -59.6672, 90.0f + (0.0f), 14.2395, projection, uniformModel);
	crearTubo2(61.71625, -59.6672, 90.0f + (0.0f), 15.210500000000003, projection, uniformModel);
	crearTubo2(73.3668, -58.3727, 90.0f + (17.74472301670676f), 8.49474716280596, projection, uniformModel);
	crearTubo2(78.7066, -53.84195, 90.0f + (68.19859051364821f), 6.971095842835618, projection, uniformModel);
	crearTubo2(80.16295, -45.26585, 90.0f + (88.26390559281641f), 10.684604521459834, projection, uniformModel);
	crearTubo2(80.3248, -34.10075, 90.0f + (90.0f), 11.650500000000001, projection, uniformModel);
	crearTubo2(79.1921, -7.72521, 90.0f + (-86.84513962572451f), 41.16298528435468, projection, uniformModel);
	crearTubo2(77.41215, 17.35585, 90.0f + (-81.86989764584403f), 9.153497282459858, projection, uniformModel);
	crearTubo2(75.30855, 25.6083, 90.0f + (-68.62894237996083f), 7.9929984893029955, projection, uniformModel);
	crearTubo2(70.77775, 30.4627, 90.0f + (-20.2249889747316f), 6.55293891090097, projection, uniformModel);
}

void tubosTraseros(glm::mat4 projection, GLuint uniformModel) {
	crearTubo3(63.4962, 26.09375, 90.0f + (52.59493328717978f), 13.851764238897516, projection, uniformModel);
	crearTubo3(57.0237, 18.65035, 90.0f + (40.60098221686725f), 5.967388112901655, projection, uniformModel);
	crearTubo3(51.0366, 16.54675, 90.0f + (2.490120795985931f), 7.4504352389642285, projection, uniformModel);
	crearTubo3(42.7841, 19.13575, 90.0f + (-31.26379769313227f), 10.601004549098166, projection, uniformModel);
	crearTubo3(34.85525, 25.4465, 90.0f + (-46.33252609109743f), 9.842689025362937, projection, uniformModel);
	crearTubo3(28.70635, 30.94815, 90.0f + (-35.21722498224841f), 6.734261291337008, projection, uniformModel);
	crearTubo3(23.52835, 32.8899, 90.0f + (-0.0f), 4.854300000000002, projection, uniformModel);
	crearTubo3(16.89405, 30.4627, 90.0f + (29.981619719268817f), 9.714198054909112, projection, uniformModel);
	crearTubo3(10.2597, 26.9028, 90.0f + (25.01702270630387f), 5.35698016796777, projection, uniformModel);
	crearTubo3(5.08165, 25.9319, 90.0f + (-3.3661554196722325f), 5.511208565278581, projection, uniformModel);
	crearTubo3(-0.42, 27.0646, 90.0f + (-19.4405795787848f), 5.834225724121411, projection, uniformModel);
	crearTubo3(-6.8925, 30.3009, 90.0f + (-31.32888822534934f), 8.713917155906406, projection, uniformModel);
	crearTubo3(-13.36505, 35.80255, 90.0f + (-49.6350779928133f), 8.49481954722995, projection, uniformModel);
	crearTubo3(-19.51395, 41.95145, 90.0f + (-40.601641789061766f), 8.951038783292137, projection, uniformModel);
	crearTubo3(-25.98645, 45.99675, 90.0f + (-20.224168528517907f), 6.552904340824761, projection, uniformModel);
	crearTubo3(-31.97355, 46.48215, 90.0f + (12.528703566628387f), 5.96739895934569, projection, uniformModel);
	crearTubo3(-38.6079, 43.89315, 90.0f + (27.55273265698377f), 8.395580730956015, projection, uniformModel);
	crearTubo3(-46.3749, 40.00965, 90.0f + (25.64107489225664f), 8.974373549724795, projection, uniformModel);
	crearTubo3(-55.1128, 37.42065, 90.0f + (7.853251326826877f), 9.474054532775286, projection, uniformModel);
	crearTubo3(-64.98345, 37.9061, 90.0f + (-12.33910377804815f), 10.600983179403697, projection, uniformModel);
	crearTubo3(-75.3395, 42.27505, 90.0f + (-32.005383208083494f), 12.212288575447268, projection, uniformModel);
	crearTubo3(-84.5628, 49.3948, 90.0f + (-43.83094912039673f), 11.215350969095883, projection, uniformModel);
	crearTubo3(-92.65345, 56.02915, 90.0f + (-34.215818352471494f), 9.784075295090497, projection, uniformModel);
	crearTubo3(-101.715, 61.0454, 90.0f + (-24.304732888795808f), 11.008051526042195, projection, uniformModel);
	crearTubo3(-112.39465, 63.3108, 90.0f + (-0.0f), 11.326899999999995, projection, uniformModel);
	crearTubo3(-121.4562, 61.53085, 90.0f + (27.645880857366844f), 7.672106780409145, projection, uniformModel);
	crearTubo3(-125.8252, 55.70555, 90.0f + (76.50405244757053f), 8.320457543308539, projection, uniformModel);
	crearTubo3(-126.7961, 47.12945, 90.0f + (90.0f), 9.061500000000002, projection, uniformModel);
	crearTubo3(-125.17795, 36.7734, 90.0f + (-74.47578754643733f), 12.091737594324483, projection, uniformModel);
	crearTubo3(-120.9708, 27.0646, 90.0f + (-56.30993247402022f), 9.334772252176267, projection, uniformModel);
	crearTubo3(-113.851, 18.0031, 90.0f + (-48.813761484042274f), 13.760789605251581, projection, uniformModel);
	crearTubo3(-103.65675, 8.61795, 90.0f + (-36.60717725733272f), 14.110248335872758, projection, uniformModel);
	crearTubo3(-91.35895, 1.1745, 90.0f + (-26.00356704117095f), 14.763229675447047, projection, uniformModel);
	crearTubo3(-75.9867, -3.6799, 90.0f + (-10.491289167940792f), 17.77291692660492, projection, uniformModel);
	crearTubo3(-59.4818, -4.1653, 90.0f + (8.297235259864937f), 15.698318163421208, projection, uniformModel);
	crearTubo3(-45.24225, 0.20365, 90.0f + (26.564874134434728f), 14.47303942715558, projection, uniformModel);
	crearTubo3(-33.10625, 6.1907, 90.0f + (25.906355980592835f), 12.592309802812192, projection, uniformModel);
	crearTubo3(-23.5593, 10.0742, 90.0f + (16.2603746701528f), 8.090632000035596, projection, uniformModel);
	crearTubo3(-14.65955, 10.55965, 90.0f + (-7.352288755200629f), 10.115670343580794, projection, uniformModel);
	crearTubo3(-5.27435, 8.2943, 90.0f + (-20.32279512906578f), 9.31793361480967, projection, uniformModel);
	crearTubo3(2.65445, 6.35255, 90.0f + (-5.194864263944052f), 7.149064650707811, projection, uniformModel);
	crearTubo3(9.4506, 7.1616, 90.0f + (19.28996735901617f), 6.857593449600231, projection, uniformModel);
	crearTubo3(16.7322, 9.26515, 90.0f + (13.495438653029938f), 8.320336967334917, projection, uniformModel);
	crearTubo3(24.4992, 10.0742, 90.0f + (-2.4893524957693316f), 7.450430894921448, projection, uniformModel);
	crearTubo3(31.29535, 7.1616, 90.0f + (-41.819924742711514f), 8.250852911669192, projection, uniformModel);
	crearTubo3(38.25335, 1.1745, 90.0f + (-39.805643650163205f), 10.110509046037198, projection, uniformModel);
	crearTubo3(46.0204, -3.19445, 90.0f + (-16.259694821024816f), 8.090604000320374, projection, uniformModel);
	crearTubo3(54.11105, -3.19445, 90.0f + (15.067926505352949f), 8.713898586740607, projection, uniformModel);
	crearTubo3(61.39265, -1.0909, 90.0f + (17.5259251347696f), 6.448221340028586, projection, uniformModel);
	crearTubo3(67.2179, -1.4145, 90.0f + (-25.20122395391477f), 6.080339099096363, projection, uniformModel);
	crearTubo3(71.91045, -6.5925, 90.0f + (-63.43494882292203f), 8.683769990620434, projection, uniformModel);
	crearTubo3(74.8231, -14.68315, 90.0f + (-77.00513455287557f), 8.63545202812221, projection, uniformModel);
	crearTubo3(75.9558, -23.25925, 90.0f + (-87.87907337083882f), 8.743890059350015, projection, uniformModel);
	crearTubo3(74.9849, -31.3499, 90.0f + (73.07236441472665f), 7.780503885996071, projection, uniformModel);
	crearTubo3(70.9396, -39.6024, 90.0f + (57.265285042836226f), 10.77244399382053, projection, uniformModel);
	crearTubo3(62.68715, -46.5604, 90.0f + (24.443914359742816f), 11.73120588217597, projection, uniformModel);
	crearTubo3(55.2626, -49.5353, 90.0f + (14.720291560195822f), 4.310892891269738, projection, uniformModel);
	crearTubo3(51.4673, -50.2217, 90.0f + (4.635553361462997f), 3.4324277414098603, projection, uniformModel);
	crearTubo3(47.5649, -50.1594, 90.0f + (-5.239679526304752f), 4.40199420263135, projection, uniformModel);
	crearTubo3(42.4605, -49.473, 90.0f + (-9.461684205915338f), 5.90554042234917, projection, uniformModel);
	crearTubo3(35.34075, -46.07495, 90.0f + (-34.695224348247514f), 10.233990647836254, projection, uniformModel);
	crearTubo3(28.22095, -40.08785, 90.0f + (-46.54802847077413f), 8.47012935556477, projection, uniformModel);
	crearTubo3(22.07205, -32.64445, 90.0f + (-53.471223040193884f), 10.87401272116232, projection, uniformModel);
	crearTubo3(15.76135, -24.392, 90.0f + (-51.632401246860944f), 9.906324253223291, projection, uniformModel);
	crearTubo3(10.0979, -18.0813, 90.0f + (-43.15253696336998f), 7.097667459102323, projection, uniformModel);
	crearTubo3(3.78715, -14.845, 90.0f + (-12.26512525240675f), 7.617365915459227, projection, uniformModel);
	crearTubo3(-5.59805, -13.55045, 90.0f + (-4.899207230654043f), 11.368434827187075, projection, uniformModel);
	crearTubo3(-16.4395, -13.22685, 90.0f + (1.7903251495862291f), 10.361057749573641, projection, uniformModel);
	crearTubo3(-24.85375, -15.16865, 90.0f + (28.810963650313397f), 7.3868900262559745, projection, uniformModel);
	crearTubo3(-30.1936, -20.9939, 90.0f + (62.52507782499277f), 9.11911948600302, projection, uniformModel);
	crearTubo3(-31.97355, -29.7318, 90.0f + (-86.05454139824778f), 9.40749575232431, projection, uniformModel);
	crearTubo3(-30.3554, -38.79335, 90.0f + (-73.49568326976372f), 9.113386714608355, projection, uniformModel);
	crearTubo3(-27.5706, -45.43805, 90.0f + (-56.78077326733756f), 5.440600022975405, projection, uniformModel);
	crearTubo3(-23.8489, -49.9688, 90.0f + (-45.30139263653787f), 6.3448155087441265, projection, uniformModel);
	crearTubo3(-18.21945, -54.16555, 90.0f + (-29.744972088854233f), 7.82742278531063, projection, uniformModel);
	crearTubo3(-10.77605, -56.9164, 90.0f + (-11.310341032121075f), 8.250939202902904, projection, uniformModel);
	crearTubo3(-1.8763, -58.0491, 90.0f + (-3.813761484042297f), 9.73034764435475, projection, uniformModel);
	crearTubo3(9.2888, -58.69635, 90.0f + (-2.9358940250233654f), 12.637987784849297, projection, uniformModel);
	crearTubo3(20.93935, -59.1818, 90.0f + (-1.7355584070412438f), 10.684601492334657, projection, uniformModel);
	crearTubo3(33.07535, -59.5054, 90.0f + (-1.3638171931305818f), 13.596151523501051, projection, uniformModel);
	crearTubo3(46.99124, -59.6672, 90.0f + (0.0f), 14.2395, projection, uniformModel);
	crearTubo3(61.71625, -59.6672, 90.0f + (0.0f), 15.210500000000003, projection, uniformModel);
	crearTubo3(73.3668, -58.3727, 90.0f + (17.74472301670676f), 8.49474716280596, projection, uniformModel);
	crearTubo3(78.7066, -53.84195, 90.0f + (68.19859051364821f), 6.971095842835618, projection, uniformModel);
	crearTubo3(80.16295, -45.26585, 90.0f + (88.26390559281641f), 10.684604521459834, projection, uniformModel);
	crearTubo3(80.3248, -34.10075, 90.0f + (90.0f), 11.650500000000001, projection, uniformModel);
	crearTubo3(79.1921, -7.7252, 90.0f + (-86.84513962572451f), 41.16298528435468, projection, uniformModel);
	crearTubo3(77.41215, 17.35585, 90.0f + (-81.86989764584403f), 9.153497282459858, projection, uniformModel);
	crearTubo3(75.30855, 25.6083, 90.0f + (-68.62894237996083f), 7.9929984893029955, projection, uniformModel);
	crearTubo3(70.77775, 30.4627, 90.0f + (-20.2249889747316f), 6.55293891090097, projection, uniformModel);
}

void tubosBase(glm::mat4 projection, GLuint uniformModel){
	float z = -6.0f;
	crearTubo6(-84.8778 - 10, -9.2566, z, 90.0f + (-61.81543289654117f), 122.46339901460354, projection, uniformModel);
	crearTubo6(-46.79225 - 10, -12.51525-5, z, 90.0f + (-85.86537263575138f), 101.69036030076794-5, projection, uniformModel);
	crearTubo6(-14.4094-10, -18.01425-5, z, 90.0f + (62.61501839254192f), 101.84033735966314-5, projection, uniformModel);
	crearTubo6(10.8615-10, -41.2322, z, 90.0f + (-86.82015545333502f), 44.059636894100706, projection, uniformModel);
	crearTubo6(32.63745 - 10, -18.4216 - 5, z, 90.0f + (-69.10221928248195f), 95.92297734020771-5, projection, uniformModel);
	crearTubo6(53.4114 - 12, -20.45825 - 5, z, 90.0f + (-88.36341722179426f), 85.57460728563117 - 5, projection, uniformModel);
	crearTubo6(68.07535 - 10, -16.79225 - 5, z, 90.0f + (78.11136372491555f), 94.90747839079911 - 5, projection, uniformModel);
	crearTubo6(89.6639 - 10, -42.04685 - 5, z, 90.0f + (90.0f), 42.3625 - 5, projection, uniformModel);

	z = -14.0f;
	crearTubo6(-84.8778 -10, -9.2566, z, 90.0f + (-61.81543289654117f), 122.46339901460354, projection, uniformModel);
	crearTubo6(-46.79225 - 10, -12.51525 - 5, z, 90.0f + (-85.86537263575138f), 101.69036030076794 - 5, projection, uniformModel);
	crearTubo6(-14.4094 - 10, -18.01425 - 5, z, 90.0f + (62.61501839254192f), 101.84033735966314 - 5, projection, uniformModel);
	crearTubo6(10.8615 - 10, -41.2322, z, 90.0f + (-86.82015545333502f), 44.059636894100706, projection, uniformModel);
	crearTubo6(32.63745 - 10, -18.4216 - 5, z, 90.0f + (-69.10221928248195f), 95.92297734020771 - 5, projection, uniformModel);
	crearTubo6(53.4114 - 10, -20.45825 - 5, z, 90.0f + (-88.36341722179426f), 85.57460728563117 - 5, projection, uniformModel);
	crearTubo6(68.07535 - 10, -16.79225 - 5, z, 90.0f + (78.11136372491555f), 94.90747839079911 - 5, projection, uniformModel);
	crearTubo6(89.6639 - 10, -42.04685 - 5, z, 90.0f + (90.0f), 42.3625 - 5, projection, uniformModel);

	crearTubo7(-95.1120 - 30, 57.2301 - 10, projection, uniformModel);
	crearTubo7(-95.1120 - 13.5, 57.2301 - 40, projection, uniformModel);
	crearTubo7(-25.4582 - 35, 33.1976, projection, uniformModel);
	crearTubo7(-25.4582 - 32.5, 33.1976 - 34, projection, uniformModel);
	crearTubo7(34.0122 - 35, 22.1996, projection, uniformModel);
	crearTubo7(34.0122 - 40, 22.1996 - 10, projection, uniformModel);
	crearTubo7(34.0122 - 56, 22.1996 - 40, projection, uniformModel);
	crearTubo7(46.6395 - 47, -24.2363 + 7.5, projection, uniformModel);
	crearTubo7(46.6395 - 45, -24.2363 - 30, projection, uniformModel);
	crearTubo7(34.0122 - 28.5, 22.1996, projection, uniformModel);
	crearTubo7(34.0122 - 25, 22.1996 - 10, projection, uniformModel);
	crearTubo7(34.0122 -1.5, 22.1996 - 71, projection, uniformModel);
	crearTubo7(79.6334 - 39.25, 16.2281 + 1, projection, uniformModel);
	crearTubo7(79.6334 - 39, 16.2281 - 20.5, projection, uniformModel);
	crearTubo7(79.6334 - 37.75, 16.2281 - 68.5, projection, uniformModel);
	crearTubo7(79.6334 - 37.75, 16.2281 - 68.5, projection, uniformModel);
	crearTubo7(83.2994 - 16, 24.6436, projection, uniformModel);
	crearTubo7(83.2994 - 21.5, -3.5, projection, uniformModel);
	crearTubo7(79.6334 - 27.9, 16.2281 - 69, projection, uniformModel);
	crearTubo7(104.6639 - 25, -25.8656 -0.5, projection, uniformModel);
	crearTubo7(104.6639 - 25, -25.8656 - 25, projection, uniformModel);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();
	//
	
	//
	CreateObjects();
	CreateShaders();
	CrearCubo();

	creaCarro();
	creaAsientos();
	creaHexagono();
	creaBaseTriangulo();
	creaBrazos();

	camera = Camera(glm::vec3(0.0f, 0.0f, 130.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	plainTexture = Texture("Textures/piso.jpg");
	plainTexture.LoadTexture();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	rojo = Texture("Textures/rojo.png");
	rojo.LoadTexture();

	//Texturas Carlos
	pisoTexture = Texture("Textures/PisoFeria.jpg");
	pisoTexture.LoadTexture();
	/*laminaTexture = Texture("Textures/Lamina.png");
	laminaTexture.LoadTexture();
	CirculoTexture = Texture("Textures/AluminioCirculo.jpg");
	CirculoTexture.LoadTexture();
	CircuitoCafeTexture = Texture("Textures/CircuitoCafe.jpg");
	CircuitoCafeTexture.LoadTexture();
	*/
	//Texturas Juego Giratorio
	amarilloTexture = Texture("Textures/Amarillo.png");
	amarilloTexture.LoadTexture();
	laminaCirculoTexture = Texture("Textures/Circulo.png");
	laminaCirculoTexture.LoadTextureA();
	azulTexture = Texture("Textures/PlasticoAzul.png");
	azulTexture.LoadTextureA();
	amarillo2Texture = Texture("Textures/AmarilloPuntos.png");
	amarillo2Texture.LoadTextureA();
	rojoMetalTexture = Texture("Textures/Rojo.png");
	rojoMetalTexture.LoadTextureA();
	circuitoAzulTexture = Texture("Textures/Circuitoazul.jpg");
	circuitoAzulTexture.LoadTexture();
	MetalPisoTexture = Texture("Textures/PisoCirculo.jpg");
	MetalPisoTexture.LoadTexture();
	FloresTexture = Texture("Textures/AmarilloFlores.png");
	FloresTexture.LoadTextureA();
	CajaMetalTexture = Texture("Textures/Caja.jpg");
	CajaMetalTexture.LoadTexture();
	coloresTexture = Texture("Textures/Colores.jpg");
	coloresTexture.LoadTexture();
	RayasTexture = Texture("Textures/PisoGira.jpg");
	RayasTexture.LoadTexture();
	
	/*Textura para la montaña Rusa*/
	montRusa = Texture("Textures/p_03_amarillo.jpg");
	montRusa.LoadTexture();
	montRusa2 = Texture("Textures/gris.jpg");
	montRusa2.LoadTexture();
	montRusa3 = Texture("Textures/p_14_rojo.jpg");
	montRusa3.LoadTexture();
	montRusa4 = Texture("Textures/p_04_azul.jpg");
	montRusa4.LoadTexture();
	montRusa5 = Texture("Textures/Magentas.jpg");
	montRusa5.LoadTexture();
	/*********************************/

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Tienda = Model();
	Tienda.LoadModel("Models/3.gol gapppe.obj");

	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");

	//********carrusel***********************
	GLfloat gira1 = 0;
	T1_a = Model();
	T1_a.LoadModel("Models/p_001_piso1.obj");

	T2_a = Model();
	T2_a.LoadModel("Models/p_001_piso2.obj");

	techo = Model();
	techo.LoadModel("Models/p_011_techo_SB.obj");

	columna = Model();
	columna.LoadModel("Models/p_14_Columna.obj");

	caballo = Model();
	caballo.LoadModel("Models/p_01_caballo.obj");
	//****************************************
	Helados = Model();
	Helados.LoadModel("Models/13924_Ice_Cream_vending_Cart_v2_l2.obj");
	Cafe = Model();
	Cafe.LoadModel("Models/Coffee Shop.obj");
	
	// -- Rueda

	Rueda = Model();
	Rueda.LoadModel("Models/rueda.obj");

	BaseRueda = Model();
	BaseRueda.LoadModel("Models/baserueda.obj");

	CabinaRueda = Model();
	CabinaRueda.LoadModel("Models/cabinarueda.obj");

	// --------

	// -- Carritos
	// -----------

	/*Casa de terror*/
	casaTerror1 = Model();
	casaTerror1.LoadModel("Models/Castle/CastleOB_1.obj");
	dragon = Model();
	dragon.LoadModel("Models/Dragon/dragonCuerpo.obj");
	dragonAlas = Model();
	dragonAlas.LoadModel("Models/Dragon/dragonAlas.obj");
	zombie = Model();
	zombie.LoadModel("Models/CasaDeTerror/Zumbi_Female.obj");

	sq1 = Model();
	sq1.LoadModel("Models/Charizard/sqCuerpo2.obj");
	sq2 = Model();
	sq2.LoadModel("Models/Charizard/sqPder.obj");
	sq3 = Model();
	sq3.LoadModel("Models/Charizard/sqPizq.obj");
	sq4 = Model();
	sq4.LoadModel("Models/Charizard/sqCola.obj");
	

	//Reja
	reja = Model();
	reja.LoadModel("Models/reja/rejas.obj");
	//Faro
	faro = Model();
	faro.LoadModel("Models/StreetLamp/faro.obj");

	/**************/
	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		2.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		3.0f, 3.0f,
		0.0, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		4.0f);
	spotLightCount++;
	//luz de faro
	spotLights[2] = SpotLight(1.0f, 0.0f, 0.0f, //Color de la luz
		1.0f, 1.0f, //Intensidad de izquierda y derecha
		0.0f, 0.0f, 0.0f, //posicion
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	//luz de helicoptero
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 1.0f,
		0.0, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	
	spotLights[4] = SpotLight(1.0f, 1.0f, 0.0f,
		3.0f, 3.0f,
		0.0, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		4.0f);
	spotLightCount++;

	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);
	glm::vec3 posSquartle = glm::vec3(2.0f, 0.0f, 0.0f);

//KEYFRAMES DECLARADOS INICIALES
	KeyFrame[0].movAvion_x = 0.0f;
	KeyFrame[0].movAvion_y = 0.0f;
	KeyFrame[0].movAvion_z = 0.0f;
	KeyFrame[0].giroDragon = 0.0f;
	KeyFrame[0].giroDragon2 = 0.0f;
	KeyFrame[0].rotAlas = 7.0f;
	KeyFrame[0].sqX = 0.0f;
	KeyFrame[0].sqY = 0.0f;
	KeyFrame[0].sqZ = 0.0f;

	KeyFrame[1].movAvion_x = 5.0f;
	KeyFrame[1].movAvion_y = 1.0f;
	KeyFrame[1].movAvion_z = 0.0f;
	KeyFrame[1].giroDragon = 10.0;
	KeyFrame[1].giroDragon2 = 0.0;
	KeyFrame[1].rotAlas = -7.0f;
	KeyFrame[1].sqX = 0.0f;
	KeyFrame[1].sqY = 1.0f;
	KeyFrame[1].sqZ = 3.0f;

	KeyFrame[2].movAvion_x = 12.0f;
	KeyFrame[2].movAvion_y = 0.0f;
	KeyFrame[2].movAvion_z = 0.0f;
	KeyFrame[2].giroDragon = 0.0f;
	KeyFrame[2].giroDragon2 = 0.0;
	KeyFrame[2].rotAlas = 7.0f;
	KeyFrame[2].sqX = 0.0f;
	KeyFrame[2].sqY = 0.0f;
	KeyFrame[2].sqZ = 5.0f;

	KeyFrame[3].movAvion_x = 18.0f;
	KeyFrame[3].movAvion_y = -1.0f;
	KeyFrame[3].movAvion_z = 0.0f;
	KeyFrame[3].giroDragon = -10.0f;
	KeyFrame[3].rotAlas = -7.0f;
	KeyFrame[3].giroDragon2 = 0.0f;
	KeyFrame[3].sqX = 0.0f;
	KeyFrame[3].sqY = 1.0f;
	KeyFrame[3].sqZ = 8.0f;

	KeyFrame[4].movAvion_x = 24.0f;
	KeyFrame[4].movAvion_y = 0.0f;
	KeyFrame[4].movAvion_z = 0.0f;
	KeyFrame[4].giroDragon = 0.0f;
	KeyFrame[4].rotAlas = 7.0;
	KeyFrame[4].giroDragon2 = 90.0f;
	KeyFrame[4].sqX = 0.0f;
	KeyFrame[4].sqY = 0.0f;
	KeyFrame[4].sqZ = 10.0f;

	KeyFrame[5].movAvion_x = 28.0f;
	KeyFrame[5].movAvion_y = 0.0f;
	KeyFrame[5].movAvion_z = -5.0f;
	KeyFrame[5].giroDragon = 0.0f;
	KeyFrame[5].rotAlas = -7.0f;
	KeyFrame[5].giroDragon2 = 90.0f;
	KeyFrame[5].sqX = 0.0f;
	KeyFrame[5].sqY = 1.0f;
	KeyFrame[5].sqZ = 12.0f;

	KeyFrame[6].movAvion_x = 28.0f;
	KeyFrame[6].movAvion_y = 3.0f;
	KeyFrame[6].movAvion_z = -8.0f;
	KeyFrame[6].giroDragon = 10.0;
	KeyFrame[6].rotAlas = 7.0f;
	KeyFrame[6].giroDragon2 = 90.0;
	KeyFrame[6].sqX = 0.0f;
	KeyFrame[6].sqY = 0.0f;
	KeyFrame[6].sqZ = 13.0f;

	KeyFrame[7].movAvion_x = 28.0f;
	KeyFrame[7].movAvion_y = 0.0f;
	KeyFrame[7].movAvion_z = -13.0f;
	KeyFrame[7].giroDragon = 0.0f;
	KeyFrame[7].rotAlas = -7.0f;
	KeyFrame[7].giroDragon2 = 90.0f;
	KeyFrame[7].sqX = 0.0f;
	KeyFrame[7].sqY = 1.0f;
	KeyFrame[7].sqZ = 14.0f;

	KeyFrame[8].movAvion_x = 28.0f;
	KeyFrame[8].movAvion_y = -1.0f;
	KeyFrame[8].movAvion_z = -15.0f;
	KeyFrame[8].giroDragon = -10.0f;
	KeyFrame[8].rotAlas = 7.0f;
	KeyFrame[8].giroDragon2 = 90.0f;
	KeyFrame[8].sqX = 0.0f;
	KeyFrame[8].sqY = 0.0f;
	KeyFrame[8].sqZ = 15.0f;

	KeyFrame[9].movAvion_x = 28.0f;
	KeyFrame[9].movAvion_y = 0.0f;
	KeyFrame[9].movAvion_z = -20.0f;
	KeyFrame[9].giroDragon = 0.0f;
	KeyFrame[9].rotAlas = -7.0f;
	KeyFrame[9].giroDragon2 = 90.0f;
	KeyFrame[9].sqX = 0.0f;
	KeyFrame[9].sqY = 1.0f;
	KeyFrame[9].sqZ = 16.0f;

	KeyFrame[10].movAvion_x = 24.0f;
	KeyFrame[10].movAvion_y = 0.0f;
	KeyFrame[10].movAvion_z = -20.0f;
	KeyFrame[10].giroDragon = 0.0f;
	KeyFrame[10].rotAlas = 7.0f;
	KeyFrame[10].giroDragon2 = 180.0f;
	KeyFrame[10].sqX = 0.0f;
	KeyFrame[10].sqY = 0.0f;
	KeyFrame[10].sqZ = 17.0f;

	KeyFrame[11].movAvion_x = 18.0f;
	KeyFrame[11].movAvion_y = 3.0f;
	KeyFrame[11].movAvion_z = -20.0f;
	KeyFrame[11].giroDragon = 10.0;
	KeyFrame[11].rotAlas = -7.0f;
	KeyFrame[11].giroDragon2 = 180.0;

	KeyFrame[12].movAvion_x = 12.0f;
	KeyFrame[12].movAvion_y = 0.0f;
	KeyFrame[12].movAvion_z = -20.0f;
	KeyFrame[12].giroDragon = 0.0f;
	KeyFrame[12].rotAlas = 7.0f;
	KeyFrame[12].giroDragon2 = 180.0f;

	KeyFrame[13].movAvion_x = 5.0f;
	KeyFrame[13].movAvion_y = -1.0f;
	KeyFrame[13].movAvion_z = -20.0f;
	KeyFrame[13].giroDragon = -10.0f;
	KeyFrame[13].rotAlas = -7.0f;
	KeyFrame[13].giroDragon2 = 180.0f;

	KeyFrame[14].movAvion_x = 0.0f;
	KeyFrame[14].movAvion_y = 0.0f;
	KeyFrame[14].movAvion_z = -20.0f;
	KeyFrame[14].giroDragon = 0.0f;
	KeyFrame[14].rotAlas = 7.0f;
	KeyFrame[14].giroDragon2 = 180.0f;


	KeyFrame[15].movAvion_x = 0.0f;
	KeyFrame[15].movAvion_y = 0.0f;
	KeyFrame[15].movAvion_z = -15.0f;
	KeyFrame[15].giroDragon = 0.0f;
	KeyFrame[15].rotAlas = -7.0f;
	KeyFrame[15].giroDragon2 = 270.0f;

	KeyFrame[16].movAvion_x = 0.0f;
	KeyFrame[16].movAvion_y = 3.0f;
	KeyFrame[16].movAvion_z = -10.0f;
	KeyFrame[16].giroDragon = 10.0;
	KeyFrame[16].rotAlas = 7.0f;
	KeyFrame[16].giroDragon2 = 270.0;

	KeyFrame[17].movAvion_x = 0.0f;
	KeyFrame[17].movAvion_y = 0.0f;
	KeyFrame[17].movAvion_z = -5.0f;
	KeyFrame[17].giroDragon = 0.0f;
	KeyFrame[17].rotAlas = -7.0f;
	KeyFrame[17].giroDragon2 = 270.0f;

	KeyFrame[18].movAvion_x = 0.0f;
	KeyFrame[18].movAvion_y = -1.0f;
	KeyFrame[18].movAvion_z = 0.0f;
	KeyFrame[18].giroDragon = -10.0f;
	KeyFrame[18].rotAlas = 7.0f;
	KeyFrame[18].giroDragon2 = 270.0f;

	KeyFrame[19].movAvion_x = 0.0f;
	KeyFrame[19].movAvion_y = 0.0f;
	KeyFrame[19].movAvion_z = 5.0f;
	KeyFrame[19].giroDragon = 0.0f;
	KeyFrame[19].rotAlas = -7.0f;
	KeyFrame[19].giroDragon2 = 270.0f;

	//DATOS PARA EL RECORRIDO Squartle

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);


	//Variables para el juego giratorio
	GiratorioMov = 0.0f;
	MovimientoGira = 0.0f;
	MovY = 0.0;
	estado = 0;

	movCoche = 2.0f;
	movOffset = 10.0f;
	avanza = 1;
	
	float giraRueda = 0.0;
	float GiratorioMov = 0.0f;
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		bool playMont = mainWindow.montRusa;
		gira1 += 17.0f;
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;


		//giraRueda += movOffset * deltaTime;
		if (avanza)
		{
			if (movCoche < 1.0f)
			{
				movCoche += movOffset * deltaTime*.15f;

				GiratorioMov += movOffset * deltaTime * 20.0f;
				avanza = 1;
			}
			else
			{
				avanza = 0;
			}
		}
		else
		{
			if (movCoche > -1.0f)
			{
				movCoche -= movOffset * deltaTime*.15f;
			}
			else
			{
				avanza = 1;
			}
		}

		if (playMont)
		{
			if (mov < 694)
			{
				mov += 1;
			}
			else
			{
				mov = 0;
			}
			//printf("%f, %f\n", movX(), movY());
		}

		if (horario) {
			if (dia)
			{
				nocheCont++;
				//printf("cont=%d\n", nocheCont);
				if (nocheCont > 200)
				{
					spotLights[2].SetPos(glm::vec3(11, 40.0f, 100.0f - 0.25f)); //Permite colocar una luz en cualquier lugar, es verde
					spotLights[3].SetPos(glm::vec3(-15, 40.0f, 100.0f - 0.25f));
					spotLights[4].SetPos(glm::vec3(10.5, 20.0f, 50.0f - 0.25f));
					spotLights[1].SetPos(glm::vec3(-10.5, 20.0f, 50.0f - 0.25f));
					rtSkybox = "Textures/Skybox/sgod_noche3_ft.jpg";
					lfSkybox = "Textures/Skybox/sgod_noche3_bk.jpg";
					dnSkybox = "Textures/Skybox/sgod_noche3_dn.jpg";
					upSkybox = "Textures/Skybox/sgod_noche3_up.jpg";
					bkSkybox = "Textures/Skybox/sgod_noche3_rt.jpg";
					ftSkybox = "Textures/Skybox/sgod_noche3_lf.jpg";

					noche = true;
					nocheCont = 0;
					dia = false;

				}

			}
			if (noche)
			{
				diaCont++;
			//	printf("cont=%f\n", diaCont);
				if (diaCont > 200)
				{
					spotLights[2].SetPos(glm::vec3(0, -50.0f, 0.0f)); //Permite colocar una luz en cualquier lugar, es verde
					spotLights[3].SetPos(glm::vec3(0, -50.0f, 0.0f));
					spotLights[4].SetPos(glm::vec3(0, -50.0f, 0.0f));
					spotLights[1].SetPos(glm::vec3(0, -50.0f, 0.0f));

					rtSkybox = "Textures/Skybox/sgod_ft.tga";
					lfSkybox = "Textures/Skybox/sgod_bk.tga";
					dnSkybox = "Textures/Skybox/sgod_dn.tga";
					upSkybox = "Textures/Skybox/sgod_up.tga";
					bkSkybox = "Textures/Skybox/sgod_rt.tga";
					ftSkybox = "Textures/Skybox/sgod_lf.tga";

					dia = true;
					diaCont = 0;
					noche = false;
				}
			}

		}
		if (recorrido)
		{
			if (cont < 1227)
			{
				rX = movRecorrido();
				cont++;
				rY = movRecorrido();
				cont++;
				rZ = movRecorrido();
				cont++;
				camera.recorrido(glm::vec3(rX, rY, rZ), deltaTime);
				printf("%f, %f, %f, %d\n", rX, rY, rZ, cont);
			}
			else
			{
				cont = 0;
				printf("cont = %d\n", cont);

				recorrido = false;
			}
		}

		if (recorridoMont)
		{
			if (mov < 1227)
			{
				camera.recorrido(glm::vec3(movX() / escala + transx, movY() / escala + transy, -13.0f), deltaTime);
			}
			else
			{
				mov = 0;
				printf("cont = %d\n", mov);

				recorrido = false;
			}
		}

		if (recorrido)
		{
			if (cont < 1227)
			{
				rX = movRecorrido();
				cont++;
				rY = movRecorrido();
				cont++;
				rZ = movRecorrido();
				cont++;
				camera.recorrido(glm::vec3(rX, rY, rZ), deltaTime);
				printf("%f, %f, %f, %d\n", rX, rY, rZ, cont);
			}
			else
			{
				cont = 0;
				printf("cont = %d\n", cont);

				recorrido = false;
			}
		}


		std::vector<std::string> skyboxFaces;
		skyboxFaces.push_back(rtSkybox);
		skyboxFaces.push_back(lfSkybox);
		skyboxFaces.push_back(dnSkybox);
		skyboxFaces.push_back(upSkybox);
		skyboxFaces.push_back(bkSkybox);
		skyboxFaces.push_back(ftSkybox);

		skybox = Skybox(skyboxFaces);


		//Recibir eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		//para keyframes
		inputKeyframes(mainWindow.getsKeys());
		animate();
		
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();
		
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);

		cylinder.init();
		cylinder.load();

		/*//Recorrido
		rX += KeyFrameR[playRIndex].rX_inc;
		rY += KeyFrameR[playRIndex].rX_inc;
		rZ += KeyFrameR[playRIndex].rX_inc;
		*/
;
		

		// ----------------------------------------------------------------------
		// PISO
		// ----------------------------------------------------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ----------------------------------------------------------------------
		// TIENDA
		// ----------------------------------------------------------------------
		/*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tienda.RenderModel();
		*/
		//#################################################################################
			//CARRUSEL
		//#################################################################################

		//model			//para guerdar la escala de los objetos
		glm::mat4 modelgira(1.0);		//para objetos en movimiento
		glm::mat4 modeltemp(1.0);

		model = glm::mat4(1.0);

		//*********objetos fijos *********************
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		modeltemp = model = glm::translate(model, glm::vec3(20.0f, -0.75f, 20.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.25f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA			
		T2_a.RenderModel();

		model = modeltemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.36f, 0.0f));
		modelgira = model = glm::rotate(model, glm::radians(gira1* toRadians), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA			
		T1_a.RenderModel();

		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		modeltemp = model = glm::translate(model, glm::vec3(-1.8f * 3, 1.5f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -movCoche, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA			
		caballo.RenderModel();

		model = modeltemp;
		model = glm::translate(model, glm::vec3(1.8f * 3, 0.0f, 1.8f * 3));
		modeltemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, movCoche, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA			
		caballo.RenderModel();

		model = modeltemp;
		model = glm::translate(model, glm::vec3(1.8f * 3, 0.0f, 1.8f * 3));
		modeltemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, -movCoche, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA			
		caballo.RenderModel();

		model = modeltemp;
		model = glm::translate(model, glm::vec3(1.8f * 3, 0.0f, 1.8f * 3));
		modeltemp = model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, movCoche, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA			
		caballo.RenderModel();

		//*******columna***********
		model = modelgira;
		modeltemp = model = glm::translate(model, glm::vec3(1.65f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA			
		columna.RenderModel();

		//*******techo***********
		model = modeltemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA			
		techo.RenderModel();

		// ----------------------------------------------------------------------
		// HELADOS
		// ----------------------------------------------------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, -2.0f, 50.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helados.RenderModel();

		// ----------------------------------------------------------------------
		// CAFE
		// ----------------------------------------------------------------------

		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(26.0f, -2.0f, 70.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Cafe.RenderModel();
		
		// ----------------------------------------------------------------------
		// RUEDA : Modelo jerarquico
		// ----------------------------------------------------------------------

		glm::mat4 modelRueda(1.0);
		glm::mat4 modelCabina(1.0);

		// Primero la base
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(46.0f, -2.0f, 40.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BaseRueda.RenderModel();

		// Despues la rueda girando
		modelRueda = model = glm::translate(model, glm::vec3(0.0f, 12.5f, 0.0f));
		modelRueda = glm::rotate(modelRueda, giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelRueda));
		Rueda.RenderModel();

		// Primera cabina
		modelCabina = modelRueda;
		modelCabina = glm::translate(modelRueda, glm::vec3(0.0f, 10.0f, 0.0f));
		modelCabina = glm::scale(modelCabina, glm::vec3(5.5f, 5.5f, 5.5f));
		modelCabina = glm::rotate(modelCabina, -giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabina));
		CabinaRueda.RenderModel();

		// Segunda cabina
		modelCabina = modelRueda;
		modelCabina = glm::translate(modelRueda, glm::vec3(0.0f, -10.0f, 0.0f));
		modelCabina = glm::scale(modelCabina, glm::vec3(5.5f, 5.5f, 5.5f));
		modelCabina = glm::rotate(modelCabina, -giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabina));
		CabinaRueda.RenderModel();

		// Tercera cabina
		modelCabina = modelRueda;
		modelCabina = glm::translate(modelRueda, glm::vec3(10.0f, -0.0f, 0.0f));
		modelCabina = glm::scale(modelCabina, glm::vec3(5.5f, 5.5f, 5.5f));
		modelCabina = glm::rotate(modelCabina, -giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabina));
		CabinaRueda.RenderModel();

		// Cuarta cabina
		modelCabina = modelRueda;
		modelCabina = glm::translate(modelRueda, glm::vec3(-10.0f, -0.0f, 0.0f));
		modelCabina = glm::scale(modelCabina, glm::vec3(5.5f, 5.5f, 5.5f));
		modelCabina = glm::rotate(modelCabina, -giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabina));
		CabinaRueda.RenderModel();

		// Quinta cabina
		modelCabina = modelRueda;
		modelCabina = glm::translate(modelRueda, glm::vec3(-7.0f, -7.0f, 0.0f));
		modelCabina = glm::scale(modelCabina, glm::vec3(5.5f, 5.5f, 5.5f));
		modelCabina = glm::rotate(modelCabina, -giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabina));
		CabinaRueda.RenderModel();

		// Sexta cabina
		modelCabina = modelRueda;
		modelCabina = glm::translate(modelRueda, glm::vec3(-7.0f, 7.0f, 0.0f));
		modelCabina = glm::scale(modelCabina, glm::vec3(5.5f, 5.5f, 5.5f));
		modelCabina = glm::rotate(modelCabina, -giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabina));
		CabinaRueda.RenderModel();

		// Septima cabina
		modelCabina = modelRueda;
		modelCabina = glm::translate(modelRueda, glm::vec3(7.0f, -7.0f, 0.0f));
		modelCabina = glm::scale(modelCabina, glm::vec3(5.5f, 5.5f, 5.5f));
		modelCabina = glm::rotate(modelCabina, -giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabina));
		CabinaRueda.RenderModel();

		// Octava cabina
		modelCabina = modelRueda;
		modelCabina = glm::translate(modelRueda, glm::vec3(7.0f, 7.0f, 0.0f));
		modelCabina = glm::scale(modelCabina, glm::vec3(5.5f, 5.5f, 5.5f));
		modelCabina = glm::rotate(modelCabina, -giraRueda * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelCabina));
		CabinaRueda.RenderModel();

		// -- FIN RUEDA ---------------------------------------------------------



		// ----------------------------------------------------------------------
		// MONTAÑA RUSA CON PRIMITIVAS
		// ----------------------------------------------------------------------
		tubosCentrales(projection, uniformModel);
		tubosFrontales(projection, uniformModel);
		tubosTraseros(projection, uniformModel);
		tubosBase(projection, uniformModel);
		crearCarrito(projection, uniformModel, uniformSpecularIntensity, uniformShininess);
		
		/*--------------------------------------*/
		/*-------- Casa de terror --------------*/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, -2.0f, 0.0f));
	//	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));			
		casaTerror1.RenderModel();
		
		/*inicia squartle*/
		model = glm::mat4(1.0);
		posSquartle = glm::vec3(posXsq + sqX, posYsq + sqY, posZsq + sqZ);
		model = glm::translate(model, posSquartle);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 50.0f));
		//	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sq1.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, posSquartle);
		//	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sq2.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, posSquartle);
		//	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sq3.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, posSquartle);
		//	model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		sq4.RenderModel();
		//Fin squartle
		
		//Inicia Dragon
		modeltemp = model = glm::mat4(1.0);
		posblackhawk = glm::vec3(posXdragon + movAvion_x, posYdragon + movAvion_y, posZDragon + movAvion_z);
		model = glm::translate(model, posblackhawk);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroDragon), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroDragon2), glm::vec3(0.0f, 1.0f, 0.0));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		dragon.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, posblackhawk);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroDragon), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroDragon2), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotAlas), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		dragonAlas.RenderModel();
		//spotLights[3].SetPos(posblackhawk);
		//Fin dragon

/*		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		zombie.RenderModel();
		*/
		/*--------------------------------------*/

		/*************Reja********************/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 100.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		reja.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, -1.7f, 50.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		faro.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -1.7f, 50.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		faro.RenderModel();

		
		//Modelos Carlos
/*___________________________________________________*/
		//Juego Giratorio

		//****************************************************************************************
		//Juego Giratorio con Primitivas//

		//Base del juego
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(12.0f, 0.4f, 12.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		MetalPisoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//Torre
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 5.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rojoMetalTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//Base1
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.3f, 0.4f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		rojoMetalTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 5.4f, 0.0f));
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		laminaCirculoTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//TorreTriangulos
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f, 0.0f));
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarillo2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[8]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 7.5f, 0.0f));
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarillo2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[8]->RenderMesh();

		//Brazos de juego
		//Brazo1
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f + MovY, 0.0f));
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarillo2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		//Carrito 1
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(0.0f * toRadians) * 4, 4.0f + MovY, sin(0.0f*toRadians) * 4));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		circuitoAzulTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Asiento 1
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(0.0f* toRadians) * 4, 4.0f + MovY, sin(0.0f* toRadians) * 4));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarilloTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//Brazo 2
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f + MovY, 0.0f));
		model = glm::rotate(model, (60 + GiratorioMov)* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarillo2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		//Carrito 2
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(60.0f* toRadians) * 4, 4.0f + MovY, sin(60.0f* toRadians) * 4));
		model = glm::rotate(model, (60)* toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FloresTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Asiento 2
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(60.0f* toRadians) * 4, 4.0f + MovY, sin(60.0f* toRadians) * 4));
		model = glm::rotate(model, (60)* toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarilloTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//Brazo 3
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f + MovY, 0.0f));
		model = glm::rotate(model, (120 + GiratorioMov) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarillo2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		//Carrito 3
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(120.0f* toRadians) * 4, 4.0f + MovY, sin(120.0f* toRadians) * 4));
		model = glm::rotate(model, (120) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		azulTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Asiento 3
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(120.0f* toRadians) * 4, 4.0f + MovY, sin(120.0f* toRadians) * 4));
		model = glm::rotate(model, (120) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarilloTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//Brazo 4
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f + MovY, 0.0f));
		model = glm::rotate(model, (180 + GiratorioMov) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarillo2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		//Carrito 4
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(180.0f* toRadians) * 4, 4.0f + MovY, sin(180.0f* toRadians) * 4));
		model = glm::rotate(model, (180) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CajaMetalTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Asiento 4
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(180.0f* toRadians) * 4, 4.0f + MovY, sin(180.0f* toRadians) * 4));
		model = glm::rotate(model, (180) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarilloTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//Brazo 5 
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f + MovY, 0.0f));
		model = glm::rotate(model, (240 + GiratorioMov) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarillo2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		//Carrito 5
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(240.0f * toRadians) * 4, 4.0f + MovY, sin(240.0f*toRadians) * 4));
		model = glm::rotate(model, (240) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		coloresTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Asiento 5
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(240.0f*toRadians) * 4, 4.0f + MovY, sin(240.0f*toRadians) * 4));
		model = glm::rotate(model, (240) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarilloTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		//Brazo 6
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::translate(model, glm::vec3(0.0f, 6.5f + MovY, 0.0f));
		model = glm::rotate(model, (300 + GiratorioMov) * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.3f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarillo2Texture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		//Carrito 6
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(300.0f*toRadians) * 4, 4.0f + MovY, sin(300.0f*toRadians) * 4));
		model = glm::rotate(model, (300) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		RayasTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[5]->RenderMesh();

		//Asiento 6
		model = glm::mat4(1.0);
		model = glm::translate(model, PosJuegoGira);
		model = glm::rotate(model, GiratorioMov * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::translate(model, glm::vec3(cos(300.0f*toRadians) * 4, 4.0f + MovY, sin(300.0f*toRadians) * 4));
		model = glm::rotate(model, (300) * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		amarilloTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}

void inputKeyframes(bool* keys)
{
	if (keys[GLFW_KEY_SPACE])
	{
		if(reproduciranimacion<1)
		{
			if (play == false && (FrameIndex > 1))
			{
				resetElements();
				//First Interpolation				
				interpolation();
				play = true;
				playIndex = 0;
				i_curr_steps = 0;
				reproduciranimacion++;
			//	printf("presiona 0 para habilitar reproducir de nuevo la animación'\n");
				habilitaranimacion = 0;

			}
			else
			{
				play = false;
			}
		}
	}
	if (keys[GLFW_KEY_0])
	{
		if (habilitaranimacion < 1)
		{
			reproduciranimacion = 0;
		}
	}	
		
	if (keys[GLFW_KEY_Q])
	{
		recorrido = true;
	}

	if (keys[GLFW_KEY_M])
	{
		recorridoMont = true;
	}
}
