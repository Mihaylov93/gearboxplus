#include "corefactory.hpp"
#include "a04core.hpp"
#include "a06core.hpp"

auto CoreFactory::make_core(const std::string& core) -> BaseCore*
{
    if (core == "a06") {
        return new A06Core();
    } else if (core == "a04") {
        return new A04Core();
    }

    return nullptr;
}
