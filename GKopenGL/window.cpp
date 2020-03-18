#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <filesystem>
#include <iostream>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#include "shader.h"
#include "shaderfactory.h"
#include "camera.h"
#include "moveablecamera.h"
#include "light.h"
#include "directLight.h"
#include "pointLight.h"
#include "spotLight.h"
#include "model.h"
#include "scene.h"
#include "scenefactory.h"

namespace fs = std::filesystem;

//window size
int WIDTH = 1280;
int HEIGHT = 720;

constexpr double FPS = 1.0 / 60.0;

//shaders Path


//models path

Scene* mainScene = nullptr;

//is some key pressed
bool isPPressed = false;

//camera
//Camera camera(glm::vec3(3.0f, 0.0f, 3.0f), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
//MoveableCamera camera(glm::vec3(4.0f, 2.0f, 0.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

//light
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightPos2(1.2f, -1.0f, 2.0f);
glm::vec3 lightPos3(0.2f, 1.0f, -2.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);

//time
double deltaTime = 0.0f;	// Time between current frame and last frame
double lastFrame = 0.0f; // Time of last frame

//shader type
//0-phong, 1-gouraud
int shader_type = 0;

//prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  
    
    glEnable(GL_DEPTH_TEST);

    //shader Program
    //Shader shaders[2] = { Shader(shaderPath::phongVertexShaderPath, shaderPath::phongFragmentShaderPath) , Shader(shaderPath::modelVertexShaderPath, shaderPath::modelFragmentShaderPath) };
    //Shader phongShader(phongVertexShaderPath, phongFragmentShaderPath);
    //Shader gouraudShader(gouraudVertexShaderPath, gouraudFragmentShaderPath);
    Shader lampShader(shaderPath::lightVertexShaderPath, shaderPath::lightFragmentShaderPath);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    double previousTime = glfwGetTime();
    int frameCount = 0;
    std::string s = "";


    //light
    PointLight light_point(lightPos);
    PointLight light_point2(lightPos2);
    PointLight light_point3(lightPos3);
    //SpotLight light_spot(camera.GetPosition(), camera.GetFront());
    DirectLight light_dir(vec3(-0.2f, -1.0f, -0.3f));

    // load models
    // -----------
    //std::filesystem
    //fs::path path = fs::canonical(modelPath::nanosuitModelPath);
    //Model ourModel(path.string());

    SceneFactory sf = SceneFactory();

    //scene
    mainScene = sf.SimpleScene();
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {        
        //per-frame time logic
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        frameCount++;
        if (currentFrame - previousTime >= 1.0)
        {
            s.append("LearnOpenGL [FPS:");
            s.append(std::to_string((int)frameCount));
            s.append("]");
            glfwSetWindowTitle(window, s.c_str());
            s.clear(); 
            frameCount = 0;
            previousTime = currentFrame;
        }
        


        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //move light
       /* lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
        lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
        light_point.setPosition(lightPos);

        float r = sin(glfwGetTime());
        float g = cos(glfwGetTime());

        light_point2.setColor(r * r, g * g, r * g + 0.5f);
        light_point3.setColor(r * r, r * g + 0.5f, g*g);


        light_spot.setPosition(camera.GetPosition());
        light_spot.setDirection(camera.GetFront());*/

        // be sure to activate shader when setting uniforms/drawing objects
        //shaders[shader_type].use();
        

      

        // view/projection transformations
        /*glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 projection = camera.GetProjectionMatrix((float)WIDTH, (float)HEIGHT);
        glm::mat4 view = camera.GetViewMatrix();*/
        

        // world transformations

        //glm::mat3 NormalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

        //vertex shader uniforms
        //shaders[shader_type].setMat4("model", model);
        //shaders[shader_type].setMat4("view", view);
        //shaders[shader_type].setMat4("projection", projection);
        //shaders[shader_type].setMat3("NormalMatrix", NormalMatrix);
        
        //shaders[shader_type].setVec3("objectColor", 1.0f, 0.5f, 0.31f);
        //shaders[shader_type].setVec3("viewPos", camera.GetPosition());

        //lights
        //int dirLights = 0, pointLights = 0, spotLights = 0;
        //shaders[shader_type].setInt("dirLightsCount", 1);
        //shaders[shader_type].setInt("pointLightsCount", 3);
        //shaders[shader_type].setInt("spotLightsCount", 1);
        
        //light_point.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);
        //light_point2.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);
        //light_point3.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);
        //light_spot.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);
        //light_dir.setShaderUniforms(shaders[shader_type], dirLights, pointLights, spotLights);


        // render the cube

        //model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

        //shaders[shader_type].setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //model = glm::mat4(1.0f);
        ////model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down

        //shaders[shader_type].setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);


        // render the loaded model
        
        //shaders[shader_type].setMat4("view", view);
        //shaders[shader_type].setMat4("projection", projection);

        //for (size_t i = 0; i < 10; i++)
        //{
        //    for (size_t j = 0; j < 10; j++)
        //    {
        //        model = glm::mat4(1.0f);
        //        model = glm::translate(model, glm::vec3(i, -1.75f, j)); // translate it down so it's at the center of the scene
        //        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
        //        float angle = glfwGetTime() * 25.0f * i + j;
        //        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //        NormalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));

        //        shaders[shader_type].setMat4("model", model);

        //        shaders[shader_type].setMat3("NormalMatrix", NormalMatrix);
        //        ourModel.Draw(shaders[shader_type]);
        //    }
        //    
        //}        

        //model
        glm::mat4 model = glm::mat4(1.0f);

        mainScene->Draw(WIDTH, HEIGHT, 0.0);

        Camera* camera = mainScene->GetActiveCamera();
        if (camera == nullptr)
            continue;

        glm::mat4 projection = camera->GetProjectionMatrix(WIDTH, HEIGHT);
        glm::mat4 view = camera->GetViewMatrix();

        // also draw the lamp object
        lampShader.use();
        lampShader.setVec3("lightColor", light_point.getColor());
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_point.getPosition());
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lampShader.use();
        lampShader.setVec3("lightColor", light_point2.getColor());
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_point2.getPosition());
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        lampShader.use();
        lampShader.setVec3("lightColor", light_point3.getColor());
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, light_point3.getPosition());
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        double endFrame = glfwGetTime();
        while (endFrame - currentFrame < FPS)
        {
            endFrame = glfwGetTime();
        }
    }
    delete mainScene;

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();

    return 0;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    WIDTH = width;
    HEIGHT = height;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (mainScene == nullptr)
        return;
    Camera* camera = mainScene->GetActiveCamera();
    if (camera == nullptr)
        return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::DOWN, deltaTime);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        if (isPPressed == false)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        isPPressed = !isPPressed;
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS)
    {
        shader_type++; 
        shader_type %= 2;

        if (shader_type == 0)
        {
            cout << "Phong shading" << endl;
        }
        else if (shader_type == 1)
        {
            cout << "Gouraud shading" << endl;
        }
    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if (mainScene == nullptr)
            return;
        mainScene->SwitchCamera();
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    if (mainScene == nullptr)
        return;
    Camera* camera = mainScene->GetActiveCamera();
    if (camera == nullptr)
        return;

    camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (mainScene == nullptr)
        return;
    Camera* camera = mainScene->GetActiveCamera();
    if (camera == nullptr)
        return;
    
    camera->ProcessMouseScroll(yoffset);
}