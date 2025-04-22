#include "ShaderProgram.h"
#include "ErrorHandling.h"

ShaderProgram::ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSouce) {
	GLCall(unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER));
	GLCall(unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));

	const char* vert = vertexShaderSource.c_str();
	const char* frag = fragmentShaderSouce.c_str();
	GLCall(glShaderSource(vertexShader, 1, &vert, nullptr));
	GLCall(glShaderSource(fragmentShader, 1, &frag, nullptr));

	int success;
	GLCall(glCompileShader(vertexShader));
	char infoLog[512];
	GLCall(glCompileShader(vertexShader));
	GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
	if(!success)
	{
		GLCall(glGetShaderInfoLog(vertexShader, 512, NULL, infoLog));
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLCall(glCompileShader(fragmentShader));
	GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLCall(unsigned int program = glCreateProgram());
	GLCall(glAttachShader(program, vertexShader));
	GLCall(glAttachShader(program, fragmentShader));
	glLinkProgram(program);

	m_programID = program;
}

ShaderProgram::~ShaderProgram()
{
	GLCall(glDeleteProgram(m_programID));
}

bool ShaderProgram::use()
{
	GLCall(glUseProgram(m_programID));
	return true;
}

bool ShaderProgram::setInt(std::string name, int value)
{
	GLCall(int uniform = glGetUniformLocation(m_programID, name.c_str()));
	GLCall(glUniform1i(uniform, value));
	return true;
}

bool ShaderProgram::setFloat(std::string name, float value)
{
	GLCall(int uniform = glGetUniformLocation(m_programID, name.c_str()));
	GLCall(glUniform1f(uniform, value));
	return true;
}


bool ShaderProgram::setVec(std::string name, std::vector<float> vectors)
{
	GLCall(int uniform = glGetUniformLocation(m_programID, name.c_str()));
	size_t length = vectors.size();
	switch (length) {
	case 2: GLCall(glUniform2f(uniform, vectors[0], vectors[1])); break;
	case 3: GLCall(glUniform3f(uniform, vectors[0], vectors[1], vectors[2])); break;
	case 4: GLCall(glUniform4f(uniform, vectors[0], vectors[1], vectors[2], vectors[3])); break;
	}
	return true;
}
