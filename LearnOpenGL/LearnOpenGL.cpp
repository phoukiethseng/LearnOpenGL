// LearnGLFW.cpp : Defines the entry point for the application.

#include "LearnOpenGL.h"
#include "ShaderProgram.h"

// Global State
ImGuiContext* g_ImGuiContext = nullptr;
float g_scaleFactor = 1.25f;
bool g_quitProgram = false;

void MakeUI();
void CustomizeImGui(ImGuiIO& io, ImGuiStyle& style);
void PrepareBuffer(GLuint* o_vertexArray, GLuint* o_indexBuffer);
std::string LoadShaderFromFile(std::string filepath);

int main() {
	if (glfwInit() == GLFW_FALSE) {
		std::cout << "Failed to Initialized GLFW" << std::endl;
		return -1;
	}
	std::cout << "GLFW: " << glfwGetVersionString() << std::endl;

	// Imgui init
	g_ImGuiContext = ImGui::CreateContext();
	

	// Window hints for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	GLFWwindow* window = glfwCreateWindow(1000, 800, "LearnOpenGL", nullptr, nullptr);

	// Tell GLFW to use OpenGL context of specified window to be current context
	glfwMakeContextCurrent(window);

	// Tell GLAD to initalize
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	} 

	unsigned int vertexArray, indexBuffer;
	PrepareBuffer(&vertexArray, &indexBuffer);
	std::string vertexShaderSource = LoadShaderFromFile("res/shaders/vertex.shader");
	std::string fragmentShaderSource = LoadShaderFromFile("res/shaders/fragment.shader");
	ShaderProgram shaderProgram(vertexShaderSource, fragmentShaderSource);

	// Tell ImGui to use 
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
		glBindVertexArray(vertexArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, nullptr);
		
		ImGui_ImplOpenGL3_RenderDrawData(drawData);
		glfwSwapBuffers(window);
	}
	
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(g_ImGuiContext);
	glfwTerminate();
	return 0;
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

void PrepareBuffer(GLuint* o_vertexArray, GLuint* o_indexBuffer)
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
	
	GLuint vertexBuffer;
	GLuint vertexArray;
	GLuint indexBuffer;

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &vertexBuffer); // Create buffer object for our triangle
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); // Bind that to OpenGL array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(trianglePositions), &trianglePositions, GL_STATIC_DRAW); // Copy buffer data to GPU

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
	
	*o_vertexArray = vertexArray;
	*o_indexBuffer = indexBuffer;
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
