#include <iostream>
#include <utility>
#include <animal.hpp>

class Panda : public Animal {
    public:
        Panda(std::string const &s)
            : _s(s) {
            std::cout << "panda: " << s << std::endl;
        }

        virtual void scream() const {
            std::cout << "panda: " << _s << std::endl;
        }

        std::string _s;
};

extern "C" {
    Panda *constructor(char const *s) {
        return new Panda(std::string(s));
    }
}

