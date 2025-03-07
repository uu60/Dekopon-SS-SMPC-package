 cmake_minimum_required(VERSION 3.27)

 project(secure_2pc_package)

 set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -O2 -g -fsanitize=address")
 set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -O2")
# cmake_policy(SET CMP0167 NEW)

 find_package(MPI REQUIRED)
 find_package(OpenSSL REQUIRED)
 find_package(tbb REQUIRED)
 find_package(Boost REQUIRED)


 file(GLOB_RECURSE LIB_SOURCES
#         "benchmark/*.h"
#         "benchmark/*.cpp"
         "include/*.h"
         "include/*.cpp"
         "src/*.cpp"
 )

 add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})

 target_include_directories(${PROJECT_NAME} PUBLIC
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>)

 target_link_libraries(
         ${PROJECT_NAME} PUBLIC MPI::MPI_CXX OpenSSL::SSL OpenSSL::Crypto TBB::tbb Boost::boost
 )

 # install libxx.so to ${CMAKE_INSTALL_PREFIX}/lib
 install(
         TARGETS ${PROJECT_NAME}
         EXPORT ${PROJECT_NAME}Targets
         ARCHIVE DESTINATION lib
         LIBRARY DESTINATION lib
         RUNTIME DESTINATION bin
 )

 # headers path
 install(DIRECTORY include/
         DESTINATION include/${PROJECT_NAME}
         FILES_MATCHING PATTERN "*.h")

 # generate xxTargets.cmake
 install(
         EXPORT ${PROJECT_NAME}Targets
         FILE ${PROJECT_NAME}Targets.cmake
         DESTINATION lib/cmake/${PROJECT_NAME}
 )

 # parameters used in xxConfig.cmake
 set(INCLUDE_DIRS include)
 set(LIBRARIES ${PROJECT_NAME})
 set(LIB_DIR lib)

 # provided by cmake
 include(CMakePackageConfigHelpers)

 # generate xxConfigVersion.cmake
 write_basic_package_version_file(
         ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
         VERSION 0.0.1
         COMPATIBILITY SameMajorVersion
 )

 # generate xxConfig.cmake
 configure_package_config_file(
         ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in
         ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
         INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
         PATH_VARS INCLUDE_DIRS LIBRARIES LIB_DIR
         INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}
 )

 install(
         FILES ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
         DESTINATION lib/cmake/${PROJECT_NAME}
 )