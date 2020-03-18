#pragma once

#include <filesystem>

#include "scene.h"
#include "shaderfactory.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"
#include "directLight.h"
#include "camera.h"
#include "moveablecamera.h"
#include "model.h"

namespace fs = std::filesystem;
using namespace glm;

namespace modelPath
{
	const char* nanosuitModelPath = "models/objects/nanosuit/nanosuit.obj";
}

class SceneFactory
{
	
public:
	Scene* SimpleScene()
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
		Model* model = new Model(path.string());

		scene->AddModel(model);

		return scene;
	}
};