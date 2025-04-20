# CMakeList.txt : CMake project for LearnOpenGL, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (LearnOpenGL "LearnOpenGL.cpp" "LearnOpenGL.h" "ShaderProgram.cpp" "ShaderProgram.h")

target_link_libraries(LearnOpenGL glfw3 imgui)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LearnOpenGL PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
