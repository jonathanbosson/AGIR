#include <iostream>
#include "Utilities.h"
#include "Shader.h"
#include "MatrixStack.hpp"
#include "physicsHandler.h"
#include "Box.h"
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"

#include <time.h>
#include <math.h>

using namespace std;

void changeScene(int scene, vector<Entity*> *objects);
void addSceneBox(vector<Entity*> *list, glm::vec3 pos, float s);
void mouse_button_callback(GLFWwindow*, int button, int action, int mods);
Entity* getSelectedObject(std::vector<Entity*> objectList, glm::vec3 cursorRay_wor, glm::mat4 viewMatrix);

const float BAD_PI = 3.141592f;
int currentScene;

// Mouse
const int MOUSE_BUTTON_RELEASED = 0;
const int MOUSE_BUTTON_LEFT = 1;
const int MOUSE_BUTTON_RIGHT = 2;
int mouseState = MOUSE_BUTTON_RELEASED;

void setupViewport(GLFWwindow *window, GLfloat *P)
{
    int width, height;
    
    glfwGetWindowSize(window, &width, &height);
    
    P[0] = P[5] * height / width;
    
    glViewport(0, 0, width, height);
}

int main()
{
    // GL-related variables
    GLfloat P[16] = { 2.42f, 0.0f, 0.0f, 0.0f
        , 0.0f, 2.42f, 0.0f, 0.0f
        , 0.0f, 0.0f, -1.0f, -1.0f
        , 0.0f, 0.0f, -0.2f, 0.0f };
    GLfloat L[3] = { 0.0f, 2.0f, 4.0f };
    GLfloat Ca[3] = { 0.0f, 0.0f, 0.0f };
    glm::vec4 li(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 cam(0.0f, 0.0f, 0.0f, 1.0f);
    GLfloat C[3];
    
    GLint locationMV;
    GLint locationP;
    GLint locationL;
    GLint locationCa;
    GLint locationColor;
    
    glm::mat4 projectionMatrix = { 2.42f, 0.0f, 0.0f, 0.0f
        , 0.0f, 2.42f, 0.0f, 0.0f
        , 0.0f, 0.0f, -1.0f, -1.0f
        , 0.0f, 0.0f, -0.2f, 0.0f };
    
    // start GLEW extension handler
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    //create GLFW window and select context
    GLFWwindow* window = glfwCreateWindow(640, 480, "Do not try and bend the spoon. That's impossible. Instead... only try to realize the truth.", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    
    //start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    printf("Renderer: %s\n", renderer);
    printf("OpenGL version supported %s\n", version);
    
    //Create objects
    std::vector<Entity*> objectList;
    
    Shader phongShader;
    phongShader.createShader("vertexshader.glsl", "fragmentshader.glsl");
    MatrixStack MVstack;
    MVstack.init();
    
    Camera theCamera(15.0f);
    
    physicsHandler theHandler;

	//Sphere mouse(glm::vec3(0.0, 0.0f, 0.0f), 65.45f, 0.1f);
    
    
    //link variables to shader
    locationMV = glGetUniformLocation(phongShader.programID, "MV");
    locationP = glGetUniformLocation(phongShader.programID, "P");
    locationL = glGetUniformLocation(phongShader.programID, "lightPosition");
    locationCa = glGetUniformLocation(phongShader.programID, "cameraPosition");
    locationColor = glGetUniformLocation(phongShader.programID, "objectColor");
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glm::mat4 viewMatrix;
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        li = glm::vec4(0.0, 5.0, 0.0, 1.0);
        cam = glm::vec4(0.0, 0.0, 0.0, 1.0);
       
        //GL calls
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glUseProgram(phongShader.programID);
        
        //Send static variables to vertexshader
        glUniformMatrix4fv(locationP, 1, GL_FALSE, P);
        
        setupViewport(window, P);
        
        //Transform calculations and rendering
        MVstack.push();
        MVstack.translate(glm::vec3(0.0f, -3.0f, -10.0f));
        MVstack.translate(glm::vec3(0.0f, 0.0f, -theCamera.getRad()));
        MVstack.rotX(theCamera.getTheta());
        MVstack.rotY(theCamera.getPhi());
        
        viewMatrix = glm::make_mat4(MVstack.getCurrentMatrix());
        li = glm::inverse(viewMatrix)*li;
        cam = glm::inverse(viewMatrix)*cam;
        
        Ca[0] = cam.x;
        Ca[1] = cam.y;
        Ca[2] = cam.z;
        L[0] = li.x;
        L[1] = li.y;
        L[2] = li.z;
        glUniform3fv(locationL, 1, L);
        glUniform3fv(locationCa, 1, Ca);
        
        
        
        MVstack.pop();
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return 0;
}