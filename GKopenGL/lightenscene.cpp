#include "lightenscene.h"
#include "scene.h"
#include "light.h"
#include "camera.h"
#include "model.h"
#include "shaderfactory.h"


LightenScene::LightenScene() : Scene(), lightShader(ShaderFactory::getInstance().getShader(ShaderType::light))
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

void LightenScene::Draw(const float& width, const float& height, const double& time)
{
	if (cameras.size() == 0)
		return;

	Camera* camera = GetActiveCamera();
	if (camera == nullptr)
		return;

	Shader shader = GetActiveShader();
	shader.use();

	SetShaderCameraMatrices(camera, shader, width, height);

	SetUpLights();

	for (Model* model : models)
	{
		model->Draw(shader);
	}

	lightShader.use();

	SetShaderCameraMatrices(camera, lightShader, width, height);

	for (Light* l : lights)
	{
		l->Draw(lightShader);
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

void LightenScene::SetUpLights()
{
	Shader shader = GetActiveShader();
	int dirLights = 0, pointLights = 0, spotLights = 0;
	shader.setInt("dirLightsCount", dirLightsCount);
	shader.setInt("pointLightsCount", pointLightsCount);
	shader.setInt("spotLightsCount", spotLightsCount);
	for (Light* l : lights)
	{
		l->setShaderUniforms(shader, dirLights, pointLights, spotLights);
	}
}
