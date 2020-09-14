#pragma once

#include <vector>

class Shader;
class Model;
class Camera;
class Light;
enum class LightType;

class Scene
{
public:
	std::vector<Model*> models;
	std::vector<Camera*> cameras;

	Scene();
	virtual ~Scene();
	void AddModel(Model* model);
	void AddShader(Shader shader);
	void AddCamera(Camera* camera);
	bool SetActiveShader(int index);
	bool SetActiveCamera(int index);
	int GetShadersCount();
	int GetCamerasCount();
	void SwitchShader();
	void SwitchCamera();
	Shader GetActiveShader();
	Camera* GetActiveCamera();
	virtual void Draw(const float& width, const float& height, const double& time);

protected:
	void SetShaderCameraMatrices(Camera* camera, const Shader& shader, const float& width, const float& height);

	std::vector<Shader> shaders;
	int activeShaderIndex;
	int activeCameraIndex;
};
