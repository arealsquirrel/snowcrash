# Set the project name
project (snowcrash
		 VERSION 1.0.0)

file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.hpp)

# Add a library with the above sources
add_library(${PROJECT_NAME} ${SRCS} ${HDRS})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}

)
