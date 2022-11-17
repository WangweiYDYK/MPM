cmake_minimum_required(VERSION 3.0.0)
project(HelloOpenGL VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)
#settings of include path


set(GLFW_H "D:\\Library\\OpenGL\\glfw-3.3.8\\include")
set(GLM "D:\\Library\\OpenGL\\glm-0.9.8.0")

include_directories(${GLFW_H} ${GLM})
include_directories(include)

set(GLFW_LINK "D:\\Library\\OpenGL\\glfw-3.3.8\\build\\src\\libglfw3dll.a")

link_libraries(${GLFW_LINK} )


aux_source_directory(${PROJECT_SOURCE_DIR}/src  SOURCES)
add_executable(HelloOpenGL ${SOURCES} glad.c)


include(CTest)
enable_testing()
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
