cmake_minimum_required(VERSION 3.10.2)
project(OnecardBasic)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(OnecardBasic
        Card.c
        Card.h
        CMakeLists.txt
        Game.c
        Game.h
        main.c
        main.h
        Player.c
        Player.h
        PlayerCard.c
        PlayerCard.h)
