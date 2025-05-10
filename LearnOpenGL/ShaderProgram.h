#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "glad/glad.h"

class ShaderProgram {
/**
	Represent a compiled and linked shader instance. For shader program contains only vertex and fragment shader.
**/
private:
	unsigned int m_rendererID;
public:
	ShaderProgram(const std::string& vertexShaderSource, const std::string& fragmentShaderSouce);
	~ShaderProgram();
	
	bool Use();
	bool SetInt(std::string name, int value);
	bool SetFloat(std::string name, float value);
	bool SetVec(std::string name, std::vector<float> vectors);
};