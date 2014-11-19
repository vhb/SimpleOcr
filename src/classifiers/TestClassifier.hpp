// Simple ocr
// Copyright (C) 2014 vhb
//
// Permission is hereby granted, free of charge, to any person obtaining
// A copy of this software and associated documentation files (the "Software"),
// To deal in the Software without restriction, including without limitation
// The rights to use, copy, modify, merge, publish, distribute, sublicense,
// And/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// In all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include <IClassifier.hpp>
#include <Dataset.hpp>

namespace ocr {
    class TestClassifier : public IClassifier {
        public:
            virtual ~TestClassifier() noexcept {}
            virtual char classify(cv::Mat &&features) const;
            virtual void serialize(std::string &&dest_path) const;
            virtual char const *name() const;
            virtual void train(Dataset &&dataset_path);
            virtual void unserialize(std::string const &) {}
    };

} /* namespace ocr */
