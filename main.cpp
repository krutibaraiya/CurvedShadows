// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "Shader.h"
#include "Camera.h"
#include "ObjModel.h"
#include "Light.h"

Camera camera({10, 10, -5}, {0, 0, 0});
Light whitelight =  {{10,  15, 15},  {1, 1, 1}};

//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
bool isDragging = false;
double dragX = -1, dragY = -1;

//implementing key callbacks left

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::FORWARD);
    } else if (key == GLFW_KEY_S and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::BACKWARD);
    } else if (key == GLFW_KEY_A and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::LEFT);
    } else if (key == GLFW_KEY_D and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::RIGHT);
    } else if (key == GLFW_KEY_UP and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::UP);
    } else if (key == GLFW_KEY_DOWN and action == GLFW_PRESS) {
        camera.translate(Camera::Direction::DOWN);
    } else if (key == GLFW_KEY_R and action == GLFW_PRESS) {
        camera.reset();
    } else if (key == GLFW_KEY_Q and action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        isDragging = action == GLFW_PRESS;
        if (not isDragging)
            dragX = dragY = -1;
    }
}

void glfw_cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
    if (isDragging) {
        if (dragX != -1 and dragY != -1)
            camera.rotate(xPos - dragX, yPos - dragY);
        dragX = xPos;
        dragY = yPos;
    }
}

void glfw_scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.zoom(yOffset);
}


int main( void )
{
    float aspect = (float) 800 / (float) 600;
    const unsigned int SHADOW_RES = 1024;
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 800, 600, "CurvedShadows", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    Shader shader("Shaders/vert_shader.glsl", "Shaders/frag_shader.glsl");
    Shader lightShader("Shaders/light_vert_shader.glsl", "Shaders/light_frag_shader.glsl");
    Shader depthShader("Shaders/depth_vert.glsl", "Shaders/depth_frag.glsl");

    auto room = ObjModel("models/room.obj");
    auto donut = ObjModel("models/donut.obj");
    auto cube = ObjModel("models/cube.obj");
    auto curvedobjects =ObjModel("model/curvedobjects.obj");


    unsigned int depthMapFBO = 0;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glDrawBuffer(GL_NONE); // don't need color buf for shadows
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    whitelight.genDepthMap(SHADOW_RES);


 //   shader.set("numLights", (int) lights.size());

    while (!glfwWindowShouldClose(window))
    {
        auto model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(-0.4f,0.2f,0.0f));
        //model = glm::scale(model, glm::vec3(40)); // a smaller cube
        float near_plane = 1.f, far_plane = 35.f;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        //first pass//render with respect to the light
            auto lightProj = glm::perspective(30.f, aspect, near_plane, far_plane);
            auto lightView = whitelight.lookAt();
            whitelight.proj_view = lightProj * lightView;
            depthShader.use();
            depthShader.set("model", model);
            depthShader.set("lightSpace", whitelight.proj_view);

            // attach depth map texture to framebuffer and render the scene from light's PoV
            glViewport(0, 0, SHADOW_RES, SHADOW_RES);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, whitelight.getDepthMap(), 0);
            glClear(GL_DEPTH_BUFFER_BIT); // only drawing depth map
            //room.draw();
            curvedobjects.draw();
            //donut.draw();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto proj = glm::perspective(45.f * camera.getZoom(), aspect, near_plane, far_plane);
        auto view = camera.lookAt();
        shader.set("projection", proj);
        shader.set("view", view);
        shader.set("model", model);
        lightShader.set("projection", proj);
        lightShader.set("view", view);
        lightShader.use();

            shader.set("whitelight.lightPos", whitelight.getPosition());
            shader.set("whitelight.lightColor", whitelight.getColor());
            shader.set("whitelight.lightSpace", whitelight.proj_view);
            shader.set("whitelight.shadowMap", 0);  // should be the index used in glActiveTexture, not the texture ID

            // draw light using a cube
            lightShader.set("model", whitelight.getModel());
            lightShader.set("lightColor", whitelight.getColor());
            cube.draw();

            glActiveTexture(GL_TEXTURE0 + 0);
            glBindTexture(GL_TEXTURE_2D, whitelight.getDepthMap());

        shader.use();
        curvedobjects.draw();
        //room.draw();
        //donut.draw();

        lightShader.use();
        lightShader.set("projection", proj);
        lightShader.set("view", view);
        lightShader.set("model", whitelight.getModel());
        lightShader.set("lightColor", whitelight.getColor());
        cube.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
