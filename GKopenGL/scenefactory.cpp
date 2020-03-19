#include <glm/gtc/matrix_transform.hpp>
#include <filesystem>

#include "scenefactory.h"
#include "shaderfactory.h"
#include "shader.h"
#include "scene.h"
#include "lightenscene.h"
#include "light.h"
#include "camera.h"
#include "moveablecamera.h"
#include "pointlight.h"
#include "directlight.h"
#include "spotlight.h"
#include "model.h"


namespace modelPath
{
	const char* nanosuitModelPath = "models/objects/nanosuit/nanosuit.obj";
	const char* alpsModelPath = "models/objects/swiss_alps/scene.gltf";
	const char* alps2ModelPath = "models/objects/snowy-mountain/SnowyMountain.obj";
	const char* alps3ModelPath = "models/objects/mont-blanc-france/MontBlanc.glb";
}

namespace fs = std::filesystem;

Scene* SceneFactory::SimpleScene()
{
	ShaderFactory& shaderFactory = ShaderFactory::getInstance();

	Shader shader = shaderFactory.getShader(ShaderType::phong);

	LightenScene* scene = new LightenScene(shader);

	//cameras
	Camera* camera1 = new Camera(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	Camera* camera2 = new MoveableCamera(glm::vec3(4.0f, 2.0f, 0.0f));

	scene->AddCamera(camera1);
	scene->AddCamera(camera2);

	//light
	glm::vec3 lightPos1(1.2f, 1.0f, 2.0f);
	glm::vec3 lightPos2(1.2f, -1.0f, 2.0f);
	glm::vec3 lightPos3(0.2f, 1.0f, -2.0f);
	glm::vec3 lightPos4(-0.2f, -1.0f, -0.3f);

	Light* light_point1 = new PointLight(lightPos1);
	Light* light_point2 = new PointLight(lightPos2);
	Light* light_point3 = new PointLight(lightPos3);
	//SpotLight light_spot(camera2.GetPosition(), camera2.GetFront());
	Light* light_dir = new DirectLight(lightPos4);

	scene->AddLight(light_point1, LightType::Point);
	scene->AddLight(light_point2, LightType::Point);
	scene->AddLight(light_point3, LightType::Point);
	//scene->AddLight(light_spot, LightType::Point);
	scene->AddLight(light_dir, LightType::Directional);

	//models
	fs::path path = fs::canonical(modelPath::nanosuitModelPath);
	//Model* model1 = new Model(path.string());

	path = fs::canonical(modelPath::alpsModelPath);
	Model* model2 = new Model(path.string());



	glm::mat4 modelMatrix = model2->GetModelMatrix();

	modelMatrix = glm::scale(modelMatrix, glm::vec3(0.005f, 0.005f, 0.005f));
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-1085882.0, -3919.0f, 5951819.0f));

	model2->SetModelMatrix(modelMatrix);

	//scene->AddModel(model1);
	scene->AddModel(model2);

	return scene;
}