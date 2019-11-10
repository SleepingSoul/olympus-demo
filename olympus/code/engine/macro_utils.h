#pragma once

#define DeclareSingleton(classname)\
public:\
classname() = default;\
static classname& getInstance();\
private:\

#define ImplementSignleton(classname, ...)\
classname Instance{__VA_ARGS__};\
classname& classname::getInstance()\
{\
    return Instance;\
}\

