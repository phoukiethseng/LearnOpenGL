#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(ShaderProgram& program) {
	
}

ShaderProgram::ShaderProgram(std::string& vertexShaderSource, std::string& fragmentShaderSource) {
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vert = vertexShaderSource.c_str();
	const char* frag = fragmentShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vert, nullptr);
	glShaderSource(fragmentShader, 1, &frag, nullptr);

	int success;
	glCompileShader(vertexShader);
	char infoLog[512];
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	m_programID = program;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_programID);
}

bool ShaderProgram::use()
{
	glUseProgram(m_programID);
	return true;
}

bool ShaderProgram::setInt(std::string name, int value)
{
	int uniform = glGetUniformLocation(m_programID, name.c_str());
	glUniform1i(uniform, value);
	return true;
}

bool ShaderProgram::setFloat(std::string name, float value)
{
	int uniform = glGetUniformLocation(m_programID, name.c_str());
	glUniform1f(uniform, value);
	return true;
}


bool ShaderProgram::setVec(std::string name, std::vector<float> vectors)
{
	int uniform = glGetUniformLocation(m_programID, name.c_str());
	int length = vectors.size();
	switch (length) {
	case 2: glUniform2f(uniform, vectors[0], vectors[1]); break;
	case 3: glUniform3f(uniform, vectors[0], vectors[1], vectors[2]); break;
	case 4: glUniform4f(uniform, vectors[0], vectors[1], vectors[2], vectors[3]); break;
	}
	return true;
}
