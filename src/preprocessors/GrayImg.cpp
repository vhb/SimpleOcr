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

#include "GrayImg.hpp"
#include <opencv2/imgproc/imgproc.hpp>


namespace ocr {

    void
    GrayImg::apply(Image &img) const
    {
        cv::Mat grayImg = img.getCurrentMatrix().clone();
        cv::cvtColor(grayImg, grayImg, cv::COLOR_BGR2GRAY);
        img.setMatrix("GrayImg", std::move(grayImg));
    }

    cv::Mat
    GrayImg::apply(cv::Mat const &mat) const
    {
        cv::Mat grayImg = mat.clone();
        cv::cvtColor(grayImg, grayImg, cv::COLOR_BGR2GRAY);
        return grayImg;
    }

    char const *
    GrayImg::name() const
    {
        return "GrayImg";
    }

} // namespace ocr

extern "C"
ocr::GrayImg *constructor()
{
    return new ocr::GrayImg();
}
