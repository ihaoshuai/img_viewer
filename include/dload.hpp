#pragma once

#include "raylib.h"

#if defined(_WIN32)
    #include <windows.h>
    #define LIB_HANDLE HMODULE
    #define LOAD_LIB(path) LoadLibraryA(path)
    #define GET_FUNC(handle, name) GetProcAddress(handle, name)
    #define CLOSE_LIB(handle) FreeLibrary(handle)
    #define LIB_EXTENSION ".dll"
#elif defined(__linux__) || defined(__APPLE__)
    #include <dlfcn.h>
    #define LIB_HANDLE void*
    #define LOAD_LIB(path) dlopen(path, RTLD_LAZY)
    #define GET_FUNC(handle, name) dlsym(handle, name)
    #define CLOSE_LIB(handle) dlclose(handle)
    #define LIB_EXTENSION ".so"
#endif

#define UPDATE_UNIFORM_FUNC_NAME "UpdateShaderUniforms"
// #define INIT_UNIFORM_FUNC_NAME "InitShaderUniforms"

typedef void (*UpdateUniformsFunc)(Shader);
// typedef bool (*InitUniformsFunc)(Shader, const char*);

