cmake_minimum_required(VERSION 2.8)

set(TARGET TEST_WEAM)
#set(Boost_USE_STATIC_LIBS OFF)
#sudo apt-get install libboost-iostreams-dev
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)

project(test_weam_software)

add_executable(${TARGET}
    main.cpp
    custom_exceptions.cpp
    fileconverter_to_hash.cpp
    cmd_parser.h
    block_info.h
    )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(Boost 1.62 COMPONENTS iostreams filesystem program_options )

set(LIBS_TO_LINK ${BOOST_STATIC_SYSTEM_LIBRARY} ${Boost_LIBRARIES} ${Boost_LIBRARIES} )

if(Boost_FOUND)
target_link_libraries(${TARGET}
    ${LIBS_TO_LINK}
)

find_package(OpenSSL REQUIRED)
target_link_libraries(${TARGET} OpenSSL::SSL)

find_package (Threads REQUIRED)
target_link_libraries (${TARGET} ${CMAKE_THREAD_LIBS_INIT})

endif()


