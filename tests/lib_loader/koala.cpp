#include <iostream>
#include "animal.hpp"

class Koala : public Animal {

    public:
        Koala() {
            std::cout << "Koala" << std::endl;
        }

        virtual void scream() const {
            std::cout << "Kreog" << std::endl;
        }
};

extern "C" {
    Koala *constructor() {
        return new Koala;
    }
}
