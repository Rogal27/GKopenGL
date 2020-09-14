#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "light.h"

using namespace std;

Scene::Scene() : activeShaderIndex(-1), activeCameraIndex(-1), isFogActive(false), backgroundColor(0.5f, 0.8f, 0.95f)
{

}

Scene::~Scene()
{
	for (Camera* camera : cameras)
	{
		delete camera;
	}
	for (Model* model : models)
	{
		delete model;
	}
}

void Scene::AddModel(Model* model)
{
	models.push_back(model);
}

void Scene::AddShader(Shader shader)
{
	if (shaders.size() == 0)
		activeShaderIndex = 0;
	shaders.push_back(shader);
}

void Scene::AddCamera(Camera* camera)
{
	if (cameras.size() == 0)
		activeCameraIndex = 0;
	cameras.push_back(camera);
}

bool Scene::SetActiveShader(int index)
{
	if (index < 0 && index >= shaders.size())
		return false;
	activeShaderIndex = index;
	return true;
}

bool Scene::SetActiveCamera(int index)
{
	if (index < 0 && index >= cameras.size())
		return false;
	activeCameraIndex = index;
	return true;
}

void Scene::SetBackgroundColor(glm::vec3 color)
{
	backgroundColor = color;
}

int Scene::GetShadersCount()
{
	return shaders.size();
}

int Scene::GetCamerasCount()
{
	return cameras.size();
}

bool Scene::GetFogState()
{
	return isFogActive;
}

void Scene::SwitchShader()
{
	if (shaders.size() == 0)
		return;
	activeShaderIndex++;
	activeShaderIndex %= shaders.size();
}

void Scene::SwitchCamera()
{
	if (cameras.size() == 0)
		return;
	activeCameraIndex++;
	activeCameraIndex %= cameras.size();
}

void Scene::SwitchFog()
{
	isFogActive = !isFogActive;
}

Shader Scene::GetActiveShader()
{
	return shaders[activeShaderIndex];
}

Camera* Scene::GetActiveCamera()
{
	return cameras[activeCameraIndex];
}

void Scene::Draw(const float& width, const float& height, const double& time)
{
	if (cameras.size() == 0)
		return;

	Camera* camera = GetActiveCamera();
	if (camera == nullptr)
		return;

	Shader shader = GetActiveShader();
	shader.use();

	SetShaderCameraMatrices(camera, shader, width, height);

	for (Model* model : models)
	{
		model->Draw(shader);
	}
}

void Scene::SetShaderCameraMatrices(Camera* camera, const Shader& shader, const float& width, const float& height)
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 normalMatrix = glm::mat4(1.0f);
	glm::mat4 projection = camera->GetProjectionMatrix(width, height);
	glm::mat4 view = camera->GetViewMatrix();

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera->GetPosition());
	shader.setBool("isFogActive", isFogActive);
	shader.setVec3("backgroundColor", backgroundColor);
}
