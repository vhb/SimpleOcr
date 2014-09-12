#include <utils/lib_loader.hpp>
#include <dlfcn.h>
#include "animal.hpp"

int main() {


    utils::LibLoader a;

    auto koala = a.load<Animal*>("./libkoala");
    auto panda = a.load<Animal*>("./libpanda", std::string("toto"));

    koala->scream();
    panda->scream();

    return 0;
}
