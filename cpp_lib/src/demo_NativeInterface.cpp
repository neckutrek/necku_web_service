#include "demo_NativeInterface.h"

#include <iostream>

JNIEXPORT void JNICALL Java_demo_NativeInterface_native_1helloWorld(
    JNIEnv *, jobject)
{
    std::cout << "Hello World from C++!" << std::endl;
}