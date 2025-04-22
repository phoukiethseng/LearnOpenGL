// LearnGLFW.cpp : Defines the entry point for the application.

#include "LearnOpenGL.h"
#include <memory>
#include "ShaderProgram.h"
#include "ErrorHandling.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

// Global State
ImGuiContext* g_ImGuiContext = nullptr;
float g_scaleFactor = 1.25f;
bool g_quitProgram = false;

int g_windowWidth = 800;
int g_windowHeight = 600;

void MakeUI();
void CustomizeImGui(ImGuiIO& io, ImGuiStyle& style);
void PrepareBuffer(std::shared_ptr<GLuint>& o_vertexArray, std::shared_ptr<IndexBuffer>& o_indexBuffer, std::shared_ptr<VertexBuffer>& o_vertexBuffer);
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

	// Tell GLFW to use OpenGL context of specified window to be current context
	glfwMakeContextCurrent(window);

	// Tell GLAD to initalize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	} 

	std::shared_ptr<IndexBuffer> indexBuffer;
	std::shared_ptr<unsigned int> vertexArray;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	PrepareBuffer(vertexArray, indexBuffer, vertexBuffer);
	ShaderProgram shaderProgram(LoadShaderFromFile("res/shaders/vertex.shader"),
								LoadShaderFromFile("res/shaders/fragment.shader"));

	// Tell ImGui to use our window for rendering
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// ImGui stlying customization
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImFont* mainFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18.0f);
	IM_ASSERT(mainFont != NULL);
	CustomizeImGui(io, style);
	io.FontGlobalScale = g_scaleFactor;

	float rgb[] = { 0.0f, 0.0f, 0.0f };

	glfwSwapInterval(1);
	while (!g_quitProgram && !glfwWindowShouldClose(window)) {
		glfwPollEvents();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::PushFont(mainFont);
		//MakeUI();
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
		shaderProgram.use();
		shaderProgram.setVec("u_Color", { rgb[0], rgb[1], rgb[2], 0.0f });
		shaderProgram.setFloat("u_aspectRatio", (float)g_windowWidth / g_windowHeight);
		glBindVertexArray(*vertexArray);
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
	int retVal = Run();
	glfwTerminate();
	return retVal;
}

void MakeUI()
{
	using namespace ImGui;
	if (BeginMainMenuBar()) {
		if (BeginMenu("File")) {
			if (MenuItem("Quit")) {
				g_quitProgram = true;
			}
			EndMenu();
		}

		EndMainMenuBar();
	}
}

void CustomizeImGui(ImGuiIO& io, ImGuiStyle& style)
{
	style.WindowRounding = 3.5f;
	style.ScaleAllSizes(g_scaleFactor);
}

void PrepareBuffer(std::shared_ptr<GLuint>& o_vertexArray, std::shared_ptr<IndexBuffer>& o_indexBuffer, std::shared_ptr<VertexBuffer>& o_vertexBuffer)
{
	float trianglePositions[] = {
		// Vertex pos			    // Vertex color
		-0.5f, -0.5f,  0.0f,		1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,		0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.0f,		0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.0f,		0.0f, 0.2f, 0.0f,
	};

	unsigned int indices[]{
		1, 2, 0, 3
	};
	
	unsigned int vertexArray;
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	std::shared_ptr<VertexBuffer> vertexBuffer = std::make_shared<VertexBuffer>(trianglePositions, 24);
	std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices, 4);

	vertexBuffer->Bind();
	indexBuffer->Bind();


	GLuint vertexStride = 6 * sizeof(float);

	// Vertex position attribute
	//		attribute index will be 0
	//		number of component of attribute will be 3 (since it is 3D position vector)
	//		value already normalized so we pass GL_FALSE
	//		since a vertex contain 6 unit of floats (3 floats of position and 3 floats of rgb color), the stride will be 6 of 4 bytes float
	//		offset in bytes within a vertex to the attribute will be 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexStride, (const void*)0);

	// Vertex position attribute
	//		attribute index will be 1
	//		number of component of attribute will be 3 (since it is RGB values)
	//		value already normalized so we pass GL_FALSE
	//		since a vertex contain 5 unit of floats (3 floats of position and 3 floats of rgb color), the stride will be 6 of 4 bytes float
	//		offset in bytes within a vertex to the attribute will be 3 of 4 bytes float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexStride, (const void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	o_vertexArray = std::make_shared<unsigned int>(vertexArray);
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
