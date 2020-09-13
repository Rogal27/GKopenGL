#include "lightenscene.h"
#include "scene.h"
#include "shader.h"
#include "light.h"
#include "camera.h"
#include "model.h"


LightenScene::LightenScene(Shader shader) : Scene(shader)
{
	dirLightsCount = 0;
	pointLightsCount = 0;
	spotLightsCount = 0;
}

LightenScene::~LightenScene()
{
	for (Light* light : lights)
	{
		delete light;
	}
}

void LightenScene::SetUpLights()
{
	int dirLights = 0, pointLights = 0, spotLights = 0;
	shader.setInt("dirLightsCount", dirLightsCount);
	shader.setInt("pointLightsCount", pointLightsCount);
	shader.setInt("spotLightsCount", spotLightsCount);
	for (Light* l : lights)
	{
		l->setShaderUniforms(shader, dirLights, pointLights, spotLights);
	}
}

void LightenScene::AddLight(Light* light, LightType type)
{
	lights.push_back(light);
	switch (type)
	{
	case LightType::Directional:
		dirLightsCount++;
		break;
	case LightType::Point:
		pointLightsCount++;
		break;
	case LightType::SpotLight:
		spotLightsCount++;
		break;
	}
}

void LightenScene::Draw(float width, float height, double time)
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

	SetUpLights();

	for (Model* model : models)
	{
		model->Draw(shader);
	}
}
