#pragma once

#include <exception>

namespace rdr
{
    class InfoException : public std::exception
    {
    public:
        InfoException(std::string info) : m_info(std::move(info)) {}
        const char* what() const override { return m_info.c_str(); }
    private:
        std::string m_info;
    };
}

#define DeclareInfoException(name)\
class name final : public rdr::InfoException\
{\
public:\
    using Base = rdr::InfoException;\
    using Base::Base;\
}\

#define NonCopyable(classname)\
classname(const classname&) = delete;\
classname& operator =(const classname&) = delete;\

#define NonMovable(classname)\
classname(classname&&) = delete;\
classname& operator =(classname&&) = delete;\

#define NonCopyableMovable(classname) NonCopyable(classname)NonMovable(classname)
