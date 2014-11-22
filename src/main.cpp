// Simple OCR program
// Copyright (C) 2014 vhb
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <Brain.hpp>

int main(int ac, char * const av[]) {
    try {
        using namespace ocr;
        if (ac < 4) {
            std::cerr << "Usage: ocr image_path json_path dataset_path [xml_path]"
                      << std::endl;
            return 1;
        }
        Brain brain(av[2], av[3]);
        if (ac == 5) {
            brain.loadTrainData(av[4]);
        }
        else {
            brain.train();
        }
        brain.apply(av[1]);
    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "An exception occured, exiting" << std::endl;
    }
    return 0;
}

