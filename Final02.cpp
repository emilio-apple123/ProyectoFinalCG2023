/*---------------------------------------------------------*/
/* ----------------  Proyecto Final   -----------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Garcia Velazquez Christopher, Vazquez Ramirez Jhoana Polette, Gonzalez Rosas Berenise ------*/

#include <Windows.h>
#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#pragma comment (lib, "Winmm.lib")

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(90.0f, 150.0f, 90.0f));
float MovementSpeed = 0.8f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, -1.0f, 0.0f);




// posiciones
//float x = 0.0f;
//float y = 0.0f;

float
movAuto_x = -600.0f,// Ubicacion inicial del auto
movAuto_z = -470.0f,
movAuto_y = 0.0f,
movPez_x = 0.0f,
movPez_y = 0,
movPez_z = 0,
movCan_x = 0.0f,
movCan_y = 0,
movCan_z = 0,
movT_x = 0.0f,
movT_y = 0,
movT_z = 0,
movGav_x = 0.0f,
movGav_y = 0.0f,
movGav_z = 0.0f,
movB_x = 0.0f,
movB_y = 0,
movB_z = 0,
orienta = 0.0f,
orientaPez = 0.0f,
orientaT = 0.0f,
orientaB = 0.0f,
orientaGav = 0.0f,
giroLlanta = 0.0f;

bool
animacion = false,
animacion2 = false,
animacion3 = true,
animacion4 = false,
animacion5 = false,
animacion6 = true,
animacion7 = false,
animacion8 = true,
animacion9 = true,
bandera = false,
recorrido1 = true,//Lambo
recorrido2 = false,
recorrido3 = false,
recorrido4 = false,
recorrido5 = true,//Cola peces
recorrido6 = false,
recorrido7 = true,//Peces nadando
recorrido8 = false,
recorrido9 = true,//Cangrejos
recorrido10 = false,
recorrido11 = true,//Tiburones
recorrido12 = false,
recorrido13 = false,
recorrido14 = false,
recorrido15 = true,//Ballenas
recorrido16 = false,
recorrido17 = false,
recorrido18 = false,
recorrido19 = true,
recorrido20 = false,
recorrido21 = true,
recorrido22 = false,
recorrido23 = false,
recorrido24 = false,
recorrido31 = true,
recorrido32 = false,
recorrido33 = false,
recorrido34 = false,
recorrido35 = false;



//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
giroMonito = 0.0f;

float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
giroMonitoInc = 0.0f;


float
variableX = 0,
variableXB = 0,
variableXT = 0,
variableXG = 0,
variableY = 0,
variableZ = 0,
MyVariable = 0.0f,
myVariable = 0.0f,
jet = 0.0f;

float		
LX = 0.0,
LY = 0.0f,
LZ = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}

void animate(void)
{
	lightPosition.x = 500.0f * cos(MyVariable);
	lightPosition.y = 50.0f * sin(MyVariable);

	MyVariable += 0.007f;




	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	///Vehículo
	/*if (animacion)
	{
		if (jet == 0.0f) {
			movAuto_z -= 3.0f;
			giroLlanta -= 3.0f;
			if (movAuto_z <= -170.0f)
				jet = 1.0f;
		}

		if (jet == 1.0f) {
			movAuto_y += 3.0f;
			if (movAuto_y >= 200.5f)
				jet = 2.0f;
		}
		if (jet == 2.0f) {
			movAuto_z += 3.0f;
			if (movAuto_z >= 240.5f)
				jet = 3.0f;
		}

		if (jet == 3.0f) {
			movAuto_y -= 3.0f;
			if (movAuto_y <= 0.0f)
				jet = 4.0f;
		}
		if (jet == 4.0f) {
			movAuto_z += 3.0f;
			if (movAuto_z >= 540.5f)
				jet = 5.0f;
		}

	}
	*/
	//Cola tiburon
	
	if (animacion6)
	{

		//Codigo recorrido en el escenario 
		if (recorrido19)
		{
			variableXT += 0.07f;

			if (variableXT >= 2.8f)
			{
				recorrido19 = false;
				recorrido20 = true;
			}
		}
		if (recorrido20)
		{
			variableXT -= 0.07f;

			if (variableXT <= -2.8f)
			{
				recorrido20 = false;
				recorrido19 = true;
			}
		}
	}
	
	//Cola peces
	if (animacion3)
	{

		//Codigo recorrido en el escenario 
		if (recorrido5)
		{
			variableX += 0.3f;

			if (variableX >= 25.0f)
			{
				recorrido5 = false;
				recorrido6 = true;
			}
		}
		if (recorrido6)
		{
			variableX -= 0.3f;

			if (variableX <= -25.0f)
			{
				recorrido6 = false;
				recorrido5 = true;
			}
		}
	}
	//Cola Ballena
	if (animacion3)
	{

		//Codigo recorrido en el escenario 
		if (recorrido5)
		{
			variableXB += 0.5f;

			if (variableXB >= 25.0f)
			{
				recorrido5 = false;
				recorrido6 = true;
			}
		}
		if (recorrido6)
		{
			variableXB -= 0.5f;

			if (variableXB <= -25.0f)
			{
				recorrido6 = false;
				recorrido5 = true;
			}
		}
	}

	//Recorrido peces 

	if (recorrido7)
	{
		movPez_z += 0.05f;
		movPez_y += 0.009;
		orientaPez = 0.0f;
		if (movPez_z >= 100.0f)
		{
			recorrido7 = false;
			recorrido8 = true;
		}
	}
	if (recorrido8)
	{
		movPez_z -= 0.05f;
		movPez_y -= 0.008;
		orientaPez = 180.0f;
		if (movPez_z <= 40.0f)
		{
			recorrido8 = false;
			recorrido7 = true;
		}
	}
	//Alas Gaviota
	if (animacion8)
	{

		//Codigo recorrido en el escenario 
		if (recorrido21)
		{
			variableXG += 0.3f;

			if (variableXG >= 5.0f)
			{
				recorrido21 = false;
				recorrido22 = true;
			}
		}
		if (recorrido22)
		{
			variableXG -= 0.3f;

			if (variableXG <= -35.0f)
			{
				recorrido22 = false;
				recorrido21 = true;
			}
		}
	}

	//Recorrido cangrejos 

	if (recorrido9)
	{
		movCan_x += 0.2f;
		if (movCan_x >= 35.0f)
		{
			recorrido9 = false;
			recorrido10 = true;
		}
	}
	if (recorrido10)
	{
		movCan_x -= 0.2f;

		if (movCan_x <= -10.0f)
		{
			recorrido10 = false;
			recorrido9 = true;
		}
	}
	//Estacionar auto
	if (animacion2)
	{

		//Codigo recorrido en el escenario 
		if (recorrido1)
		{
			
			movAuto_x = -600.0f;
			movAuto_z += 4.5f;
			orienta = 0.0f;
			if (movAuto_z >= 430.0f)
			{
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2)
		{
			movAuto_x += 0.3f;
			movAuto_z = 430.0f;
			orienta = -90.0f;
			if (movAuto_x >= -530.0f)
			{
				recorrido2 = false;
				recorrido3 = true;
			}
		}

		if (recorrido3)
		{
			movAuto_x = -530.0f;
			movAuto_z -= 0.3f;
			orienta = 180.0f;

			if (movAuto_z <= 400.0f)
			{
				recorrido3 = false;
				recorrido4 = true;
			}
		}

		if (recorrido4)
		{
			movAuto_x = -530.0f;
			movAuto_z += 0.3f;

			if (movAuto_z >= 555.0f)
			{
				recorrido4 = false;
				recorrido5 = false;
			}
		}
	}
	//Paseo Tiburones 
	if (animacion5)
	{

		if (recorrido11)
		{
			movT_x = 0.0f;
			movT_z += 1.0f;
			orientaT = 90.0f;
			if (movT_z >= 185.0f)
			{
				recorrido11 = false;
				recorrido12 = true;
			}
		}
		if (recorrido12)
		{
			movT_x -= 1.0f;
			movT_z += 0.0f;
			orientaT = 0.0f;

			if (movT_x <= -200.0f)
			{
				recorrido12 = false;
				recorrido13 = true;
			}
		}

		if (recorrido13)
		{
			movT_x += 0.0f;
			movT_z -= 1.0f;
			orientaT = 270.0f;

			if (movT_z <= 20.0f)
			{
				recorrido13 = false;
				recorrido14 = true;
			}
		}

		if (recorrido14)
		{
			movT_x += 1.0f;
			movT_z += 0.0f;
			orientaT = 180.0f;

			if (movT_x >= -10.0f)
			{
				recorrido14 = false;
				recorrido11 = true;
			}
		}

	}
	
	//Paseo Gaviota
	if (animacion9)
	{
		
		if (recorrido31)
		{
			movGav_x += 0.0f;
			movGav_z += 1.0f;
			movGav_y += 0.001;
			orientaGav = 0.0f;
			if (movGav_z >= 500.0f)
			{
				recorrido31 = false;
				recorrido32 = true;
			}
		}
		if (recorrido32)
		{
			movGav_x -= 0.625 * 1.0f;
			movGav_z -= 1.0 * 1.0f;
			orientaGav = 212.0f; 
			movGav_y -= 0.001;
			if (movGav_x <= -500.0f && movGav_x <= -625.0f)
			{
				recorrido32 = false;
				recorrido33 = true;
			}
		}

		if (recorrido33)
		{
			movGav_x += 1.25 * 1.0f;
			movGav_z += 1.0 * 1.0f;
			orientaGav = 51.34f; 
			if (movGav_z >= 0.0f && movGav_x >= 0.0f)
			{
				recorrido33 = false;
				recorrido31 = true;
			}
		}

	}
	//Paseo Ballenas 
	if (animacion4)
	{

		if (recorrido15)
		{
	
			movB_x += 0.0f;
			movB_z += 0.5f;
			movB_y += 0.1;
			orientaB = 90.0f;
		
			if (movB_z >= 140.0f)
			{
				recorrido15 = false;
				recorrido16 = true;
			}
		}
		if (recorrido16)
		{
			movB_x -= 0.5f;
			movB_z += 0.0f;
			movB_y += 0.1;
			orientaB = 0.0f;

			if (movB_x <= -185.0f)
			{
				recorrido16 = false;
				recorrido17 = true;
			}
		}

		if (recorrido17)
		{
			movB_x += 0.0f;
			movB_z -= 0.5f;
			movB_y -= 0.1;
			orientaB = 270.0f;

			if (movB_z <= +25.0f)
			{
				recorrido17 = false;
				recorrido18 = true;
			}
		}

		if (recorrido18)
		{
			movB_x += 0.5f;
			movB_z += 0.0f;
			movB_y -= 0.1;
			orientaB = 180.0f;

			if (movB_x >= -15.0f)
			{
				recorrido18 = false;
				recorrido15 = true;
			}
		}

	}
}
void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model arbol("resources/objects/piso/piso.obj");
	Model manati("resources/objects/manati/12957_Manatee_v1_l3.obj");
	Model tortuga("resources/objects/tortuga/10042_Sea_Turtle_V2_iterations-2.obj");
	Model tiburon1("resources/objects/tiburones/2/Shark_v2.obj");
	Model tiburon2("resources/objects/tiburones/3/Bambooshark.obj");
	Model pinguino("resources/objects/pinguino/PenguinBaseMesh.obj");
	Model morsa("resources/objects/morsa/10053_Walrus_v1_L3.obj");
	
	//Ballena por partes
	Model ballena("resources/objects/ballena/cuerpo.obj");
	Model ballenacola("resources/objects/ballena/cola.obj");
	
	Model bashfish("resources/objects/bassfish/bass.obj");
	Model cangrejo("resources/objects/cangrejo/10012_crab_v2_iterations-1.obj");
	Model camaron("resources/objects/camaron/10048_Shrimp_v1_L3.obj");
	
	//Peces por partes 
	Model colapez1("resources/objects/Fishes/tropical1/cola.obj");
	Model cuerpopez1("resources/objects/Fishes/tropical1/cuerpo.obj");
	
	//Tiburon por partes 
	Model colatiburon("resources/objects/tiburones/2/cola.obj");
	Model cuerpotiburon("resources/objects/tiburones/2/cuerpo.obj");
	Model aleta1tiburon("resources/objects/tiburones/2/aleta1.obj");
	Model aleta2tiburon("resources/objects/tiburones/2/aleta2.obj");


	//Personaje
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	
	//Carro
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model piso2("resources/objects/piso2/Piso2.obj");
	Model estructura("resources/objects/estructura/estructura/estructura.obj");
	Model letras("resources/objects/amg/LetrasAcuario.obj");
	Model bob("resources/objects/bob/bob.obj");
	Model banca("resources/objects/banca/banca.obj");
	Model estrella("resources/objects/estrellamar/Lowpoly_Starfish.obj");
	
	//Gaviota por partes 
	Model gaviota("resources/objects/gaviota/gaviota/cuerpo.obj");
	Model gaviota2("resources/objects/gaviota/gaviota/alas.obj");
	Model gaviota3("resources/objects/gaviota/gaviota/mediaAla.obj");
	Model gaviota4("resources/objects/gaviota/gaviota/puntaALA.obj");


	Model hielo1("resources/objects/hielo/EstructuraDePoligonos.obj");
	Model hielo2("resources/objects/hielo/bloquesdehielo.obj");
	Model hielo3("resources/objects/hielo/cueva/cueva.obj");
	Model parque("resources/objects/parque/parque.obj");
	Model roca("resources/objects/roca/rocap1.obj");
	Model palmera("resources/objects/arboles/palmera.obj");
	Model cafeteria("resources/objects/cafeteria/cafe.obj");
	Model pino("resources/objects/arboles/pino/por_tree.obj");
	Model fuente("resources/objects/Fuente/f1.obj");
	Model hellow("resources/objects/Bote/bin.obj");
	//Model banca("resources/objects/banca/Wooden Bench Weathered 2.obj");
	
	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	//animacionPersonaje.initShaders(animShader.ID);
	
	ModelAnim praying("resources/objects/praying/Praying.dae");
	praying.initShaders(animShader.ID);

	ModelAnim acariciar("resources/objects/acariciar/PettingAnimal.dae");
	acariciar.initShaders(animShader.ID);

	ModelAnim zombie("resources/objects/zombie/ZombieBiting.dae");
	zombie.initShaders(animShader.ID);

	ModelAnim cmn("resources/objects/cmn/Taunt.dae");
	cmn.initShaders(animShader.ID);
	
	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.7f, 0.7f, 0.7f)); //caras menos iluminadas
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.7f, 0.7f, 0.7f)); // caras mas iluminadas
		staticShader.setVec3("dirLight.specular", glm::vec3(0.3f, 0.3f, 0.3f)); //brillo
		
		//SOL
		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 0.7f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 0.7f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 1.0f, 0.7f));
		staticShader.setFloat("pointLight[0].constant", 0.00000000001f); //atenuacion de la luz de foquito controlan la intensidad (mayor control) 
		staticShader.setFloat("pointLight[0].linear", 0.09f);  //intensidad (menos control)
		staticShader.setFloat("pointLight[0].quadratic", 0.000032f); //mayor distancia mientas mas pequeño la atenuacion es menor 
		staticShader.setFloat("material_shininess", 32.0f);
		
		//Cambio de color
		//staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		//staticShader.setVec3("pointLight[1].ambient", glm::vec3(variableX, variableY, variableZ));
		//staticShader.setVec3("pointLight[1].diffuse", glm::vec3(variableX, variableY, variableZ));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0032f);
		staticShader.setFloat("material_shininess", 32.0f);
		
		//Teclado 
		//staticShader.setVec3("pointLight[2].position", glm::vec3(50.0, 4.0f, 0.0f));
		//staticShader.setVec3("pointLight[2].ambient", glm::vec3(LX, LY, LZ));
		//staticShader.setVec3("pointLight[2].diffuse", glm::vec3(LX, LY, LZ));
		//staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.0032f);
		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-140.0f, 17.75f, -250.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.19f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		praying.Draw(animShader);
		
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(88.0f, 30.75f, 115.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.19f));// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		acariciar.Draw(animShader);
		
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-550.0f, 1.75f, -550.f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.19f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		zombie.Draw(animShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-117.5f, 17.75f, 390.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.175f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		cmn.Draw(animShader);
		
		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);
		
		//Pez1 partes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f + movPez_x,  20+ movPez_y, 300 + movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableX), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		colapez1.Draw(staticShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f + movPez_x, 20 + movPez_y, 300 + movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		cuerpopez1.Draw(staticShader);
 
		//Pez 2 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-245.0f + movPez_x, 20+movPez_y, 304.5+movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableX), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		colapez1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-245.0f + movPez_x, 20+movPez_y, 304.5+movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		cuerpopez1.Draw(staticShader);

		//Pez 3 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-240.0f + movPez_x, 20+movPez_y, 303+movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableX), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		colapez1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-240.0f + movPez_x, 20+ movPez_y, 303+movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		cuerpopez1.Draw(staticShader);

		//Pez 4 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-235.0f + movPez_x, 20 + movPez_y, 300 + movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableX), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		colapez1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-235.0f + movPez_x, 20 + movPez_y, 300 + movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		cuerpopez1.Draw(staticShader);

		//Pez 5 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-255.0f + movPez_x, 20 + movPez_y, 302 + movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableX), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		colapez1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-255.0f + movPez_x, 20+movPez_y, 302 + movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		cuerpopez1.Draw(staticShader);

		//Pez 6 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-260.0f + movPez_x, 20+movPez_y, 299 + movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableX), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		colapez1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-260.0f + movPez_x, 20+movPez_y, 299 + movPez_z));
		tmp = model = glm::rotate(model, glm::radians(orientaPez), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		cuerpopez1.Draw(staticShader);
		
		//Bassfish
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-210.0f, 25.75f, 350.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		bashfish.Draw(staticShader);

		//Ballena por partes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(240.0f + movB_x, 45.75f + movB_y, -330.0f + movB_z));
		tmp = model = glm::rotate(model, glm::radians(orientaB), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		staticShader.setMat4("model", model);
		ballena.Draw(staticShader);

		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(240.0f + movB_x, 45.75f + movB_y, -330.0f + movB_z));
		tmp = model = glm::rotate(model, glm::radians(orientaB), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f));
		model = glm::rotate(model, glm::radians(variableXB), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		ballenacola.Draw(staticShader);

		//Tiburones
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-200.0f, 30.75f, -250.0f));
		model = glm::scale(model, glm::vec3(12.0f));
		staticShader.setMat4("model", model);
		tiburon2.Draw(staticShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-55.0f + movT_x, 30.75 + movT_y, -350 + movT_z));
		tmp = model = glm::rotate(model, glm::radians(orientaT), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableXT), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		colatiburon.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-55.0f + movT_x, 30.75 + movT_y, -350 + movT_z));
		tmp = model = glm::rotate(model, glm::radians(orientaT), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		cuerpotiburon.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-55.0f + movT_x, 30.75 + movT_y, -350 + movT_z));
		tmp = model = glm::rotate(model, glm::radians(orientaT), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableXT), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		aleta1tiburon.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-55.0f + movT_x, 30.75 + movT_y, -350 + movT_z));
		tmp = model = glm::rotate(model, glm::radians(orientaT), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		model = glm::rotate(model, glm::radians(variableXT), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		aleta2tiburon.Draw(staticShader);

		//Pinguino
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-120.0f, 15.75f, 350.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		staticShader.setMat4("model", model);
		pinguino.Draw(staticShader);

		//Hielo Pinguino
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-155.0f, 12.75f, 330.0f));
		model = glm::scale(model, glm::vec3(13.0f));
		staticShader.setMat4("model", model);
		hielo1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-155.0f, 12.75f, 410.0f));
		model = glm::scale(model, glm::vec3(13.0f));
		staticShader.setMat4("model", model);
		hielo2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-80.0f, 12.75f, 410.0f));
		model = glm::scale(model, glm::vec3(13.0f));
		staticShader.setMat4("model", model);
		hielo2.Draw(staticShader);

		//Estructura
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 5.75f, 150.0f));
		model = glm::scale(model, glm::vec3(30.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		estructura.Draw(staticShader);
		
		//Letras
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, -0.75f, 430.0f));
		model = glm::scale(model, glm::vec3(10.0f));
		staticShader.setMat4("model", model);
		letras.Draw(staticShader);
		
		//Piso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
        
		//Piso2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -5.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25));
		staticShader.setMat4("model", model);
		piso2.Draw(staticShader);

		//Morsa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 30.75f, 125.0f));
		model = glm::scale(model, glm::vec3(13.0f));
		model = glm::rotate(model, glm::radians(155.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		morsa.Draw(staticShader);

		//Iglu
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(135.0f, 12.75f, 450.0f));
		model = glm::scale(model, glm::vec3(19.0f));
		staticShader.setMat4("model", model);
		hielo3.Draw(staticShader);

		//Cangrejo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(140.0f+movCan_x, 15.75f, 370.0f+movCan_z));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		cangrejo.Draw(staticShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(120.0f + movCan_x, 15.75f, 360.0f + movCan_z));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		cangrejo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(160.0f + movCan_x, 15.75f, 350.0f + movCan_z));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		cangrejo.Draw(staticShader);

		//Rocas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(200.0f , 7.75f, 410.0f ));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(120.0f, 7.75f, 410.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(145.0f, 7.75f, 390.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);
		
		//Bob
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-550.0f, -0.75f, -550.0f));
		model = glm::scale(model, glm::vec3(47.0f));
		staticShader.setMat4("model", model);
		bob.Draw(staticShader);

		//Palmeras
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-500.0f, -0.75f, 500.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(500.0f, -0.75f, 500.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(550.0f, -0.75f, -550.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(580.0f, -0.75f, -570.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(570.0f, -0.75f, -570.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(590.0f, -0.75f, -580.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(590.0f, -0.75f, -590.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(580.0f, -0.75f, -560.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(570.0f, -0.75f, -560.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(600.0f, -0.75f, -600.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		palmera.Draw(staticShader);

		//Pino
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-550.0f, -0.75f, 260.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-550.0f, -0.75f, -210.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(550.0f, -0.75f, 200.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		staticShader.setMat4("model", model);
		pino.Draw(staticShader);

		//Hielo Exterior 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-350.0f, -0.75f, -550.0f));
		model = glm::scale(model, glm::vec3(47.0f));
		staticShader.setMat4("model", model);
		hielo1.Draw(staticShader);
		
		//Parque
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-500.0f, 1.75f, -20.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		parque.Draw(staticShader);

		//Fuente
		model = glm::translate(glm::mat4(1.0f), glm::vec3(490.0f, 1.75f, -20.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		staticShader.setMat4("model", model);
		fuente.Draw(staticShader);
		
		//Cafeteria
		model = glm::translate(glm::mat4(1.0f), glm::vec3(550.0f, -0.75f, -250.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.37f));
		staticShader.setMat4("model", model);
		cafeteria.Draw(staticShader);

		//Basura
		model = glm::translate(glm::mat4(1.0f), glm::vec3(510.0f, -0.75f, -300.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f));
		staticShader.setMat4("model", model);
		hellow.Draw(staticShader);
		
		//Gaviota 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f + movGav_x, 241.5f + movGav_y, 0.0f + movGav_z));
		model = glm::scale(model, glm::vec3(6.0f));
		tmp = model = glm::rotate(model, glm::radians(orientaGav), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		gaviota.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.0f + movGav_x, 241.5f + movGav_y, 0.0f +  movGav_z));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(variableXG), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(orientaGav), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		gaviota3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f + movGav_x, 241.5f + movGav_y, 0.0f + movGav_z));
		model = glm::scale(model, glm::vec3(6.0f));
		tmp = model = glm::rotate(model, glm::radians(variableXG), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = model = glm::rotate(model, glm::radians(orientaGav), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		gaviota4.Draw(staticShader);


		//Banca
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-590.0f, -0.75f, -350.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-520.0f, -0.75f, -350.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-590.0f, -0.75f, 150.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-520.0f, -0.75f, 150.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-590.0f, -0.75f, 350.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-520.0f, -0.75f, 350.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(590.0f, -0.75f, 350.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(520.0f, -0.75f, 350.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		banca.Draw(staticShader);


		//Estrella de Mar 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-210.0f, 15.75f, 365.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		estrella.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-200.0f, 15.75f, 385.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		estrella.Draw(staticShader);
		
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-215.0f, 15.75f, 390.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		estrella.Draw(staticShader);

		//Manati
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-100.0f, 15.75f, 125.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		manati.Draw(staticShader);

		//Tortugas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, 15.75f, 360.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		tortuga.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, 15.75f, 385.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		tortuga.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f, 15.75f, 370.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		tortuga.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.0f, 15.75f, 370.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		tortuga.Draw(staticShader);

		//Rocas tortugas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-40.0f, 7.75f, 410.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f, 7.75f, 390.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);

		//Camaron
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, 15.75f, 340.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(22.0f, 15.75f, 337.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 15.75f, 338.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(18.0f, 15.75f, 332.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 15.75f, 330.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, 15.75f, 331.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(22.0f, 15.75f, 336.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.0f, 15.75f, 340.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, movAuto_y, movAuto_z));//manipulacion de objeto en el escenario
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 100, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		//torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 100.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, 100.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 100.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, 99.1f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, 99.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, 99.0, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, 99.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		variableX--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		variableX++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		animacion2 ^= true;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		animacion3 ^= true;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		animacion4 ^= true;
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		animacion5 ^= true;
	

	//Car animation
	//cambia el valor de la variable que mueve el carro
	//if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animacion = true;
		jet = 0.0f;
		//animacion ^= true;


	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		PlaySound(L"resources/sounds/ballena-llamada-2-.wav", NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
	
	}
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
	{
	PlaySound(L"resources/sounds/ES_Baby Penguin 3 - SFX Producer.wav", NULL, SND_FILENAME | SND_ASYNC);

	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
	PlaySound(L"resources/sounds/ES_Car Engine Rev 5 - SFX Producer.wav", NULL, SND_FILENAME | SND_ASYNC);

	}
	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
	{
		PlaySound(L"resources/sounds/metc.wav", NULL, SND_FILENAME | SND_ASYNC);

	}
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
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
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	///Vehículo
	if (animacion)
	{
		if (jet == 0.0f) {
			movAuto_z -= 3.0f;
			giroLlanta -= 3.0f;
			if (movAuto_z <= -170.0f)
				jet = 1.0f;
		}

		if (jet == 1.0f) {
			movAuto_y += 3.0f;
			if (movAuto_y >= 200.5f)
				jet = 2.0f;
		}
		if (jet == 2.0f) {
			movAuto_z += 3.0f;
			if (movAuto_z >= 240.5f)
				jet = 3.0f;
		}

		if (jet == 3.0f) {
			movAuto_y -= 3.0f;
			if (movAuto_y <= 0.0f)
				jet = 4.0f;
		}
		if (jet == 4.0f) {
			movAuto_z += 3.0f;
			if (movAuto_z >= 540.5f)
				jet = 5.0f;
		}

	}

}
void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model arbol("resources/objects/piso/piso.obj");
	Model manati("resources/objects/manati/12957_Manatee_v1_l3.obj");
	Model tortuga("resources/objects/tortuga/10042_Sea_Turtle_V2_iterations-2.obj");
	Model tiburon1("resources/objects/tiburones/2/Shark_v2.obj");
	Model tiburon2("resources/objects/tiburones/3/Bambooshark.obj");
	Model pinguino("resources/objects/pinguino/PenguinBaseMesh.obj");
	Model morsa("resources/objects/morsa/10053_Walrus_v1_L3.obj");
	Model ballena("resources/objects/ballena/10054_Whale_v2_L3.obj");
	Model bashfish("resources/objects/bassfish/bass.obj");
	Model cangrejo("resources/objects/cangrejo/10012_crab_v2_iterations-1.obj");
	Model camaron("resources/objects/camaron/10048_Shrimp_v1_L3.obj");
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	//Model casaVieja("resources/objects/casa/OldHouse.obj");
	//Model cubo("resources/objects/cubo/cube02.obj");
	//Model casaDoll("resources/objects/casa/DollHouse.obj");
	//Model banca("resources/objects/banca/Wooden Bench Weathered 2.obj");

	//ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	//animacionPersonaje.initShaders(animShader.ID);

	ModelAnim praying("resources/objects/praying/Praying.dae");
	praying.initShaders(animShader.ID);

	ModelAnim acariciar("resources/objects/acariciar/PettingAnimal.dae");
	acariciar.initShaders(animShader.ID);

	ModelAnim zombie("resources/objects/zombie/ZombieBiting.dae");
	zombie.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.7f, 0.7f, 0.7f)); //caras menos iluminadas
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.3f, 0.3f, 0.3f)); // caras mas iluminadas
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f)); //brillo
		
		//SOL
		//staticShader.setVec3("pointLight[0].position", lightPosition);
		//staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 0.7f));
		//staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 0.7f));
		//staticShader.setVec3("pointLight[0].specular", glm::vec3(1.0f, 1.0f, 0.7f));
		staticShader.setFloat("pointLight[0].constant", 0.00000000001f); //atenuacion de la luz de foquito controlan la intensidad (mayor control) 
		staticShader.setFloat("pointLight[0].linear", 0.09f);  //intensidad (menos control)
		staticShader.setFloat("pointLight[0].quadratic", 0.000032f); //mayor distancia mientas mas pequeño la atenuacion es menor 
		staticShader.setFloat("material_shininess", 32.0f);
		
		//Cambio de color
		//staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		//staticShader.setVec3("pointLight[1].ambient", glm::vec3(variableX, variableY, variableZ));
		//staticShader.setVec3("pointLight[1].diffuse", glm::vec3(variableX, variableY, variableZ));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.0032f);
		staticShader.setFloat("material_shininess", 32.0f);
		
		//Teclado 
		//staticShader.setVec3("pointLight[2].position", glm::vec3(50.0, 4.0f, 0.0f));
		//staticShader.setVec3("pointLight[2].ambient", glm::vec3(LX, LY, LZ));
		//staticShader.setVec3("pointLight[2].diffuse", glm::vec3(LX, LY, LZ));
		//staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.0032f);
		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		praying.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-140.3f, 1.75f, 200.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		acariciar.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(240.3f, 1.75f, 100.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		zombie.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//casaDoll.Draw(staticShader);
			
		
		//Tiburones
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(100.0f, 1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		tiburon1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(140.0f, 1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		tiburon2.Draw(staticShader);
		
		//Pinguino
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(140.0f, 50.75f, 50.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		pinguino.Draw(staticShader);

		//Morsa
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(140.0f, 150.75f, 50.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		morsa.Draw(staticShader);

		//Cangrejo
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(140.0f, 1.75f, 50.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		cangrejo.Draw(staticShader);

		//Camaron
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(140.0f, 1.75f, 150.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		camaron.Draw(staticShader);
		
		//Ballena
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(140.0f, 1.75f, 250.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		ballena.Draw(staticShader);

		//Bassfish
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-100.0f, 1.75f, 250.0f));
		model = glm::scale(model, glm::vec3(7.0f));
		staticShader.setMat4("model", model);
		bashfish.Draw(staticShader);

		//Piso
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
        
		//Manati
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 14.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		manati.Draw(staticShader);

		//Tortugas
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.0f, 5.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		tortuga.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(55.0f, 5.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		tortuga.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(70.0f, 5.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		tortuga.Draw(staticShader);

		

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(25.0f));
		staticShader.setMat4("model", model);
		//banca.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, movAuto_y, movAuto_z));//manipulacion de objeto en el escenario
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::rotate(model, glm::radians(giroLlanta), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		//torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		movAuto_z = 0.0f;

	//Car animation
	//cambia el valor de la variable que mueve el carro
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		animacion = true;
		jet = 0.0f;
		//animacion ^= true;


	}
	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
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
