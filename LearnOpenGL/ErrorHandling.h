#pragma once
#include <iostream>
#include "glad/glad.h"

#define ASSERT(x) if (!x) __debugbreak()
#define GLCall(x) \
	ClearGLError(); \
	x; \
	ASSERT(GetGLLog());
	


static void ClearGLError() {
	while (glGetError() != GL_NO_ERROR);
}

static bool GetGLLog() {
	using namespace std;
	GLenum error;
	bool noError = true;
	while ((error = glGetError()) != GL_NO_ERROR) {
		cout << "[OpenGL Error] (" << error << ")" << endl;
		noError = false;
	}
	return noError;
}
