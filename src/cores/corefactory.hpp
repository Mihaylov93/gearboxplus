#pragma once

#include "basecore.hpp"

class CoreFactory : public BaseCore
{
public:
    CoreFactory() = default;
    static BaseCore *make_core(std::string core);
};