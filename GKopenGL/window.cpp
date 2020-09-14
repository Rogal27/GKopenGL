#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cmath>

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
//0-phong, 1-gouraud
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
		float sin_value = sin(glm::radians(8*currentFrame));
		glm::vec3 current_sky_color = sky_color * (sin_value + 1.0f) / 2.0f;

		glClearColor(current_sky_color.x, current_sky_color.y, current_sky_color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Light* dirLight = mainScene->lights[0];
		DirectLight* directLight = dynamic_cast<DirectLight*>(dirLight);
		if (sin_value > -0.5)
		{	
			glm::vec3 direction = glm::normalize(glm::vec3(-sin_value, -1.0f, -sin_value));
			directLight->setDirection(direction);
			directLight->TurnOn();
		}
		else
		{
			directLight->TurnOff();
		}

		//runwayLights
		AnimateRunwayLines(mainScene, currentFrame);
		//fire truck siren
		AnimateFireTruckSiren(mainScene, currentFrame);
		//Animate moving truck
		AnimateFireTruckSelfDriving(mainScene, currentFrame);





		//move light
	   /* lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
		light_point.setPosition(lightPos);

		float r = sin(glfwGetTime());
		float g = cos(glfwGetTime());

		light_point2.setColor(r * r, g * g, r * g + 0.5f);
		light_point3.setColor(r * r, r * g + 0.5f, g*g);

		light_spot.setPosition(camera.GetPosition());
		light_spot.setDirection(camera.GetFront());*/

		// be sure to activate shader when setting uniforms/drawing objects
		//shaders[shader_type].use();

		// view/projection transformations
		/*glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 projection = camera.GetProjectionMatrix((float)WIDTH, (float)HEIGHT);
		glm::mat4 view = camera.GetViewMatrix();*/

		// world transformations

		//glm::mat3 NormalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

		//vertex shader uniforms
		//shaders[shader_type].setMat4("model", model);
		//shaders[shader_type].setMat4("view", view);
		//shaders[shader_type].setMat4("projection", projection);
		//shaders[shader_type].setMat3("NormalMatrix", NormalMatrix);

		//shaders[shader_type].setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		//shaders[shader_type].setVec3("viewPos", camera.GetPosition());

		//lights
		//int dirLights = 0, pointLights = 0, spotLights = 0;
		//shaders[shader_type].setInt("dirLightsCount", 1);
		//shaders[shader_type].setInt("pointLightsCount", 3);
		//shaders[shader_type].setInt("spotLightsCount", 1);

		//light_point.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);
		//light_point2.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);
		//light_point3.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);
		//light_spot.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);
		//light_dir.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);

		// render the cube

		//model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

		//shaders[shader_type].setMat4("model", model);

		//glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//model = glm::mat4(1.0f);
		////model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down

		//shaders[shader_type].setMat4("model", model);

		//glBindVertexArray(cubeVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// render the loaded model

		//shaders[shader_type].setMat4("view", view);
		//shaders[shader_type].setMat4("projection", projection);

		//for (size_t i = 0; i < 10; i++)
		//{
		//    for (size_t j = 0; j < 10; j++)
		//    {
		//        model = glm::mat4(1.0f);
		//        model = glm::translate(model, glm::vec3(i, -1.75f, j)); // translate it down so it's at the center of the scene
		//        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		//        float angle = glfwGetTime() * 25.0f * i + j;
		//        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		//        NormalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

		//        shaders[shader_type].setMat4("model", model);

		//        shaders[shader_type].setMat3("NormalMatrix", NormalMatrix);
		//        ourModel.Draw(shaders[shader_type]);
		//    }
		//
		//}

		//model
		//glm::mat4 model = glm::mat4(1.0f);

		mainScene->Draw(WIDTH, HEIGHT, 0.0);

		//Camera* camera = mainScene->GetActiveCamera();
		//if (camera == nullptr)
		//	continue;

		//glm::mat4 projection = camera->GetProjectionMatrix(WIDTH, HEIGHT);
		//glm::mat4 view = camera->GetViewMatrix();

		//// also draw the lamp object
		//lampShader.use();
		//lampShader.setVec3("lightColor", light_point.getColor());
		//lampShader.setMat4("projection", projection);
		//lampShader.setMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, light_point.getPosition());
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		//lampShader.setMat4("model", model);

		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//lampShader.use();
		//lampShader.setVec3("lightColor", light_point2.getColor());
		//lampShader.setMat4("projection", projection);
		//lampShader.setMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, light_point2.getPosition());
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		//lampShader.setMat4("model", model);

		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//lampShader.use();
		//lampShader.setVec3("lightColor", light_point3.getColor());
		//lampShader.setMat4("projection", projection);
		//lampShader.setMat4("view", view);
		//model = glm::mat4(1.0f);
		//model = glm::translate(model, light_point3.getPosition());
		//model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		//lampShader.setMat4("model", model);

		//glBindVertexArray(lightVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();

		/*double endFrame = glfwGetTime();
		while (endFrame - currentFrame < FPS)
		{
			endFrame = glfwGetTime();
		}*/
	}
	delete mainScene;

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	//glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteVertexArrays(1, &lightVAO);
	//glDeleteBuffers(1, &VBO);

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
		shader_type %= 2;

		if (shader_type == 0)
		{
			cout << "Phong shading" << endl;
		}
		else if (shader_type == 1)
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
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		;
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
		scene->lights[i]->setColor(color);
		scene->lights[i + 1]->setColor(color);
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
		glm::vec3 pos = light->getPosition();
		glm::vec3 dir = light->getDirection();
		light->setPosition(glm::vec3(rotation * glm::vec4(pos, 0.0f)));
		light->model.SetModelMatrix(rotation);		
	}
}
