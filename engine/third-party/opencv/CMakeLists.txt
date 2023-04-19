add_library(opencv SHARED IMPORTED GLOBAL)
set_property(TARGET opencv PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/opencv_world470d.dll)
set_property(TARGET opencv PROPERTY IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/opencv_world470d.lib)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/opencv_world470d.dll DESTINATION ${BINARY_ROOT_DIR})

target_include_directories(opencv INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)