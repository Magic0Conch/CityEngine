
set(ENGINE_ROOT_DIR "${EASYENGINE_ROOT_DIR}/engine")
set(THIRD_PARTY_DIR "${ENGINE_ROOT_DIR}/third-party")
set(ENGINE_ASSET_DIR "/asset")
set(ENGINE_SOURCE_DIR "${ENGINE_ROOT_DIR}/source")


set(SHADER_COMPILE_TARGET EasyEngineShaderCompile)
add_subdirectory(shader)
add_subdirectory(third-party)
add_subdirectory(source)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/asset/ DESTINATION ${CMAKE_INSTALL_PREFIX}/asset)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/shader/ DESTINATION ${CMAKE_INSTALL_PREFIX}/shader)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/configs/ DESTINATION ${CMAKE_INSTALL_PREFIX}/configs)
