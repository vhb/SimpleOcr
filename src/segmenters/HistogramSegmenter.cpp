// Simple ocrk
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


#include "HistogramSegmenter.hpp"

namespace ocr {
    std::vector<Image>
    HistogramSegmenter::apply(Image &&img) const
    {
        cv::Mat mat = img.getCurrentMatrix();


        cv::Mat horizontal(mat.cols, 1, CV_32S);
        horizontal = cv::Scalar::all(0);
        cv::Mat vertical(mat.rows, 1, CV_32S);
        vertical = cv::Scalar::all(0);
    }

    char const *
    HistogramSegmenter::name() const
    {
        return "HistogramSegmenter";
    }
} /* namespace ocr */
