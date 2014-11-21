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

#include "ScaleDown.hpp"

namespace ocr {

    ScaleDown::ScaleDown() noexcept {}

    ScaleDown::~ScaleDown() noexcept {}

    cv::Mat
    ScaleDown::extract(Image const &img, int index) const {
        cv::Mat const & data = img.getSubMatrix(index);
        return extract(data);
    }

    inline std::string type2str(int type) {
        std::string r;

        uchar depth = type & CV_MAT_DEPTH_MASK;
        uchar chans = 1 + (type >> CV_CN_SHIFT);

        switch ( depth ) {
            case CV_8U:  r = "8U"; break;
            case CV_8S:  r = "8S"; break;
            case CV_16U: r = "16U"; break;
            case CV_16S: r = "16S"; break;
            case CV_32S: r = "32S"; break;
            case CV_32F: r = "32F"; break;
            case CV_64F: r = "64F"; break;
            default:     r = "User"; break;
        }

        r += "C";
        r += (chans+'0');

        return r;
    }


    cv::Mat
    ScaleDown::extract(cv::Mat const &m) const {
        cv::Size size(width, height);
        cv::Mat dest(width, height, CV_32F);
        //std::cout << type2str(dest.type());
        cv::resize(m, dest, size);
        cv::Mat value;
        dest.convertTo(value, CV_32F);
        dest = value;
        //std::cout << dest << std::endl;
        //std::cout << type2str(m.type()) << std::endl;
        dest = dest.reshape(0, 1);
        return dest;
    }

    int
    ScaleDown::nb_features() const {
        return width * height;
    }

    char const *
    ScaleDown::name() const
    {
        return "ScaleDown";
    }

}

    extern "C"
ocr::ScaleDown *constructor()
{
    return new ocr::ScaleDown();
}
