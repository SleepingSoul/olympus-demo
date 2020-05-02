#pragma once

#include <exception>
#include <utils/macros.h>

BeginNamespaceOlympus

class InfoException : public std::exception
{
public:
    InfoException(std::string info) : m_info(std::move(info)) {}
    const char* what() const override { return m_info.c_str(); }
private:
    std::string m_info;
};

EndNamespaceOlympus

#define DeclareInfoException(name)\
class name final : public oly::InfoException\
{\
public:\
    using Base = oly::InfoException;\
    using Base::Base;\
}\

