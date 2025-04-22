#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "glad/glad.h"

class ShaderProgram {
private:
	unsigned int m_programID;
public:
	ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSouce);
	~ShaderProgram();
	
	bool use();
	bool setInt(std::string name, int value);
	bool setFloat(std::string name, float value);
	bool setVec(std::string name, std::vector<float> vectors);
};