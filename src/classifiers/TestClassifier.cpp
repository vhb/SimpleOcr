// Simple ocr
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


#include "TestClassifier.hpp"
#include <iostream>

namespace ocr {
    std::string
    TestClassifier::classify(cv::Mat &&features, Dataset const &) const
    {
        std::cout << "TestClassifier" << std::endl;
        return "c";
    }

    char const *
    TestClassifier::name() const
    {
        return "TestClassifier";
    }

    void
    TestClassifier::serialize(std::string &&dest_path) const
    {
    }

    void
    TestClassifier::train(Dataset &&dataset_path)
    {
    }

} /* namespace ocr */

extern "C"
ocr::TestClassifier *constructor(){
    auto value = new ocr::TestClassifier;
    std::cout << "constructor classifier" << value << std::endl;
    return value;
}
