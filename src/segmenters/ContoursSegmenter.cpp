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


#include "ContoursSegmenter.hpp"


namespace ocr {

    std::vector<Image>
    ContoursSegmenter::apply(Image&& img) const
    {
        std::vector<std::vector<cv::Point> > contours;
        cv::Mat contourOutput = img.getCurrentMatrix().clone();
        cv::findContours(contourOutput,
                         contours,
                         cv::RETR_LIST,
                         cv::CHAIN_APPROX_NONE);

        std::vector<std::vector<cv::Point>> approxCurve(contours.size());
        std::cout << contours.size() << std::endl;
        //cv::approxPolyDP(contourOutput, approxCurve, 3, true);

        std::vector<Image> value(contours.size());
        for (std::size_t i = 0; i < contours.size(); ++i) {
            auto rect = cv::boundingRect(cv::Mat(approxCurve[i]));
            std::cout << i << "\t" << contours.size() << std::endl;
            value[i] = img.subImage(std::move(rect));
            std::cout << "end" << std::endl;
        }
        //for (std::size_t i = 0; i < approxCurve.size(); ++i) {
            //auto rect = cv::boundingRect(cv::Mat(approxCurve[i]));
            //value[i] = img.subImage(std::move(rect));
        //}
        return value;
    }

} /* namespace ocr */


extern "C"
ocr::ContoursSegmenter *constructor()
{
    auto value = new ocr::ContoursSegmenter();
    return value;
}
