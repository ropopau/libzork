
file(GLOB ZORKXPLORER_SRCS src/*.cc)

add_executable(zorkxplorer ${ZORKXPLORER_SRCS})

# Lier la libzork
target_link_libraries(zorkxplorer PRIVATE libzork)

# Inclure les headers
target_include_directories(zorkxplorer PRIVATE ${CMAKE_SOURCE_DIR}/include)
