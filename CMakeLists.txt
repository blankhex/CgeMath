cmake_minimum_required(VERSION 3.10)
project(CgeMath LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCES
    Box2f.c
    Box3f.c
    Line.c
    Mat3f.c
    Mat4f.c
    Misc.c
    Plane.c
    Quat4f.c
    Ray2f.c
    Ray3f.c
    Vec2f.c
    Vec2i.c
    Vec3f.c
    Vec3i.c
    Vec4f.c
    Vec4i.c
)

set(HEADERS
    CgeMath.h
)

add_library(CgeMath STATIC ${SOURCES} ${HEADERS})

target_include_directories(CgeMath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS CgeMath
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS} DESTINATION include)
