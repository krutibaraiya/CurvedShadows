/// Include standard headers
#include <iostream>

/// Include GLEW
#include <GL/glew.h>

/// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

/// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "Shader.h"
#include "Camera.h"
#include "ObjModel.h"
#include "Light.h"

///Initialising Camera position and target
Camera camera({10, 10, -5}, {0, 0, 0});
Light whitelight =  {{10,  15, 10},  {1, 0.6, 0.6}};


bool isDragging = false;
double dragX = -1, dragY = -1;


/**
 * @brief Method to define the keyboard keys for camera movement
 */
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
/**
 * @brief Method to define the mouse movement for camera movement
 * @param window GLFW window
 * @param button Mouse click
 * @param action action of mouse button
 */
void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        isDragging = action == GLFW_PRESS;
        if (not isDragging)
            dragX = dragY = -1;
    }
}
/**
 * @brief Method to drag the scene for cursor movement
 * @param window GLFW window
 * @param xPos x coordinate of the cursor
 * @param yPos y coordinate of the cursor
 */
void glfw_cursor_position_callback(GLFWwindow* window, double xPos, double yPos) {
    if (isDragging) {
        if (dragX != -1 and dragY != -1)
            camera.rotate(xPos - dragX, yPos - dragY);
        dragX = xPos;
        dragY = yPos;
    }
}
/**
 * @brief method to define the scroll movement
 * @param window GLFW window
 * @param xOffset x coordinate of the scroll
 * @param yOffset y coordinate of the scroll
 */
void glfw_scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    camera.zoom(yOffset);
}


int main( void )
{
    float aspect = (float) 800 / (float) 600;
    const unsigned int SHADOW_RES = 1024;
    /// Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /// Open a window and create its OpenGL context
    window = glfwCreateWindow( 800, 600, "CurvedShadows", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    /// Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);
    glfwSetCursorPosCallback(window, glfw_cursor_position_callback);
    glfwSetScrollCallback(window, glfw_scroll_callback);

    glEnable(GL_DEPTH_TEST); /// enable depth-testing
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);

    /// Shader objects
    Shader shader("Shaders/vert_shader.glsl", "Shaders/frag_shader.glsl");
    Shader lightShader("Shaders/light_vert_shader.glsl", "Shaders/light_frag_shader.glsl");
    Shader depthShader("Shaders/depth_vert.glsl", "Shaders/depth_frag.glsl");

    /// Light source
    auto cube = ObjModel("models/cube.obj");
    /// Objects in the scene
    auto objects =ObjModel("models/objects.obj");


    unsigned int depthMapFBO = 0;
    glGenFramebuffers(1, &depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glDrawBuffer(GL_NONE); 
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    whitelight.genDepthMap(SHADOW_RES);


    while (!glfwWindowShouldClose(window))
    {
        auto model = glm::mat4(1.0f);
        float near_plane = 1.f, far_plane = 35.f;
        model = glm::rotate(model, ((float)glfwGetTime() * glm::radians(50.0f))/2, glm::vec3(0.0f, 1.0f, 0.0f));
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        /// 1st pass
        /// Generate the depth maps for the light source
        auto lightProj = glm::perspective(30.f, aspect, near_plane, far_plane);
        auto lightView = whitelight.lookAt();
        whitelight.proj_view = lightProj * lightView;
        depthShader.use();
        depthShader.set("model", model);
        depthShader.set("lightSpace", whitelight.proj_view);

        glViewport(0, 0, SHADOW_RES, SHADOW_RES);

        /// Binding the depth buffer to frame buffer and rendering the scene with respect to light source
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, whitelight.getDepthMap(), 0);
        glClear(GL_DEPTH_BUFFER_BIT); 

        /// rendering the objects
        objects.draw();

        /// unbinding the frame buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        /// 2nd pass
        /// Rendering the scene with shadow with respect to the viewer
        glViewport(0, 0, 800, 600);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// defining the perspective for viewer
        auto proj = glm::perspective(45.f * camera.getZoom(), aspect, near_plane, far_plane);
        auto view = camera.lookAt();

        /// setting the uniform variables in shaders
        shader.set("projection", proj);
        shader.set("view", view);
        shader.set("model", model);
        lightShader.set("projection", proj);
        lightShader.set("view", view);
        lightShader.use();

        shader.set("whitelight.lightPos", whitelight.getPosition());
        shader.set("whitelight.lightColor", whitelight.getColor());
        shader.set("whitelight.lightSpace", whitelight.proj_view);
        shader.set("whitelight.shadowMap", 0);  

        lightShader.set("model", whitelight.getModel());
        lightShader.set("lightColor", whitelight.getColor());
        cube.draw();

        glActiveTexture(GL_TEXTURE0 + 0);
        glBindTexture(GL_TEXTURE_2D, whitelight.getDepthMap());

        shader.use();
        objects.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
