cmake_minimum_required(VERSION 3.28)
project(Ueb9)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        arm.cpp
        ka_tra_os.cpp
        lib_fh3.cpp
        rob.cpp
        scara_w4_jg_v02_vsc.cpp
        ueb9.cpp
        ueb9_bewegen.cpp
        ueb9_fenster.cpp
)
add_executable(Ueb9 WIN32 ${SOURCE_FILES})