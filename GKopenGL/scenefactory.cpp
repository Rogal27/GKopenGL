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
	const char* sphere2ModelPath = "models/objects/_working/sphere2/sphere.obj";
	const char* sphere3ModelPath = "models/objects/_working/sphere3/sphere.obj";
	const char* sphere4ModelPath = "models/objects/_working/sphere3/sphere_detailed.obj";
	const char* grayCubeModelPath = "models/objects/_working/wall/wall.obj";
	//working
	const char* floorModelPath = "models/objects/_working/floor/floor.obj";
	const char* boeingKLMModelPath = "models/objects/_working/boeing-klm/source/boeing_klm_cycles/boeing_klm.obj";
	const char* boeingFijiModelPath = "models/objects/_working/boeing-fiji/source/boeing_airpacific.obj";
	const char* fireTruckModelPath = "models/objects/_working/fire_truck/scene.gltf";
	const char* smallJetModelPath = "models/objects/_working/embraer-erj-145/source/erj145.obj";
	const char* cargoPlaneModelPath = "models/objects/_working/ilyushin-il-76/source/il76.obj";
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

LightenScene* SceneFactory::MainLightenScene()
{
	LightenScene* scene = new LightenScene();

	//shaders
	ShaderFactory& shaderFactory = ShaderFactory::getInstance();
	scene->AddShader(shaderFactory.getShader(ShaderType::phong));
	scene->AddShader(shaderFactory.getShader(ShaderType::gouraud));

	//cameras
	Camera* camera1 = new Camera(glm::vec3(80.0f, 20.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Camera* camera2 = new MoveableCamera(glm::vec3(0.0f, 2.0f, 0.0f));
	Camera* camera3 = new Camera(glm::vec3(10.0f, 20.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Camera* camera4 = new Camera(glm::vec3(10.0f, 20.0f, 50.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	Camera* camera5 = new MoveableCamera(glm::vec3(15.0f, 2.0f, -17.0f));

	scene->AddCamera(camera1);
	scene->AddCamera(camera2);
	scene->AddCamera(camera3);
	scene->AddCamera(camera4);
	scene->AddCamera(camera5);

	//Point And Spot Lights pointers
	PointLight* pointLight;
	SpotLight* spotLight;

	//FIRST LIGHT --------------------------------------------------------------------------------------------------------
	//Directional Lights | offset = 0
	Light* light_dir = new DirectLight(glm::vec3(-0.2f, -1.0f, -0.3f));
	scene->AddLight(light_dir, LightType::Directional);

	//runway | offset = 1
	const int pointLights_size = 16;
	Light* pointLights[pointLights_size];
	for (int i = 0; i < pointLights_size; i+=2)
	{
		pointLights[i] = new PointLight(modelPath::cubeModelPath);
		pointLights[i]->setColor(glm::vec3(1.0f, 0.0f, 0.3f));

		pointLight = dynamic_cast<PointLight*>(pointLights[i]);
		pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(-20.0f, 0.15f, -80.0f + i * 10.0f)));
		pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.3f)));

		pointLights[i + 1] = new PointLight(modelPath::cubeModelPath);
		pointLights[i + 1]->setColor(glm::vec3(1.0f, 0.0f, 0.3f));

		pointLight = dynamic_cast<PointLight*>(pointLights[i + 1]);
		pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(-45.0f, 0.15f, -80.0f + i * 10.0f)));
		pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.3f)));

		scene->AddLight(pointLights[i], LightType::Point);
		scene->AddLight(pointLights[i + 1], LightType::Point);
	}

	//firetruck siren lights - truck 1 | offset = 17
	Light* truckPointLight1 = new PointLight(modelPath::cubeModelPath);
	truckPointLight1->setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	pointLight = dynamic_cast<PointLight*>(truckPointLight1);
	pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(16.72f, 3.35f, 14.53f)));
	pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	Light* truckPointLight2 = new PointLight(modelPath::cubeModelPath);
	truckPointLight2->setColor(glm::vec3(0.0f, 0.0f, 1.0f));

	pointLight = dynamic_cast<PointLight*>(truckPointLight2);
	pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(16.72f, 3.35f, 16.35f)));
	pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	Light* truckPointLight3 = new PointLight(modelPath::cubeModelPath);
	truckPointLight3->setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	pointLight = dynamic_cast<PointLight*>(truckPointLight3);
	pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(26.2f, 3.12f, 14.53f)));
	pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	Light* truckPointLight4 = new PointLight(modelPath::cubeModelPath);
	truckPointLight4->setColor(glm::vec3(0.0f, 0.0f, 1.0f));

	pointLight = dynamic_cast<PointLight*>(truckPointLight4);
	pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(26.2f, 3.12f, 16.35f)));
	pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	scene->AddLight(truckPointLight1, LightType::Point);
	scene->AddLight(truckPointLight2, LightType::Point);
	scene->AddLight(truckPointLight3, LightType::Point);
	scene->AddLight(truckPointLight4, LightType::Point);

	//firetruck with sirens -  reflectors- for truck1 | offset = 21
	SpotLight* truckSpotLight1 = new SpotLight(modelPath::cubeModelPath, glm::normalize(glm::vec3(-1.0f, -0.1f, 0.0f)));
	truckSpotLight1->setColor(glm::vec3(1.0f, 0.89f, 0.6f));

	spotLight = dynamic_cast<SpotLight*>(truckSpotLight1);
	spotLight->model.SetPositionMatrix(glm::translate(spotLight->model.GetPositionMatrix(), glm::vec3(16.12f, 1.5f, 14.53f)));
	spotLight->model.SetPositionMatrix(glm::scale(spotLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	SpotLight* truckSpotLight2 = new SpotLight(modelPath::cubeModelPath, glm::normalize(glm::vec3(-1.0f, -0.1f, 0.0f)));
	truckSpotLight2->setColor(glm::vec3(1.0f, 0.89f, 0.6f));

	spotLight = dynamic_cast<SpotLight*>(truckSpotLight2);
	spotLight->model.SetPositionMatrix(glm::translate(spotLight->model.GetPositionMatrix(), glm::vec3(16.12f, 1.5f, 16.35f)));
	spotLight->model.SetPositionMatrix(glm::scale(spotLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	scene->AddLight(truckSpotLight1, LightType::SpotLight);
	scene->AddLight(truckSpotLight2, LightType::SpotLight);

	//firetruck  -  reflectors - for truck2 | offset = 23
	SpotLight* truckSpotLight3 = new SpotLight(modelPath::cubeModelPath, glm::normalize(glm::vec3(0.0f, -0.1f, -1.0f)), 30.0f, 40.0f, 1.0f, 0.01f, 0.001f);
	truckSpotLight3->setColor(glm::vec3(1.0f, 0.89f, 0.6f));

	spotLight = dynamic_cast<SpotLight*>(truckSpotLight3);
	spotLight->model.SetPositionMatrix(glm::translate(spotLight->model.GetPositionMatrix(), glm::vec3(-3.87f, 1.5f, 14.52f)));
	spotLight->model.SetPositionMatrix(glm::scale(spotLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	SpotLight* truckSpotLight4 = new SpotLight(modelPath::cubeModelPath, glm::normalize(glm::vec3(0.0f, -0.1f, -1.0f)), 30.0f, 40.0f, 1.0f, 0.01f, 0.001f);
	truckSpotLight4->setColor(glm::vec3(1.0f, 0.89f, 0.6f));

	spotLight = dynamic_cast<SpotLight*>(truckSpotLight4);
	spotLight->model.SetPositionMatrix(glm::translate(spotLight->model.GetPositionMatrix(), glm::vec3(-2.05f, 1.5f, 14.52f)));
	spotLight->model.SetPositionMatrix(glm::scale(spotLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	scene->AddLight(truckSpotLight3, LightType::SpotLight);
	scene->AddLight(truckSpotLight4, LightType::SpotLight);

	//firetruck  -  reflectors - for truck3 | offset = 25
	SpotLight* truckSpotLight5 = new SpotLight(modelPath::cubeModelPath, glm::normalize(glm::vec3(0.0f, -0.1f, -1.0f)));
	truckSpotLight5->setColor(glm::vec3(1.0f, 0.89f, 0.6f));

	spotLight = dynamic_cast<SpotLight*>(truckSpotLight5);
	spotLight->model.SetPositionMatrix(glm::translate(spotLight->model.GetPositionMatrix(), glm::vec3(-11.87f, 1.5f, 14.52f)));
	spotLight->model.SetPositionMatrix(glm::scale(spotLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	SpotLight* truckSpotLight6 = new SpotLight(modelPath::cubeModelPath, glm::normalize(glm::vec3(0.0f, -0.1f, -1.0f)));
	truckSpotLight6->setColor(glm::vec3(1.0f, 0.89f, 0.6f));

	spotLight = dynamic_cast<SpotLight*>(truckSpotLight6);
	spotLight->model.SetPositionMatrix(glm::translate(spotLight->model.GetPositionMatrix(), glm::vec3(-10.05f, 1.5f, 14.52f)));
	spotLight->model.SetPositionMatrix(glm::scale(spotLight->model.GetPositionMatrix(), glm::vec3(0.1f)));

	scene->AddLight(truckSpotLight5, LightType::SpotLight);
	scene->AddLight(truckSpotLight6, LightType::SpotLight);

	//Camera spot Lights offset = 0 | offset = 27
	SpotLight* cameraLightSpot = new SpotLight(modelPath::cubeModelPath, camera5->GetFront(), 12.5f, 20.0f);
	spotLight = dynamic_cast<SpotLight*>(cameraLightSpot);
	spotLight->model.SetPositionMatrix(glm::translate(spotLight->model.GetPositionMatrix(), glm::vec3(camera5->GetPosition())));
	spotLight->model.SetPositionMatrix(glm::scale(spotLight->model.GetPositionMatrix(), glm::vec3(0.04f)));
	//spotLight->setPosition(camera5->GetPosition());
	scene->AddLight(cameraLightSpot, LightType::SpotLight);
	
	//Three lights | offset = 28
	Light* light_point1 = new PointLight(modelPath::cubeModelPath);
	light_point1->setColor(glm::vec3(1.0f, 0.0f, 0.0f));

	pointLight = dynamic_cast<PointLight*>(light_point1);
	pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(8.0f, 1.0f, 5.0f)));
	pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.2f)));

	Light* light_point2 = new PointLight(modelPath::cubeModelPath);
	light_point2->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	pointLight = dynamic_cast<PointLight*>(light_point2);
	pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(8.0f, 1.0f, 10.0f)));
	pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.2f)));

	Light* light_point3 = new PointLight(modelPath::cubeModelPath);
	light_point3->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
	pointLight = dynamic_cast<PointLight*>(light_point3);
	pointLight->model.SetPositionMatrix(glm::translate(pointLight->model.GetPositionMatrix(), glm::vec3(8.0f, 1.0f, 15.0f)));
	pointLight->model.SetPositionMatrix(glm::scale(pointLight->model.GetPositionMatrix(), glm::vec3(0.2f)));

	scene->AddLight(light_point1, LightType::Point);
	scene->AddLight(light_point2, LightType::Point);
	scene->AddLight(light_point3, LightType::Point);
	

	//models
	Model* sphere1 = LoadModel(modelPath::sphere2ModelPath);
	Model* sphere2 = LoadModel(modelPath::sphere2ModelPath);
	Model* sphere3 = LoadModel(modelPath::sphere4ModelPath);
	Model* sphere4 = LoadModel(modelPath::sphere4ModelPath);
	Model* sphere5 = LoadModel(modelPath::sphere4ModelPath);
	Model* wall = LoadModel(modelPath::grayCubeModelPath);
	Model* boeingKLM = LoadModel(modelPath::boeingKLMModelPath);
	Model* boeingFiji = LoadModel(modelPath::boeingFijiModelPath);
	Model* fireTruck1 = LoadModel(modelPath::fireTruckModelPath);
	Model* fireTruck2 = LoadModel(modelPath::fireTruckModelPath);
	Model* fireTruck3 = LoadModel(modelPath::fireTruckModelPath);
	Model* smallJet = LoadModel(modelPath::smallJetModelPath);
	Model* cargoPlane = LoadModel(modelPath::cargoPlaneModelPath);

	Model* floor = LoadModel(modelPath::floorModelPath);

	if (fireTruck2)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(-2.0f, -17.0f, 0.0f));
		matrix = glm::translate(matrix, fireTruck2->GetFirstPoint());

		matrix = glm::rotate(matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		matrix = glm::scale(matrix, glm::vec3(0.15f));

		fireTruck2->SetPositionMatrix(matrix);

		scene->AddModel(fireTruck2);
	}

	if (fireTruck3)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(6.0f, -17.0f, 0.0f));
		matrix = glm::translate(matrix, fireTruck3->GetFirstPoint());

		matrix = glm::rotate(matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		matrix = glm::scale(matrix, glm::vec3(0.15f));

		fireTruck3->SetPositionMatrix(matrix);

		scene->AddModel(fireTruck3);
	}

	if (sphere1)
	{
		sphere1->Translate(glm::vec3(8.0f, 2.0f, -10.0f));
		scene->AddModel(sphere1);
	}

	if (sphere2)
	{
		sphere2->Translate(glm::vec3(4.5f, 2.0f, -24.0f));
		scene->AddModel(sphere2);
	}

	if (sphere3)
	{
		sphere3->Translate(glm::vec3(6.5f, 2.0f, -17.0f));
		sphere3->Scale(glm::vec3(1.0f));
		scene->AddModel(sphere3);
	}

	if (sphere4) // by plane
	{
		sphere4->Translate(glm::vec3(9.5f, 2.0f, 21.5f));
		sphere4->Scale(glm::vec3(1.0f));
		scene->AddModel(sphere4);
	}

	if (sphere5) //by truck 3
	{
		sphere5->Translate(glm::vec3(-10.0f, 1.5f, 5.0f));
		sphere5->Scale(glm::vec3(0.8f));
		scene->AddModel(sphere5);
	}

	if (wall)
	{
		wall->Translate(glm::vec3(5.0f, -0.5f, 13.0f));
		wall->Scale(glm::vec3(0.4f, 4.0f, 10.0f));
		scene->AddModel(wall);
	}

	if (boeingKLM)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(-32.0f, 2.0f, 20.0f));
		matrix = glm::translate(matrix, boeingKLM->GetFirstPoint());

		matrix = glm::rotate(matrix, glm::radians(-6.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		matrix = glm::scale(matrix, glm::vec3(0.7f));
		
		boeingKLM->SetPositionMatrix(matrix);

		scene->AddModel(boeingKLM);
	}

	if (boeingFiji)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(-32.0f, 20.0f, -70.0f));
		matrix = glm::translate(matrix, boeingFiji->GetFirstPoint());

		matrix = glm::scale(matrix, glm::vec3(0.7f));

		boeingFiji->SetPositionMatrix(matrix);

		scene->AddModel(boeingFiji);
	}

	//with sirens - beetwen airplanes
	if (fireTruck1)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(28.0f, -17.0f, -2.0f));
		matrix = glm::translate(matrix, fireTruck1->GetFirstPoint());

		matrix = glm::rotate(matrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		matrix = glm::scale(matrix, glm::vec3(0.15f));

		fireTruck1->SetPositionMatrix(matrix);

		scene->AddModel(fireTruck1);
	}

	if (smallJet)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(28.0f, 5.0f, 0.0f));
		matrix = glm::translate(matrix, smallJet->GetFirstPoint());

		matrix = glm::rotate(matrix, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		matrix = glm::scale(matrix, glm::vec3(1.0f));

		smallJet->SetPositionMatrix(matrix);

		scene->AddModel(smallJet);
	}

	if (cargoPlane)
	{
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::translate(matrix, glm::vec3(35.0f, 8.7f, 20.0f));
		matrix = glm::translate(matrix, cargoPlane->GetFirstPoint());

		matrix = glm::scale(matrix, glm::vec3(1.f));

		cargoPlane->SetPositionMatrix(matrix);

		scene->AddModel(cargoPlane);
	}

	if (floor)
	{
		floor->IsFloor = true;
		scene->AddModel(floor);
	}
	
	return scene;
}
