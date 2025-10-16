set(MH_SOURCES
    minhook/src/buffer.c
    minhook/src/hook.c
    minhook/src/trampoline.c
)

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(HDE_SOURCES minhook/src/hde/hde32.c)
else()
    set(HDE_SOURCES minhook/src/hde/hde64.c)
endif()

set(SOURCES 
    dinput8.c
    dinput8.def
    fps.c
    utils.c
)

add_library(dinput8 SHARED ${MH_SOURCES} ${HDE_SOURCES} ${SOURCES})
target_include_directories(dinput8 PRIVATE minhook/include)
target_compile_definitions(dinput8 PRIVATE -D_CRT_SECURE_NO_WARNINGS)
target_compile_options(dinput8 PRIVATE /W3)
