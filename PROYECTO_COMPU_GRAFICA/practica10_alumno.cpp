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

//******************
Model caballo;
Model T1_a;
Model T2_a;
Model techo;
Model columna;
//*******************
Model reja; 

Model Helados;
Model Cafe;
Model Circo;

// -- RUEDA 

Model Rueda;
Model BaseRueda;
Model CabinaRueda;

// --------

// -- CARRITOS

Model Tarima;
Model Carro;

// -----------

Skybox skybox;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

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


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	Tagave = Texture("Textures/Agave.tga");
	Tagave.LoadTextureA();
	rojo = Texture("Textures/rojo.png");
	rojo.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	Tienda = Model();
	Tienda.LoadModel("Models/3.gol gapppe.obj");


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
	reja = Model();
	reja.LoadModel("Models/p_01_caballo.obj");
	//llllllllllllllllllll

	Helados = Model();
	Helados.LoadModel("Models/13924_Ice_Cream_vending_Cart_v2_l2.obj");
	Cafe = Model();
	Cafe.LoadModel("Models/Coffee Shop.obj");
	Circo = Model();
	Circo.LoadModel("Models/17475_Circus_Tent_NEW.obj");
	//Circo.LoadModel("Models/Circus tent N240211.3ds");

	// -- Rueda

	Rueda = Model();
	Rueda.LoadModel("Models/rueda.obj");

	BaseRueda = Model();
	BaseRueda.LoadModel("Models/baserueda.obj");

	CabinaRueda = Model();
	CabinaRueda.LoadModel("Models/cabinarueda.obj");

	// --------

	// -- Carritos

	Tarima = Model();
	Tarima.LoadModel("Models/Tarima2.obj");

	Carro = Model();
	Carro.LoadModel("Models/carrito.obj");
	// -----------

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
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		10.0f, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de faro
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, -1.5f, 0.0f,
		-4.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	//luz de helicoptero
	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 1.0f,
		2.0 - movCoche, 2.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;
	glm::vec3 posblackhawk = glm::vec3(2.0f, 0.0f, 0.0f);

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	/*skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake-night_ft.tga");*/


	skybox = Skybox(skyboxFaces);


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movCoche = 0.0f;
	movOffset = 10.0f;
	avanza = 1;
	float giraRueda = 0.0;
	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		gira1 += 17.0f;
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		giraRueda += movOffset * deltaTime;
		if (avanza)
		{
			if (movCoche < 1.0f)
			{
				movCoche += movOffset * deltaTime*.15f;
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

		//Recibir eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
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

		// ----------------------------------------------------------------------
		// PISO
		// ----------------------------------------------------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		plainTexture.UseTexture();
		//rojo.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		// ----------------------------------------------------------------------
		// TIENDA
		// ----------------------------------------------------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.4f, 1.4f, 1.4f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tienda.RenderModel();

		//#################################################################################
			//CARRUSEL
		//#################################################################################
		
		//model			//para guerdar la escala de los objetos
		glm::mat4 modelgira(1.0);		//para objetos en movimiento
		glm::mat4 modeltemp(1.0);

		model = glm::mat4(1.0);

		//*********objetos fijos *********************
		dadoTexture.UseTexture();
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		modeltemp = model = glm::translate(model, glm::vec3(4.15f, -1.0f, -0.5f));
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
		model = glm::translate(model, glm::vec3(16.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Helados.RenderModel();

		// ----------------------------------------------------------------------
		// CAFE
		// ----------------------------------------------------------------------


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(26.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.06f, 0.06f, 0.06f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
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
		model = glm::translate(model, glm::vec3(46.0f, -2.0f, 0.0f));
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
		// CARRITOS CHOCONES
		// ----------------------------------------------------------------------

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Tarima.RenderModel();

		// Carro

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Carro.RenderModel();

		// -- FIN CARRITOS ------------------------------------------------------


		// ----------------------------------------------------------------------
		// CIRCO
		// ----------------------------------------------------------------------

		/*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(86.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Circo.RenderModel(); */

		// Ejemplo para cargar modelos

		/*
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movCoche, -1.5f, 0.0f));
		//model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kitt_M.RenderModel();
		spotLights[2].SetPos(glm::vec3(movCoche, -1.5f, 0.0f));
		*/


		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}