#pragma once

#include <vector>

#include "shader.h"

class Model;
class Camera;
class Light;
enum class LightType;

using namespace std;

class Scene
{
protected:
	Shader shader;
	vector<Camera*> cameras;
	int activeCameraIndex;

public:
	vector<Model*> models;

	Scene(Shader shader);
	virtual ~Scene();
	void AddModel(Model* model);
	void SetShader(Shader shader);
	void AddCamera(Camera* camera);
	bool SetActiveCamera(int index);
	int GetCamerasCount();
	void SwitchCamera();
	Camera* GetActiveCamera();
	virtual void Draw(float width, float height, double time);
};

//class LightOnlyScene : public Scene
//{
//protected:
//	vector<Light> lights;
//public:
//	LightOnlyScene(Shader shader);
//	virtual ~LightOnlyScene() {}
//	void AddLight(Light& light, LightType type);
//	virtual void Draw(float width, float height, double time) override;
//};









