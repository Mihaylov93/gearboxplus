#include "corefactory.hpp"
#include "a06core.hpp"
#include "a04core.hpp"

BaseCore *CoreFactory::make_core(std::string core)
{
    if (core == "a06") {
        return new A06Core();
    } else if (core == "a04") {
        return new A04Core();
    } else
        return nullptr;
}
