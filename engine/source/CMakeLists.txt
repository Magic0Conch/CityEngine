set(ENGINE_RUNTIME_DIR "${ENGINE_SOURCE_DIR}/runtime")
set(ENGINE_TOOL_DIR "${ENGINE_SOURCE_DIR}/tool")
# Taget etool
set(ETOOL_NAME "etool")
file(GLOB_RECURSE ETOOL_SRC "${ENGINE_TOOL_DIR}/*.cpp")
file(GLOB_RECURSE ETOOL_INC "${ENGINE_TOOL_DIR}/*.h")
add_library(${ETOOL_NAME} SHARED ${ETOOL_SRC} ${ETOOL_INC})
target_include_directories(${ETOOL_NAME} PUBLIC "tool")
target_link_libraries(${ETOOL_NAME} PUBLIC glfw)


# Target runtime
set(RUNTIME_NAME "engine-runtime")
# set(RUNTIME_INC_PATH ${GLFW_LIB_NAME}/include)
find_package(OpenGL REQUIRED)
file(GLOB_RECURSE RUNTIME_SRC "${ENGINE_RUNTIME_DIR}/*.cpp")
add_executable(${RUNTIME_NAME} ${RUNTIME_SRC}) #.h?
target_compile_definitions(${RUNTIME_NAME} PRIVATE -DBINARY_ROOT_DIR=\"${BINARY_ROOT_DIR}\")


target_include_directories(
  ${RUNTIME_NAME}
    PUBLIC
    external
        "${ENGINE_SOURCE_DIR}"
        ${OPENGL_INCLUDE_DIR}
        ${GLAD_INC_PATH}
)


target_link_libraries(${RUNTIME_NAME}
    PUBLIC
        ${OPENGL_gl_LIBRARY}
        glfw
        glad
        glm
        jsoncpp_static    
        opencv
        assimp
        imgui
        etool
)
