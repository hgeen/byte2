// This file links translation units together and also links these translation units to dynamic libraries
#ifdef _WIN32

#include <windows.h>

void get_library() {
        HMODULE h = NULL;
}

void *extern_func() {
        
}

#elif defined(__linux__) || defined(__APPLE__)

#include <dlfcn.h>

void get_library() {

}

void *extern_func() {

}

#endif