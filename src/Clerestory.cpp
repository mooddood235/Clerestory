#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "Mesh.h"
#include "Primitives.h"
#include "WindowInfo.h"
#include "Texture.h"
#include "Camera.h"
#include "Volume.h"

WindowInfo InitGLFW();
void InitGlAD();
void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);

int main()
{
    WindowInfo windowInfo = InitGLFW();
    InitGlAD();

    glDebugMessageCallback(glDebugOutput, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glEnable(GL_FRAMEBUFFER_SRGB);
    // ---------------------------------
    Mesh quad = Mesh(QUAD_VERTS, QUAD_INDICES);
    // ---------------------------------
    ShaderProgram postProcessShader = ShaderProgram("src/Shaders/NDC.vert", "src/Shaders/PostProcess.frag");
    ShaderProgram renderShader = ShaderProgram("src/Shaders/Render.comp");
    // ---------------------------------
    Texture cumulativeRenderTexture = Texture(windowInfo.width, windowInfo.height);
    Texture finalRenderTexture = Texture(windowInfo.width, windowInfo.height);
    Texture environmentMap = Texture("HDRIs/puresky.hdr");

    cumulativeRenderTexture.BindImageTexture(1, GL_READ_WRITE);
    finalRenderTexture.BindImageTexture(0, GL_WRITE_ONLY);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, finalRenderTexture.GetID());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, environmentMap.GetID());

    postProcessShader.SetInt("finalRenderTexture", 0);
    renderShader.SetInt("environmentMap", 1);
    // ---------------------------------
    Camera camera = Camera(45.0f, windowInfo);
    camera.SetMoveSpeed(40.0f);
    // ---------------------------------
    Volume volume = Volume(glm::vec3(-1.0) * 10.0f, glm::vec3(1.0) * 10.0f);
    renderShader.SetVec3("volume.cornerMin", volume.cornerMin);
    renderShader.SetVec3("volume.cornerMax", volume.cornerMax);
    renderShader.SetVec3("volume.center", (volume.cornerMin + volume.cornerMax) / 2.0f);
    // ---------------------------------
    float lastTime = 0.0f;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // ---------------------------------

    float sampleNum = 1.0;
    glm::mat4 lastCamerModelMatrix = camera.GetModelMatrix();

    while (!glfwWindowShouldClose(windowInfo.window)) {
        // Calculate delta time
        float currTime = glfwGetTime();
        float deltaTime = currTime - lastTime;
        lastTime = currTime;

        // Input
        if (glfwGetKey(windowInfo.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(windowInfo.window, true);
        camera.ProcessInput(windowInfo, deltaTime);

        if (lastCamerModelMatrix != camera.GetModelMatrix()) {
            sampleNum = 1.0;
            lastCamerModelMatrix = camera.GetModelMatrix();
        }

        // Render
        glClear(GL_COLOR_BUFFER_BIT);
        
        renderShader.SetFloat("_Time", currTime);
        renderShader.SetFloat("_SampleNum", sampleNum);

        renderShader.SetVec3("camera.pos", camera.GetPosition());

        renderShader.SetVec3("camera.xAxis", camera.GetXAxis());
        renderShader.SetVec3("camera.yAxis", camera.GetYAxis());
        renderShader.SetVec3("camera.zAxis", camera.GetZAxis());

        renderShader.SetFloat("camera.focalLength", camera.GetFocalLength());

        renderShader.Use();
        glDispatchCompute(glm::ceil(windowInfo.width / 8), glm::ceil(windowInfo.height / 4), 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        postProcessShader.Use();
        quad.Draw();
        ShaderProgram::Unuse();

        sampleNum++;

        // Poll events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(windowInfo.window);
    }

    return 0;
}

WindowInfo InitGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Clerestory", NULL, NULL);
    
    if (!window) {
        std::cout << "ERROR: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return WindowInfo(window, 1920, 1080);
}
void InitGlAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "ERROR: Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}
void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " << message << std::endl;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
    case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
    case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
    case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
    case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
    case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
    case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}