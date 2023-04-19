
set(ENGINE_ROOT_DIR "${EASYENGINE_ROOT_DIR}/engine")
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/third-party")
set(ENGINE_ASSET_DIR "/asset")


# set(CMAKE_CXX_FLAGS "-lstdc++fs")
set(SHADER_COMPILE_TARGET EasyEngineShaderCompile)
add_subdirectory(shader)
add_subdirectory(third-party)
add_subdirectory(source/runtime)

# include_directories("${ENGINE_ROOT_DIR}/source")
# include_path()
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/asset/ DESTINATION ${CMAKE_INSTALL_PREFIX}/asset)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shader/ DESTINATION ${CMAKE_INSTALL_PREFIX}/shader)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/configs/ DESTINATION ${CMAKE_INSTALL_PREFIX}/configs)