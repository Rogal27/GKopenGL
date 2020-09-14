#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>
#include <algorithm>

#include "shader.h"
#include "shaderfactory.h"
#include "camera.h"
#include "moveablecamera.h"
#include "light.h"
#include "directLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "model.h"
#include "lightenscene.h"
#include "scenefactory.h"

//window size
int WIDTH = 1280;
int HEIGHT = 720;

glm::vec3 sky_color = glm::vec3(0.5f, 0.8f, 0.95f);
glm::vec3 fog_color = glm::vec3(0.65f, 0.65f, 0.65f);

LightenScene* mainScene = nullptr;

//is some key pressed
bool isPPressed = false;

float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//time
double deltaTime = 0.0f;	// Time between current frame and last frame
double lastFrame = 0.0f; // Time of last frame

//shader type
//0-phong, 1-blinn, 2-gouraud
int shader_type = 0;

//prototypes
GLFWwindow* CreateMainWindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void AnimateRunwayLines(LightenScene* scene, double currentTime);
void AnimateFireTruckSiren(LightenScene* scene, double currentTime);
void AnimateFireTruckSelfDriving(LightenScene* scene, double currentTime);
void FollowFireTruckSelfDrivingStaticCamera(LightenScene* scene);
void FollowFireTruckSelfDrivingMovingCamera(LightenScene* scene);
void MoveLightSpotWithCamera5(LightenScene* scene);


int main()
{
	GLFWwindow* window = CreateMainWindow();

	double previousTime = glfwGetTime();
	int frameCount = 0;
	std::string s = "";

	//create scene
	SceneFactory sf = SceneFactory();
	mainScene = sf.MainLightenScene();

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		//per-frame time logic
		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;
		if (currentFrame - previousTime >= 1.0)
		{
			s.append("GK OpenGL [FPS:");
			s.append(std::to_string((int)frameCount));
			s.append("]");
			glfwSetWindowTitle(window, s.c_str());
			s.clear();
			frameCount = 0;
			previousTime = currentFrame;
		}

		// input
		// -----
		processInput(window);

		//day and night
		float sin_value = sin(glm::radians(3 * currentFrame));
		float multiplier = (sin_value + 1.0f) / 2.0f;
		glm::vec3 current_sky_color = glm::vec3(1.0f);
		if (mainScene->GetFogState())
			current_sky_color = fog_color * std::clamp(multiplier, 0.0f, 1.0f);
		else
			current_sky_color = sky_color * multiplier;

		glClearColor(current_sky_color.x, current_sky_color.y, current_sky_color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Light* dirLight = mainScene->lights[0];
		dirLight->setDiffuse(glm::vec3(0.8f) * multiplier);
		dirLight->setSpecular(glm::vec3(1.0f) * multiplier);
		mainScene->SetBackgroundColor(current_sky_color);

		//runwayLights
		AnimateRunwayLines(mainScene, currentFrame);
		//fire truck siren
		AnimateFireTruckSiren(mainScene, currentFrame);
		//Animate moving truck
		AnimateFireTruckSelfDriving(mainScene, currentFrame);
		//follow truck with static camera
		FollowFireTruckSelfDrivingStaticCamera(mainScene);
		//move light spot with camera5
		MoveLightSpotWithCamera5(mainScene);

		mainScene->Draw(WIDTH, HEIGHT, 0.0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete mainScene;

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}

GLFWwindow* CreateMainWindow()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GK OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSwapInterval(1);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glEnable(GL_DEPTH_TEST);

	return window;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (mainScene == nullptr)
		return;
	Camera* camera = mainScene->GetActiveCamera();
	if (camera == nullptr)
		return;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraMovement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera->ProcessKeyboard(CameraMovement::DOWN, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (isPPressed == false)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		isPPressed = !isPPressed;
	}
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		if (mainScene == nullptr)
			return;
		mainScene->SwitchShader();

		shader_type++;
		shader_type %= 3;

		if (shader_type == 0)
		{
			cout << "Phong shading" << endl;
		}
		else if (shader_type == 1)
		{
			cout << "Blinn shading" << endl;
		}
		else if (shader_type == 2)
		{
			cout << "Gouraud shading" << endl;
		}
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		if (mainScene == nullptr)
			return;
		mainScene->SwitchCamera();
	}
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (mainScene == nullptr)
			return;
		mainScene->SwitchFog();
	}
}

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

	if (mainScene == nullptr)
		return;
	Camera* camera = mainScene->GetActiveCamera();
	if (camera == nullptr)
		return;

	camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (mainScene == nullptr)
		return;
	Camera* camera = mainScene->GetActiveCamera();
	if (camera == nullptr)
		return;

	camera->ProcessMouseScroll(yoffset);
}

void AnimateRunwayLines(LightenScene* scene, double currentTime)
{
	int offset = 1;
	int lights_count = 16;
	auto orange = glm::vec3(0.96f, 0.68f, 0.0f);
	for (int i = offset; i < offset + lights_count; i+=2)
	{
		glm::vec3 color = orange * (sin(glm::radians(i * 10.0f + 150.0f * static_cast<float>(currentTime))) + 1.3f) / 2.0f;
		scene->lights[2 * offset + lights_count - i - 1]->setColor(color);
		scene->lights[2 * offset + lights_count - i - 2]->setColor(color);
	}
}

void AnimateFireTruckSiren(LightenScene* scene, double currentTime)
{
	int dir_lights = 1;
	int runway_lights = 16;
	int offset = dir_lights + runway_lights;
	int sirens_count = 4;
	auto red = glm::vec3(1.0f, 0.0f, 0.0f);
	auto blue = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 white = glm::vec3(1.0f);
	for (int i = offset; i < offset + sirens_count; i += 2)
	{
		float red_pow = pow(sin(glm::radians(i * 10.0f + 400.0f * static_cast<float>(currentTime))), 2);
		float blue_pow = pow(cos(glm::radians(i * 10.0f + 400.0f * static_cast<float>(currentTime))), 2);
		glm::vec3 red_siren = red * red_pow + white * blue_pow / 1.5f;
		glm::vec3 blue_siren = blue * blue_pow + white * red_pow / 1.5f;
		scene->lights[i]->setColor(red_siren);
		scene->lights[i + 1]->setColor(blue_siren);
	}
}

void AnimateFireTruckSelfDriving(LightenScene* scene, double currentTime)
{
	int offset = 23;
	int lights_count = 2;
	Model* truck = scene->models[1];
	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::translate(rotation, glm::vec3(-70.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, glm::radians(static_cast<float>(10*currentTime)), glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::translate(rotation, glm::vec3(70.0f, 0.0f, 0.0f));
	truck->SetModelMatrix(rotation);
	for (int i = offset; i < offset + lights_count; i++)
	{
		SpotLight* light = dynamic_cast<SpotLight*>(scene->lights[i]);
		//glm::vec3 pos = light->getPosition();
		//glm::vec3 dir = light->getDirection();
		//light->setPosition(glm::vec3(rotation * glm::vec4(pos, 0.0f)));
		light->model.SetModelMatrix(rotation);		
	}
	FollowFireTruckSelfDrivingMovingCamera(mainScene);
}

void FollowFireTruckSelfDrivingStaticCamera(LightenScene* scene)
{
	Model* truck = scene->models[1];
	Camera* follow = scene->cameras[2];

	glm::vec3 target_pos = glm::vec3(truck->GetModelMatrix() * truck->GetPositionMatrix() * glm::vec4(truck->GetFirstPoint(), 1.0f));

	follow->ChangeTarget(target_pos);
}

void FollowFireTruckSelfDrivingMovingCamera(LightenScene* scene)
{
	Model* truck = scene->models[1];
	Camera* follow = scene->cameras[3];

	glm::mat4 rotation = truck->GetModelMatrix() * truck->GetPositionMatrix();

	glm::vec3 target_pos = glm::vec3(rotation * glm::vec4(truck->GetFirstPoint(), 1.0f));

	glm::vec3 truck_point = glm::vec3(truck->GetPositionMatrix() * glm::vec4(truck->GetFirstPoint(), 1.0f));

	glm::vec3 camera_pos = glm::vec3(0.0f, 5.0f, 20.0f) + truck_point;
	camera_pos = glm::vec3(truck->GetModelMatrix() * glm::vec4(camera_pos, 1.0f));

	follow->FollowObject(target_pos, target_pos - camera_pos);
}

void MoveLightSpotWithCamera5(LightenScene* scene)
{
	int offset = 27;
	Camera* camera = scene->cameras[4];
	SpotLight* light = dynamic_cast<SpotLight*>(scene->lights[offset]);
	light->setPosition(camera->GetPosition());
	light->setDirection(camera->GetFront());
}
