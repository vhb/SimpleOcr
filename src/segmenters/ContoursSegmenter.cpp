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

    std::vector<cv::Rect>
    ContoursSegmenter::apply(Image&& img) const
    {
        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Rect> value;
        cv::Mat m = img.getCurrentMatrix();
        cv::Mat contourOutput = m.clone();
        cv::findContours(contourOutput,
                         contours,
                         cv::RETR_EXTERNAL,
                         cv::CHAIN_APPROX_SIMPLE,
                         cv::Point(0, 0));

        std::vector<std::vector<cv::Point>> approxCurve(contours.size());
        std::size_t i;
        int xmin, ymin, xmax, ymax;
        auto color = cv::Scalar(255, 255, 255);

        for (i = 0; i < contours.size();  ++i) {
            cv::approxPolyDP(contours[i], approxCurve[i], 1, true);
            auto rect = cv::boundingRect(cv::Mat(contours[i]));
            value.push_back(rect);
            cv::Point pt1, pt2;
            pt1.x = rect.x;
            pt2.x = rect.x + rect.width;
            pt1.y = rect.y;
            pt2.y = rect.y + rect.height;
            //if (not merge) {
                cv::rectangle(contourOutput, pt1, pt2, color, 1, 8, 0);
                img.addSubMatrix(std::move(rect));
            //}
            //else {
                //if (i == 0) {
                    //xmin = std::min(pt1.x, pt2.x);
                    //ymin = std::min(pt1.y, pt2.y);
                    //xmax = std::max(pt1.x, pt2.x);
                    //ymax = std::max(pt1.y, pt2.y);
                //}
                //else {
                    //xmin = std::min(xmin, std::min(pt1.x, pt2.x));
                    //ymin = std::min(ymin, std::min(pt1.y, pt2.y));
                    //xmax = std::max(xmax, std::max(pt1.x, pt2.x));
                    //ymax = std::max(ymax, std::max(pt1.y, pt2.y));
                //}
            //}
        }
        //if (merge) {
            //cv::Point pt1(xmin, ymin), pt2(xmax, ymax);
            //cv::rectangle(contourOutput, pt1, pt2, color, 1, 8, 0);
            //auto rect = cv::Rect(pt1, pt2);
            //img.addSubMatrix(std::move(rect));
        //}
        img.setMatrix("contourOutput", std::move(contourOutput));
        return value;
        //return i;
    }

    char const *
    ContoursSegmenter::name() const
    {
        return "ContoursSegmenter";
    }

} /* namespace ocr */


extern "C"
ocr::ContoursSegmenter *constructor()
{
    auto value = new ocr::ContoursSegmenter();
    return value;
}
