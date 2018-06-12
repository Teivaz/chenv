set(PROJ_NAME chenv)
project("${PROJ_NAME}")

add_definitions(-DUNICODE -D_UNICODE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

file(GLOB_RECURSE sources "${CMAKE_CURRENT_LIST_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_LIST_DIR}/src/*.h")

if(YES)
	file(GLOB_RECURSE sources_test "${CMAKE_CURRENT_LIST_DIR}/tests/*.cpp")
	file(GLOB_RECURSE headers_test "${CMAKE_CURRENT_LIST_DIR}/tests/*.h")
	set(sources "${sources}" "${sources_test}")
	set(headers "${headers}" "${headers_test}")
endif()

set(deps "gtest" "Kernel32.lib" "Shlwapi.lib")

add_executable("${PROJ_NAME}" ${sources} ${headers})


target_include_directories("${PROJ_NAME}" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src")
target_include_directories("${PROJ_NAME}" PUBLIC "${CMAKE_CURRENT_LIST_DIR}/tests")
target_compile_definitions("${PROJ_NAME}" PRIVATE "GTEST_USE_OWN_TR1_TUPLE=0")

target_link_libraries("${PROJ_NAME}" PUBLIC ${deps})

