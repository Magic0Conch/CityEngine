# GLFW
set(GLFW_LIB_NAME "glfw-master") #not sensitive to upper and lower case?
set(GLFW_INC_PATH ${GLFW_LIB_NAME}/include)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_LIB_NAME})

#opencv
set(OPENCV_LIB_NAME "opencv")
add_subdirectory(${OPENCV_LIB_NAME})

#jsoncpp
set(JSONCPP_LIB_NAME "jsoncpp")
set(JSONCPP_INC_PATH ${JSONCPP_LIB_NAME}/include)
add_subdirectory(${JSONCPP_LIB_NAME})

# glm
set(GLM_LIB_NAME "glm")
set(GLM_INC_PATH ${GLM_LIB_NAME}/glm)

set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLM_LIB_NAME})


# Glad
set(GLAD_LIB_NAME "glad")
set(GLAD_SRC_PATH "${GLAD_LIB_NAME}/src")
set(GLAD_INC_PATH "${GLAD_LIB_NAME}/include")

add_library( ${GLAD_LIB_NAME}
    STATIC
        "${GLAD_SRC_PATH}/glad.c"
)

target_include_directories(${GLAD_LIB_NAME}
    PUBLIC
        ${GLAD_INC_PATH}
)

# assimp
set(ASSIMP_LIB_NAME "assimp")
add_subdirectory(${ASSIMP_LIB_NAME})

# imgui
if(NOT TARGET imgui)
    include(imgui.cmake)
    set_target_properties(imgui PROPERTIES FOLDER ${third_party_folder}/imgui)
endif()
