add_library(assimp SHARED IMPORTED GLOBAL)
set_property(TARGET assimp PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/assimp-vc143-mt.dll)
set_property(TARGET assimp PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/assimp-vc143-mt.lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/assimp-vc143-mt.dll DESTINATION ${BINARY_ROOT_DIR})

target_include_directories(assimp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)