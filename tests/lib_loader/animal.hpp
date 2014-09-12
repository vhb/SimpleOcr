#pragma once
#include <iostream>

class Animal {
    public:
        virtual ~Animal() {
            std::cout << "~Animal" << std::endl;
        }
        virtual void scream() const = 0;
};
