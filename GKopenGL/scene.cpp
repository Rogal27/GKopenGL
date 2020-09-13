#include "scene.h"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "light.h"



Scene::Scene(Shader shader) : shader(shader), activeCameraIndex(-1)
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

void Scene::SetShader(Shader shader)
{
	this->shader = shader;
}

void Scene::AddCamera(Camera* camera)
{
	if (cameras.size() == 0)
		activeCameraIndex = 0;
	cameras.push_back(camera);
}

bool Scene::SetActiveCamera(int index)
{
	if (index < 0 && index >= cameras.size())
		return false;
	activeCameraIndex = index;
	return true;
}

Camera* Scene::GetActiveCamera()
{
	return cameras[activeCameraIndex];
}

int Scene::GetCamerasCount()
{
	return cameras.size();
}

void Scene::SwitchCamera()
{
	if (cameras.size() == 0)
		return;
	activeCameraIndex++;
	activeCameraIndex %= cameras.size();
}

void Scene::Draw(float width, float height, double time)
{
	if (cameras.size() == 0)
		return;

	Camera* camera = GetActiveCamera();
	if (camera == nullptr)
		return;

	shader.use();

	glm::mat4 modelMatrix = glm::mat4(1.0f);
	glm::mat4 normalMatrix = glm::mat4(1.0f);
	glm::mat4 projection = camera->GetProjectionMatrix(width, height);
	glm::mat4 view = camera->GetViewMatrix();

	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	shader.setVec3("viewPos", camera->GetPosition());

	for (Model* model : models)
	{
		model->Draw(shader);
	}
}
