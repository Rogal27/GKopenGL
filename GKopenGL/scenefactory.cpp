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
	//working
	const char* floorModelPath = "models/objects/_working/floor/floor.obj";
	const char* boeingKLMModelPath = "models/objects/_working/boeing-klm/source/boeing_klm_cycles/boeing_klm.obj";
	const char* fireTruckModelPath = "models/objects/_working/fire_truck/scene.gltf";
	const char* boeingFijiModelPath = "models/objects/_working/boeing-fiji/source/boeing_airpacific.obj";
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

Scene* SceneFactory::SimpleScene()
{
	//ShaderFactory& shaderFactory = ShaderFactory::getInstance();

	//Shader shader = shaderFactory.getShader(ShaderType::phong);

	//LightenScene* scene = new LightenScene(shader);

	////cameras
	//Camera* camera1 = new Camera(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
	//Camera* camera2 = new MoveableCamera(glm::vec3(4.0f, 2.0f, 0.0f));

	//scene->AddCamera(camera1);
	//scene->AddCamera(camera2);

	////light
	//glm::vec3 lightPos1(1.2f, 1.0f, 2.0f);
	//glm::vec3 lightPos2(1.2f, -1.0f, 2.0f);
	//glm::vec3 lightPos3(0.2f, 1.0f, -2.0f);
	//glm::vec3 lightPos4(-0.2f, -1.0f, -0.3f);

	//Light* light_point1 = new PointLight(lightPos1);
	//light_point1->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
	//Light* light_point2 = new PointLight(lightPos2);
	//light_point2->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
	//Light* light_point3 = new PointLight(lightPos3);
	//light_point3->setColor(glm::vec3(1.0f, 0.0f, 1.0f));
	////SpotLight light_spot(camera2.GetPosition(), camera2.GetFront());
	//Light* light_dir = new DirectLight(lightPos4);
	////light_dir->setColor(glm::vec3(0.0f, 1.0f, 1.0f));

	//scene->AddLight(light_point1, LightType::Point);
	//scene->AddLight(light_point2, LightType::Point);
	//scene->AddLight(light_point3, LightType::Point);
	////scene->AddLight(light_spot, LightType::Point);
	//scene->AddLight(light_dir, LightType::Directional);

	////models
	//fs::path path = fs::canonical(modelPath::nanosuitModelPath);
	////Model* model1 = new Model(path.string());

	//path = fs::canonical(modelPath::alpsModelPath);
	//Model* model2 = new Model(path.string());

	//path = fs::canonical(modelPath::liftModelPath);
	//Model* chairLift = new Model(path.string());



	//glm::mat4 modelMatrix = model2->GetModelMatrix();

	//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.01f, 0.01f, 0.01f));
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(-1085882.0, -3919.0f, 5951819.0f));

	//model2->SetModelMatrix(modelMatrix);

	//modelMatrix = chairLift->GetModelMatrix();

	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 10.0f, 0.0f));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));

	//chairLift->SetModelMatrix(modelMatrix);

	////scene->AddModel(model1);
	//scene->AddModel(model2);
	//scene->AddModel(chairLift);

	//return scene;
	return nullptr;
}

Scene* SceneFactory::SimpleScene2()
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
	light_point1->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	Light* light_point2 = new PointLight(lightPos2);
	light_point2->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	Light* light_point3 = new PointLight(lightPos3);
	light_point3->setColor(glm::vec3(1.0f, 1.0f, 1.0f));
	//SpotLight light_spot(camera2.GetPosition(), camera2.GetFront());
	Light* light_dir = new DirectLight(lightPos4);
	//light_dir->setColor(glm::vec3(0.0f, 1.0f, 1.0f));

	scene->AddLight(light_point1, LightType::Point);
	scene->AddLight(light_point2, LightType::Point);
	scene->AddLight(light_point3, LightType::Point);
	//scene->AddLight(light_spot, LightType::Point);
	scene->AddLight(light_dir, LightType::Directional);

	//models


	//path = fs::canonical(modelPath::boeing474ModelPath);
	//Model* model2 = new Model(path.string());

	//path = fs::canonical(modelPath::boeingKLMModelPath);
	//Model* model3 = new Model(path.string());

	Model* model1 = LoadModel(modelPath::test12ModelPath);
	Model* floor = LoadModel(modelPath::floorModelPath);
	Model* model3 = nullptr;//LoadModel(modelPath::test17ModelPath);

	if (model1)
	{
		//glm::mat4 modelMatrix = model1->GetModelMatrix();
		/*glm::vec3 point = model1->meshes[0].vertices[0].Position;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-point.x - 10.f, -point.y - 10.f, -point.z - 10.f));
		model1->SetModelMatrix(modelMatrix);*/

		scene->AddModel(model1);
	}



	if (model3)
	{
		glm::mat4 modelMatrix = model3->GetModelMatrix();
		glm::vec3 point = model3->meshes[0].vertices[0].Position;
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-point.x + 10.f, -point.y + 10.f, -point.z + 10.f));
		model3->SetModelMatrix(modelMatrix);

		scene->AddModel(model3);
	}

	//Add floor
	if (floor)
	{
		for (int x = -10; x < 10; x++)
		{
			for (int y = -10; y < 10; y++)
			{
				int scale = 5;
				float size = 2.0f;
				Model* floorCopy = new Model(floor);
				glm::mat4 modelMatrix = floorCopy->GetModelMatrix();
				modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
				modelMatrix = glm::translate(modelMatrix, glm::vec3(x * size, 0.0f, y * size));
				floorCopy->SetModelMatrix(modelMatrix);

				scene->AddModel(floorCopy);
			}
		}
	}
	
	return scene;
}