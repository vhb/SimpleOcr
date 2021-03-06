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

#include <preprocessors/Smooth.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace ocr {
    void
    Smooth::apply(Image &img) const
    {
        cv::Mat &mat = img.getCurrentMatrix();
        img.setMatrix("Smooth", std::move(apply(mat)));
    }

    cv::Mat
    Smooth::apply(cv::Mat const &mat) const
    {
        cv::Mat dest = mat.clone();
        cv::Size blurKernel(3, 3);
        cv::blur(mat, dest, blurKernel, cv::Point(-1, -1));
        return dest;
    }

    char const *
    Smooth::name() const
    {
        return "Smooth";
    }

} /* namespace ocr */

extern "C" {
    ocr::Smooth* constructor() {
        return new ocr::Smooth();
    }
}
