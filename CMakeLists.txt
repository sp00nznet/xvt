cmake_minimum_required(VERSION 3.20)
project(XvTRecomp VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

if(MSVC)
    add_link_options(/DYNAMICBASE:NO)      # fixed addresses; no ASLR
    add_link_options(/BASE:0x10000000)     # keep 0x00100000-0x00C00000 free for the guest
    add_link_options(/STACK:8388608)       # lifted code makes deep host call chains

    add_compile_options(/W1 /wd4996 /wd4244 /wd4146 /wd4102 /wd4013 /wd4101 /wd4267 /wd4018)

    # /GL misoptimizes cross-function global-register access in lifted code (XWA finding)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /O2 /Oi")
endif()

file(GLOB GENERATED_SOURCES "src/game/recomp/gen/recomp_*.c")

add_executable(xvt_recomp
    src/game/main.c
    src/game/imports.c
    ${GENERATED_SOURCES}
)

target_include_directories(xvt_recomp PRIVATE
    src/game
    src/game/recomp
    src/game/recomp/gen
)

target_link_libraries(xvt_recomp PRIVATE
    kernel32 user32 gdi32 advapi32 shell32 ole32 winmm
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Must build 32-bit: cmake -B build -G \"Visual Studio 17 2022\" -A Win32")
endif()
