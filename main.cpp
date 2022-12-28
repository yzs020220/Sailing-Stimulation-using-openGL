#pragma execution_character_set("utf-16")
#include "Angel.h"
#include "TriMesh.h"
#include "Camera.h"
#include "MeshPainter.h"

#include <vector>
#include <string>

int WIDTH = 600;
int HEIGHT = 600;

int mainWindow;

Camera* camera = new Camera();
Light* light = new Light();
MeshPainter *painter = new MeshPainter();

// 这个用来回收和删除我们创建的物体对象
std::vector<TriMesh *> meshList;

Fluid* water = new Fluid();
TriMesh* ship = new TriMesh();
TriMesh* ground = new TriMesh();
TriMesh* rock = new TriMesh();
TriMesh* shark = new TriMesh();
TriMesh* cloud = new TriMesh();

TriMesh* skybox1 = new TriMesh();
TriMesh* skybox2 = new TriMesh();
TriMesh* skybox3 = new TriMesh();
TriMesh* skybox4 = new TriMesh();
TriMesh* skybox5 = new TriMesh();
TriMesh* skybox6 = new TriMesh();

float t = 0.0f;
float speed = 0.0f;
float dir = 0.0f;
float waterScale = 10.0f;
bool controlShark = false;

std::string vshader, fshader;
// 读取着色器并使用

void init()
{
	// 设置光源位置
	light->setTranslation(glm::vec3(0.0, waterScale / 2, -waterScale / 2));
	light->setAmbient(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 环境光
	light->setDiffuse(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 漫反射
	light->setSpecular(glm::vec4(1.0, 1.0, 1.0, 1.0)); // 镜面反射
	light->setAttenuation(1.0, 0.45, 0.075); // 衰减系数

	glm::vec4 mat_ambient = { 1, 1, 1,1.0f };
	glm::vec4 mat_diffuse = { 1, 1, 1, 1.0f };
	glm::vec4 mat_specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	float shine = 200.0f;

	ground->generateSquare(glm::vec3(1.0, 0.9, 0.8));
	ground->setTranslation(glm::vec3(0.0, -0.001, 0.0));
	ground->setRotation(glm::vec3(-90, 0, 0));
	ground->setScale(glm::vec3(waterScale, waterScale, 1.0));
	
	painter->addMesh(ground, "mesh_ground", "./assets/Texture_01_A.png", vshader, fshader, 4);
	meshList.push_back(ground);

	ship->setNormalize(true);
	ship->readObj("./assets/ship.obj");

	// 设置物体的旋转位移
	ship->setTranslation(glm::vec3(0.0, 0.5, 0.0));
	ship->setRotation(glm::vec3(0.0, -65.0, 0.0));
	ship->setScale(glm::vec3(1.0, 1.0, 1.0));

	// 设置材质
	ship->setAmbient(mat_ambient); // 环境光
	ship->setDiffuse(mat_diffuse); // 漫反射
	ship->setSpecular(mat_specular); // 镜面反射
	ship->setShininess(shine); //高光系数

	painter->addMesh(ship, "mesh_ship", "./assets/Texture_01_A.png", vshader, fshader, 3);
	meshList.push_back(ship);

	rock->setNormalize(true);
	rock->readObj("./assets/rock1.obj");
	rock->setTranslation(glm::vec3(2, 0.401, -2));
	rock->setRotation(glm::vec3(0.0, 0.0, 0.0));
	rock->setScale(glm::vec3(2.0, 2.0, 2.0));

	rock->setAmbient(mat_ambient); // 环境光
	rock->setDiffuse(mat_diffuse); // 漫反射
	rock->setSpecular(mat_specular); // 镜面反射
	rock->setShininess(shine); //高光系数

	painter->addMesh(rock, "mesh_rock", "./assets/Texture_01_A.png", vshader, fshader, 3);
	meshList.push_back(rock);

	shark->setNormalize(true);
	shark->readObj("./assets/shark.obj");
	shark->setTranslation(glm::vec3(-2, 0.201, -2));
	shark->setRotation(glm::vec3(0.0, -135, 0.0));
	shark->setScale(glm::vec3(1.0, 1.0, 1.0));

	shark->setAmbient(mat_ambient); // 环境光
	shark->setDiffuse(mat_diffuse); // 漫反射
	shark->setSpecular(mat_specular); // 镜面反射
	shark->setShininess(shine); //高光系数

	painter->addMesh(shark, "mesh_shark", "./assets/Texture_01_A.png", vshader, fshader, 3);
	meshList.push_back(shark);

	cloud->setNormalize(true);
	cloud->readObj("./assets/cloud.obj");
	cloud->setTranslation(glm::vec3(-2, 1.2, -2));
	cloud->setRotation(glm::vec3(0.0, -135, 0.0));
	cloud->setScale(glm::vec3(1.0, 1.0, 1.0));

	cloud->setAmbient(mat_ambient); // 环境光
	cloud->setDiffuse(mat_diffuse); // 漫反射
	cloud->setSpecular(mat_specular); // 镜面反射
	cloud->setShininess(shine); //高光系数

	painter->addMesh(cloud, "mesh_cloud", "./assets/Texture_01_A.png", vshader, fshader, 1);
	meshList.push_back(cloud);

	skybox1->generateSquare(glm::vec3(0.0, 0.0, 0.0));

	skybox1->setTranslation(glm::vec3(0.0, waterScale / 15, -waterScale / 2));
	skybox1->setRotation(glm::vec3(0.0, 0.0, 0.0));
	skybox1->setScale(glm::vec3(waterScale, waterScale, waterScale));

	painter->addMesh(skybox1, "skybox1", "./assets/_skybox_6.jpg", vshader, fshader, 1);
	meshList.push_back(skybox1);

	skybox2->generateSquare(glm::vec3(0.0, 0.0, 0.0));

	skybox2->setTranslation(glm::vec3(0.0, waterScale / 15, waterScale / 2));
	skybox2->setRotation(glm::vec3(0.0, 180.0, 0.0));
	skybox2->setScale(glm::vec3(waterScale, waterScale, waterScale));

	painter->addMesh(skybox2, "skybox2", "./assets/_skybox_8.jpg", vshader, fshader, 1);
	meshList.push_back(skybox2);

	skybox3->generateSquare(glm::vec3(0.0, 0.0, 0.0));

	skybox3->setTranslation(glm::vec3(-waterScale / 2, waterScale / 15, 0.0));
	skybox3->setRotation(glm::vec3(0.0, 90.0, 0.0));
	skybox3->setScale(glm::vec3(waterScale, waterScale, waterScale));

	painter->addMesh(skybox3, "skybox3", "./assets/_skybox_5.jpg", vshader, fshader, 1);
	meshList.push_back(skybox3);

	skybox4->generateSquare(glm::vec3(0.0, 0.0, 0.0));

	skybox4->setTranslation(glm::vec3(waterScale / 2, waterScale / 15, 0.0));
	skybox4->setRotation(glm::vec3(0.0, -90.0, 0.0));
	skybox4->setScale(glm::vec3(waterScale, waterScale, waterScale));

	painter->addMesh(skybox4, "skybox4", "./assets/_skybox_7.jpg", vshader, fshader, 1);
	meshList.push_back(skybox4);

	skybox5->generateSquare(glm::vec3(0.0, 0.0, 0.0));

	skybox5->setTranslation(glm::vec3(0.0, waterScale / 15 + waterScale / 2, 0.0));
	skybox5->setRotation(glm::vec3(90.0, 0.0, 0.0));
	skybox5->setScale(glm::vec3(waterScale, waterScale, waterScale));

	painter->addMesh(skybox5, "skybox5", "./assets/_skybox_2.jpg", vshader, fshader, 1);
	meshList.push_back(skybox5);

	skybox6->generateSquare(glm::vec3(0.0, 0.0, 0.0));

	skybox6->setTranslation(glm::vec3(0.0, waterScale / 15 -waterScale / 2, 0.0));
	skybox6->setRotation(glm::vec3(-90.0, 0.0, 0.0));
	skybox6->setScale(glm::vec3(waterScale, waterScale, waterScale));

	painter->addMesh(skybox6, "skybox6", "./assets/_skybox_10.jpg", vshader, fshader, 1);
	meshList.push_back(skybox6);

	water->generateSurface(30, glm::vec3(0.0, 0.0, 0.0), 0.03f, 0.0f, 0.05f);
	water->coefficientCount(0.1f);
	water->setTranslation(glm::vec3(0, 0.25, 0));
	water->setRotation(glm::vec3(0, 0, 0));
	water->setScale(glm::vec3(waterScale, 1.0, waterScale));

	painter->addMesh(water, "mesh_water", "./assets/water1.jpg", vshader, fshader, 2);
	meshList.push_back(water);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	// glClearColor(0.0, 0.0, 0.0, 1.0);
}



void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	painter->drawMeshes(light, camera);

	glDepthMask(GL_TRUE);
}


void printHelp()
{
	std::cout << "================================================" << std::endl;
	std::cout << "Use mouse to controll the light position (drag)." << std::endl;
	std::cout << "================================================" << std::endl << std::endl;

	std::cout << "Keyboard Usage" << std::endl;
	std::cout <<
		"[Window]" << std::endl <<
		"ESC:		Exit" << std::endl <<
		"h:		Print help message" << std::endl <<

		std::endl <<
		"[Camera]" << std::endl <<
		"SPACE:		Reset camera parameters" << std::endl <<
		"u/U:		Increase/Decrease the rotate angle" << std::endl <<
		"i/I:		Increase/Decrease the up angle" << std::endl <<
		"o/O:		Increase/Decrease the camera radius" << std::endl << std::endl <<
		"w/s:       Move forward/backward\n" <<
		"a/d:       Turn left/right\n" <<
		"1/2:       control ship/shark\n\n";
}

void printMsg()
{
	std::cout << "speed: " << speed << "  dir: " << dir << '\n';
}

// 键盘响应函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	float tmp;
	glm::vec4 ambient;
	if (action == GLFW_PRESS) {
		switch (key)
		{
		case GLFW_KEY_ESCAPE: exit(EXIT_SUCCESS); break;
		case GLFW_KEY_H: printHelp(); break;
		case GLFW_KEY_2: controlShark = true; speed = 0; dir = 0; break;
		case GLFW_KEY_1: controlShark = false; speed = 0; dir = 0; break;
		case GLFW_KEY_W: speed += 1; printMsg(); break;
		case GLFW_KEY_S: speed -= 1; printMsg(); break;
		case GLFW_KEY_A: dir += 1; printMsg(); break;
		case GLFW_KEY_D: dir -= 1; printMsg(); break;
		default:
			camera->keyboard(key, action, mode);
			break;
		}
	}
}

// 重新设置窗口
void reshape(GLsizei w, GLsizei h)
{
	glViewport(0, 0, w, h);
}

void cleanData() {
	// 释放内存
	
	delete camera;
	camera = NULL;

	delete light;
	light = NULL;

	painter->cleanMeshes();

	delete painter;
	painter = NULL;
	
	for (int i=0; i<meshList.size(); i++) {
		delete meshList[i];
	}
	meshList.clear();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(int argc, char **argv)
{
	vshader = "shaders/vshader.vert";
	fshader = "shaders/fshader.frag";

	// 初始化GLFW库，必须是应用程序调用的第一个GLFW函数
	glfwInit();

	// 配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// 配置窗口属性
	GLFWwindow* window = glfwCreateWindow(800, 800, "2020155013_杨子燊_期末大作业_", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// 调用任何OpenGL的函数之前初始化GLAD
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Init mesh, shaders, buffer
	init();
	// 输出帮助信息
	printHelp();
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		if (t > 3)
		{
			t = 0;
			water->updateSurfacePosition();
			painter->replaceMesh(water, "mesh_water", "./assets/water1.jpg", vshader, fshader, 2);
		}
		if (controlShark)
		{
			shark->shark_movement(speed);
			shark->changeDir(dir);
			glm::vec3 pos = shark->getTranslation();
			camera->focus(glm::vec4(pos, 1.0f));
			camera->updateCamera();
		}
		else
		{
			ship->movement(speed);
			if(fabs(speed) > 0.01f)
				ship->changeDir(dir);
			glm::vec3 pos = ship->getTranslation();
			pos.y = 0.5;
			camera->focus(glm::vec4(pos, 1.0f));
			camera->updateCamera();
		}
		glm::vec3 pos = ship->getTranslation();
		float offset = water->getOffset(pos / waterScale);
		pos.y = 0.5 + offset / 10;
		ship->setTranslation(pos);
		ship->shake(offset);
		display();
		t += 1;
		//reshape();

		// 交换颜色缓冲 以及 检查有没有触发什么事件（比如键盘输入、鼠标移动等）
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	cleanData();

	return 0;
}

// 每当窗口改变大小，GLFW会调用这个函数并填充相应的参数供你处理。
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow * window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
