# CMakeList.txt : CMake project for LearnOpenGL, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (LearnOpenGL "LearnOpenGL.cpp" "ShaderProgram.cpp" "VertexBuffer.cpp" "IndexBuffer.cpp" "VertexArray.cpp" "VertexBufferLayout.cpp" "stb_image.h")
target_include_directories(LearnOpenGL PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(LearnOpenGL glfw3 imgui)

set_property(TARGET LearnOpenGL PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET LearnOpenGL PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
