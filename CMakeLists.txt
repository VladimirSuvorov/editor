cmake_minimum_required(VERSION 3.8)

#Project config
set(PROJECT_NAME "print_ip")
if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
  project(${PROJECT_NAME} VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
  project(${PROJECT_NAME} VERSION 0.0.1)
endif()



#Packages
set (GTEST_ROOT /usr/src/gtest)
set (GTEST_LIBRARY /usr/src/gtest/libgtest.a)
set (GTEST_MAIN_LIBRARY /usr/src/gtest/libgtest_main.a)

#find_package(Boost REQUIRED)
#find_package(Boost COMPONENTS system REQUIRED)
find_package(GTest REQUIRED)

find_package(Threads REQUIRED)

include_directories(
    ../include
    ${GTEST_INCLUDE_DIRS}
)

#Configure
#configure_file(version.h.in version.h)

#Targets
add_executable(print_ip Source.cpp)
add_executable(homework_test tests.cpp)



#Compilation setings
set_target_properties(print_ip homework_test
PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra;--std=c++17;"
#    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

set_target_properties(homework_test
PROPERTIES
    INCLUDE_DIRECTORIES ${GTEST_INCLUDE_DIR}
)


#Linking
#target_link_libraries(allocator
#    ${Boost_LIBRARIES}
#)


target_link_libraries(homework_test
    ${GTEST_LIBRARIES}
    Threads::Threads
)

#Install
install(TARGETS print_ip RUNTIME DESTINATION bin)
#CPack
set (CPACK_GENERATOR DEB)

#set (CPACK_DEB_COMPONENT_INSTALL ON)
#set (CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON)
#set (CPACK_DEBIAN_PROG_PACKAGE_DEPENDS print_ip-lib)
#set (CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
#set (CPACK_PACKAGE_ARCHITECTURE "amd64")
set(CPACK_PACKAGE_CONTACT vsuvorov1996@gmail.com)
#set (CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)


#set(CPACK_GENERATOR DEB)
#set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
#set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
#set(CPACK_PACKAGE_CONTACT vsuvorov1996@gmail.com)
include(CPack)


#Testing
enable_testing()
add_test(homework_test homework_test)
