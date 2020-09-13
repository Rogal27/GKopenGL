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
#include "mesh.h"


namespace modelPath
{
	//cube
	const char* cubeModelPath = "models/objects/_working/cube/cube.obj";
	//working
	const char* floorModelPath = "models/objects/_working/floor/floor.obj";
	const char* boeingKLMModelPath = "models/objects/_working/boeing-klm/source/boeing_klm_cycles/boeing_klm.obj";
	const char* boeingFijiModelPath = "models/objects/_working/boeing-fiji/source/boeing_airpacific.obj";
	const char* fireTruckModelPath = "models/objects/_working/fire_truck/scene.gltf";
	const char* test11ModelPath = "models/objects/_working/embraer-erj-145/source/erj145.obj";
	const char* test12ModelPath = "models/objects/_working/ilyushin-il-76/source/il76.obj";
}

namespace fs = std::filesystem;

Model* LoadModel(string relativePath)
{
	fs::path path;
	try
	{
		path = fs::canonical(relativePath);
	}
	catch (...)
	{
		cout << "Error in model path: " << relativePath << endl;
		return nullptr;
	}
	return new Model(path.string());
}

Scene* SceneFactory::SimpleScene2()
{
	LightenScene* scene = new LightenScene();

	//shaders
	ShaderFactory& shaderFactory = ShaderFactory::getInstance();
	scene->AddShader(shaderFactory.getShader(ShaderType::phong));
	scene->AddShader(shaderFactory.getShader(ShaderType::gouraud));

	//cameras
	Camera* camera1 = new Camera(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	Camera* camera2 = new MoveableCamera(glm::vec3(0.0f, 0.0f, 0.0f));

	scene->AddCamera(camera1);
	scene->AddCamera(camera2);

	//light
	glm::vec3 lightPos1(8.0f, 1.0f, 5.0f);
	glm::vec3 lightPos2(8.0f, 1.0f, 10.0f);
	glm::vec3 lightPos3(8.0f, 1.0f, 15.0f);
	glm::vec3 lightPos4(-0.2f, -1.0f, -0.3f);

	//Point Lights
	Light* light_point1 = new PointLight(modelPath::cubeModelPath, lightPos1);
	light_point1->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	dynamic_cast<PointLight*>(light_point1)->MoveModelToLight();
	//dynamic_cast<PointLight*>(light_point1)->model.Scale(glm::vec3(0.2f));

	Light* light_point2 = new PointLight(modelPath::cubeModelPath, lightPos2);
	light_point2->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	dynamic_cast<PointLight*>(light_point2)->MoveModelToLight();
	//dynamic_cast<PointLight*>(light_point2)->model.Scale(glm::vec3(0.2f));

	Light* light_point3 = new PointLight(modelPath::cubeModelPath, lightPos3);
	light_point3->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	dynamic_cast<PointLight*>(light_point3)->MoveModelToLight();
	//dynamic_cast<PointLight*>(light_point3)->model.Scale(glm::vec3(0.2f));

	//Spot Lights

	//SpotLight light_spot(camera2.GetPosition(), camera2.GetFront());

	//Directional Lights
	Light* light_dir = new DirectLight(lightPos4);
	//light_dir->setColor(glm::vec3(0.0f, 1.0f, 1.0f));

	scene->AddLight(light_point1, LightType::Point);
	scene->AddLight(light_point2, LightType::Point);
	scene->AddLight(light_point3, LightType::Point);
	//scene->AddLight(light_spot, LightType::Spot);
	scene->AddLight(light_dir, LightType::Directional);

	//models


	//path = fs::canonical(modelPath::boeing474ModelPath);
	//Model* model2 = new Model(path.string());

	//path = fs::canonical(modelPath::boeingKLMModelPath);
	//Model* model3 = new Model(path.string());

	Model* cube001 = LoadModel(modelPath::cubeModelPath);
	Model* cube010 = LoadModel(modelPath::cubeModelPath);
	Model* cube100 = LoadModel(modelPath::cubeModelPath);
	Model* boeingKLM = LoadModel(modelPath::boeingKLMModelPath);
	Model* boeingFiji = LoadModel(modelPath::boeingFijiModelPath);
	Model* fireTruck = LoadModel(modelPath::fireTruckModelPath);
	Model* floor = LoadModel(modelPath::floorModelPath);



	if (cube001)
	{
		cube001->Translate(glm::vec3(10.0f, 0.0f, 5.0f));
		scene->AddModel(cube001);
	}
	if (cube001)
	{
		cube010->Translate(glm::vec3(10.0f, 0.0f, 10.0f));
		scene->AddModel(cube010);
	}
	if (cube001)
	{
		cube100->Translate(glm::vec3(10.0f, 0.0f, 15.0f));
		scene->AddModel(cube100);
	}

	if (boeingKLM)
	{
		boeingKLM->Scale(glm::vec3(0.1f, 0.1f, 0.1f));
		boeingKLM->Translate(boeingKLM->GetFirstPoint() + glm::vec3(10.0f));

		scene->AddModel(boeingKLM);
	}

	if (boeingFiji)
	{
		boeingFiji->Scale(glm::vec3(0.1f, 0.1f, 0.1f));
		boeingFiji->Translate(boeingFiji->GetFirstPoint() + glm::vec3(-10.0f));

		scene->AddModel(boeingFiji);
	}

	if (fireTruck)
	{
		fireTruck->Scale(glm::vec3(0.1f, 0.1f, 0.1f));
		fireTruck->Translate(fireTruck->GetFirstPoint() + glm::vec3(-10.0f));

		scene->AddModel(fireTruck);
	}

	if (floor)
	{
		floor->IsFloor = true;
		scene->AddModel(floor);
	}
	
	return scene;
}
