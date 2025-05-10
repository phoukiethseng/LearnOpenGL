// LearnGLFW.cpp : Defines the entry point for the application.

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include "glad/glad.h" // include glad before GLFW
#include "GLFW/glfw3.h"

#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ShaderProgram.h"
#include "ErrorHandling.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Global State
ImGuiContext* g_ImGuiContext = nullptr;
float g_scaleFactor = 1.25f;
bool g_quitProgram = false;

int g_windowWidth = 1280;
int g_windowHeight = 720;

void CustomizeImGui(ImGuiIO& io, ImGuiStyle& style);
void PrepareBuffer(std::shared_ptr<VertexArray>& o_vertexArray, std::shared_ptr<IndexBuffer>& o_indexBuffer, std::shared_ptr<VertexBuffer>& o_vertexBuffer);
std::string LoadShaderFromFile(std::string filepath);

void UpdateWindowWidthAndHeight(GLFWwindow* p_window, int width, int height) {
	using namespace std;
	//cout << "Window size changed to " << width << "," << height << endl;
	g_windowWidth = width;
	g_windowHeight = height;
	GLCall(glViewport(0, 0, width, height)); // Tell OpenGL to resize its frame buffer
}

int Run() {

	// Imgui init
	g_ImGuiContext = ImGui::CreateContext();
	

	// Window hints for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create window and set callbacks
	GLFWwindow* window = glfwCreateWindow(g_windowWidth, g_windowHeight, "LearnOpenGL", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(window, UpdateWindowWidthAndHeight);

	// Tell GLFW to Use OpenGL context of specified window to be current context
	glfwMakeContextCurrent(window);

	// Tell GLAD to initalize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	} 

	std::shared_ptr<IndexBuffer> indexBuffer;
	std::shared_ptr<VertexArray> vertexArray;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	PrepareBuffer(vertexArray, indexBuffer, vertexBuffer);
	ShaderProgram shaderProgram(LoadShaderFromFile("res/shaders/vertex.shader"),
								LoadShaderFromFile("res/shaders/fragment.shader"));

	// Tell ImGui to Use our window for rendering
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// ImGui stlying customization
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImFont* mainFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18.0f * g_scaleFactor);
	IM_ASSERT(mainFont != NULL);
	CustomizeImGui(io, style);
	
	float rgb[] = { 0.0f, 0.0f, 0.0f };

	// Brick wall texture
	unsigned int brickWallTexture;
	glGenTextures(1, &brickWallTexture);
	glBindTexture(GL_TEXTURE_2D, brickWallTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, 100, 100, 0,)

	glfwSwapInterval(1);
	while (!g_quitProgram && !glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(mainFont);
		ImGui::Begin("Shader Parameters");
		ImGui::ColorPicker3("Uniform Color", rgb);
		ImGui::ShowDemoWindow();
		ImGui::End();
		ImGui::PopFont();

		ImGui::EndFrame();
		ImGui::Render();
		ImDrawData* drawData = ImGui::GetDrawData();

		// Clear frame buffer of previous draws
		glClearColor(0.0f, 0.0f, 0.0f, 0.4f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Triangle using vertex array we created
		shaderProgram.Use();
		shaderProgram.SetVec("u_Color", { rgb[0], rgb[1], rgb[2], 0.0f });
		shaderProgram.SetFloat("u_aspectRatio", (float)g_windowWidth / g_windowHeight);
		vertexArray->Bind();
		indexBuffer->Bind();
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
		
		ImGui_ImplOpenGL3_RenderDrawData(drawData);
		glfwSwapBuffers(window);
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(g_ImGuiContext);
}

int main() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to Initialized GLFW" << std::endl;
		return -1;
	}
	std::cout << "GLFW: " << glfwGetVersionString() << std::endl;
	int retVal = 0;
	{
		retVal = Run();
	}
	glfwTerminate();
	return retVal;
}

void CustomizeImGui(ImGuiIO& io, ImGuiStyle& style)
{
	style.WindowRounding = 3.5f;
	style.ScaleAllSizes(g_scaleFactor);
}

void PrepareBuffer(std::shared_ptr<VertexArray>& o_vertexArray, std::shared_ptr<IndexBuffer>& o_indexBuffer, std::shared_ptr<VertexBuffer>& o_vertexBuffer)
{
	float rectanglePos[] = {
		// Vertex Position	        // Vertex Color			// Texture Coordinate
		-0.5f, -0.5f,  0.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,		0.0f, 1.0f, 0.0f,		1.0f, 0.0f,
		 0.5f,  0.5f,  0.0f,		0.0f, 0.0f, 1.0f,		1.0f, 1.0f,
		-0.5f,  0.5f,  0.0f,		1.0f, 0.2f, 0.0f,		0.0f, 1.0f
	};

	unsigned int indices[]{
		1, 2, 0, 3
	};
	
	std::shared_ptr<VertexArray> vertexArray = std::make_shared<VertexArray>();
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(rectanglePos, 24);
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, 4);

	VertexBufferLayout vertexBufferLayout;
	vertexBufferLayout.Push<float>(3, true); // Vertex Position
	vertexBufferLayout.Push<float>(3, true); // Vertex Color
	vertexBufferLayout.Push<float>(2, true); // Texture Coordinate

	vertexArray->AddBuffer(*vertexBuffer, vertexBufferLayout);

	o_vertexArray = vertexArray;
	o_indexBuffer = indexBuffer;
	o_vertexBuffer = vertexBuffer;
}

std::string LoadShaderFromFile(std::string filepath)
{
	std::ifstream shaderFile(filepath);
	if (!shaderFile.is_open()) {
		std::cout << "Cannot load shader file " << filepath << "\n";
	}

	std::string shaderSource;
	std::string line;
	while (std::getline(shaderFile, line)) {
		shaderSource.append(line + "\n");
	}
	shaderFile.close();

	return shaderSource;
}
