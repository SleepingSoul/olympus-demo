#pragma once

#define OlyNonCopyable(classname)\
classname(const classname&) = delete;\
classname& operator =(const classname&) = delete;\

#define OlyNonMovable(classname)\
classname(classname&&) = delete;\
classname& operator =(classname&&) = delete;\

#define OlyNonCopyableMovable(classname) OlyNonCopyable(classname)OlyNonMovable(classname)

#define BeginNamespaceOlympus namespace oly {
#define EndNamespaceOlympus }