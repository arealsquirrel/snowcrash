
project(testing)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME}
	snowcrash
)

enable_testing()
# add_test(CPMExampleCatch2 CPMExampleCatch2)

