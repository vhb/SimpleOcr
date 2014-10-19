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


#include <preprocessors/HueThreshold.hpp>

namespace ocr {
    HueThreshold::~HueThreshold() noexcept
    {
    }

    void
    HueThreshold::apply(Image &img) const
    {
        auto &tmp = img.getCurrentMatrix();
        auto value = tmp.clone();
        cv::Scalar avg, avgStd;
        cv::meanStdDev(tmp, avg, avgStd);
        int thresh = (int)avg[0]-7*(int)(avgStd[0]/8);
        cv::threshold(tmp, value, thresh, 255, cv::THRESH_BINARY_INV);
        img.setMatrix("Threshold", std::move(value));
        // Dans l'ordre je load mes images sous differents formats
        //
        // smooth
        // je calcule average value and standard deviation of array elements
        // J'applique un threshold avec pour paramettres : (int)avg.val[0]-7*(int)(avgStd.val[0]/8),
        // 255, CV_THRESH_BINARY_INV
        // Erode puis dilate
        // Ensuite je fais un findContours
        // puis un approxPolyDP pour optimiser les contours
        // Ensuite regarder pour les moments au niveau de la feature extraction
    }
} /* namespace ocr */

extern "C" {
    ocr::HueThreshold *constructor() {
        return new ocr::HueThreshold();
    }
}

