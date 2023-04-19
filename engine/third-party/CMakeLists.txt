# GLFW
set(GLFW_LIB_NAME "glfw-master") #not sensitive to upper and lower case?
set(GLFW_INC_PATH ${GLFW_LIB_NAME}/include)

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLFW_LIB_NAME})

#opencv
# set(OPENCV_BUILD_JASPER     OFF CACHE BOOL "" FORCE)
# set(OPENCV_BUILD_JAVA    OFF CACHE BOOL "" FORCE)
# set(OPENCV_BUILD_JPEG OFF CACHE BOOL "" FORCE)
# set(OPENCV_BUILD_LIST "core,highgui,imgcodecs")
set(OPENCV_LIB_NAME "opencv")
# set(OPENCV_INC_PATH ${OPENCV_LIB_NAME}/include)
add_subdirectory(${OPENCV_LIB_NAME})

#jsoncpp
set(JSONCPP_LIB_NAME "jsoncpp")
set(JSONCPP_INC_PATH ${JSONCPP_LIB_NAME}/include)
add_subdirectory(${JSONCPP_LIB_NAME})
# target_include_directories(
#     ${JSONCPP_LIB_NAME}
#     PUBLIC
#         ${JSONCPP_INC_PATH}
# )
# set(GLM_LIB_NAME "glm")
# set(GLM_INC_PATH ${GLM_LIB_NAME}/glm)

# add_library(${GLM_LIB_NAME} INTERFACE)

# target_include_directories(${GLM_LIB_NAME}
#     INTERFACE
#         ${GLM_INC_PATH}
# )

# glm
set(GLM_LIB_NAME "glm")
set(GLM_INC_PATH ${GLM_LIB_NAME}/glm)

set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)

add_subdirectory(${GLM_LIB_NAME})

# target_include_directories(${GLM_LIB_NAME}
#    PUBLIC
#        $(GLM_INC_PATH)
# )

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
# set(ASSIMP_SRC_PATH "${ASSIMP_LIB_NAME}/src")
# set(ASSIMP_INC_PATH "${ASSIMP_LIB_NAME}/include")


# add_library( ${ASSIMP_LIB_NAME}
#     "${ASSIMP_LIB_NAME}/libassimp-5.dll"
#     "${ASSIMP_LIB_NAME}/libassimp.dll.a"
# )

# target_include_directories(${ASSIMP_LIB_NAME}
# PUBLIC
# "${ASSIMP_INC_PATH}"
# )
# set_target_properties(${ASSIMP_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

# ImGui
# set(IMGUI_LIB_NAME "ImGui")

# set(IMGUI_SOURCES
#     "${IMGUI_LIB_NAME}/imgui.cpp"
#     "${IMGUI_LIB_NAME}/imgui_demo.cpp"
#     "${IMGUI_LIB_NAME}/imgui_draw.cpp"
#     "${IMGUI_LIB_NAME}/imgui_widgets.cpp"
#     "${IMGUI_LIB_NAME}/examples/imgui_impl_glfw.cpp"
#     "${IMGUI_LIB_NAME}/examples/imgui_impl_opengl3.cpp"
# )

# set(IMGUI_HEADERS
#     "${IMGUI_LIB_NAME}/imconfig.h"
#     "${IMGUI_LIB_NAME}/imgui.h"
#     "${IMGUI_LIB_NAME}/imgui_internal.h"
#     "${IMGUI_LIB_NAME}/imstb_rectpack.h"
#     "${IMGUI_LIB_NAME}/imstb_textedit.h"
#     "${IMGUI_LIB_NAME}/imstb_truetype.h"
#     "${IMGUI_LIB_NAME}/examples/imgui_impl_glfw.h"
#     "${IMGUI_LIB_NAME}/examples/imgui_impl_opengl3.h"
# )

# set(IMGUI_INC_PATH "${IMGUI_LIB_NAME}/")

# add_library(${IMGUI_LIB_NAME}
#     STATIC
#         ${IMGUI_SOURCES}
#         ${IMGUI_HEADERS}
# )

# target_compile_definitions(${IMGUI_LIB_NAME}
#     PRIVATE
#         IMGUI_IMPL_OPENGL_LOADER_GLAD=1
# )

# target_include_directories(${IMGUI_LIB_NAME}
#     PUBLIC
#         "${IMGUI_INC_PATH}"
#         "${GLFW_INC_PATH}"
#         "${GLAD_INC_PATH}"
# )