set(DEPENDENCIES_INCLUDE 
	"${CMAKE_CURRENT_LIST_DIR}/glfw/include"
	"${CMAKE_CURRENT_LIST_DIR}/glad/include"
	"${CMAKE_CURRENT_LIST_DIR}/imgui"
	"${CMAKE_CURRENT_LIST_DIR}/imgui/backends"
)

set(DEPENDENCIES_LIB 
	"${CMAKE_CURRENT_LIST_DIR}/glfw/lib-vc2022"
)


add_library(imgui STATIC
	"${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_demo.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_tables.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_glfw.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_opengl3.cpp"
)
target_include_directories(imgui PRIVATE
	"${CMAKE_CURRENT_LIST_DIR}/glfw/include"
	"${CMAKE_CURRENT_LIST_DIR}/imgui"
	"${CMAKE_CURRENT_LIST_DIR}/imgui/backends"
)

target_include_directories(LearnOpenGL PRIVATE ${DEPENDENCIES_INCLUDE})
target_link_directories(LearnOpenGL PRIVATE ${DEPENDENCIES_LIB})
target_sources(LearnOpenGL PRIVATE 
	"${CMAKE_CURRENT_LIST_DIR}/glad/src/glad.c"
)