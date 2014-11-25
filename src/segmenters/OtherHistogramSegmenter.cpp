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
    std::vector<cv::Rect>
    HistogramSegmenter::apply(Image &&img) const
    {
        cv::Mat mat = img.getCurrentMatrix();
        std::vector<cv::Rect> value;
        cv::Mat horizontal(mat.rows, 1, CV_32S);
        horizontal = cv::Scalar::all(0);
        cv::Mat vertical(mat.cols, 1, CV_32S);
        vertical = cv::Scalar::all(0);

        for (int cols = 0; cols < mat.cols; ++cols) {
            // cv::Rect(0, row, mat.cols, 1)
            auto item = cv::countNonZero(mat(cv::Rect(cols, 0, 1, mat.rows)));
            horizontal.at<int>(0, cols) = item;
        }

        auto color = cv::Scalar(255, 255, 255);
        auto hor_pic = detectPic(std::move(horizontal));
        for (auto & i : hor_pic) {
            auto br = cv::Rect(i.first, 0, i.second - i.first, mat.rows);
            auto subMatrix = mat(br).clone();
            for (int row = 1; row < subMatrix.rows; ++row) {
                auto line = subMatrix(cv::Range(row, row + 1), cv::Range::all());
                auto item = cv::countNonZero(line);
                vertical.at<int>(0, row) = item;
            }
            auto vert_pic = detectPic(std::move(vertical));
            for (auto &j: vert_pic) {
                auto pt1 = cv::Point(j.first, i.first);
                auto pt2 = cv::Point(j.second, i.second);
                cv::rectangle(mat, pt1, pt2, color, 1, 8, 0);
                auto rect = cv::Rect(pt1, pt2);
                value.push_back(rect);
                img.addSubMatrix(std::move(rect));
            }
        }
        img.setMatrix(name(), std::move(mat));
        return value;
    }

    char const *
    HistogramSegmenter::name() const
    {
        return "OtherHistogramSegmenter";
    }

    void
    HistogramSegmenter::plotHistogram(cv::Mat &&mat,
                                      std::string &&dest_path) const
    {
        int hist_w = 512;
        int hist_h = 700;
        double bin_w = double(hist_w) / double(mat.cols);
        /*std::cout << bin_w << std::endl;*/

        cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

        for (int i = 1; i < mat.rows; i++) {
            auto origin = cv::Point(bin_w * (i - 1),
                                    hist_h - cvRound(mat.at<int>(i - 1, 0)));
            auto dest = cv::Point(bin_w * i,
                                  hist_h - cvRound(mat.at<int>(i, 0)));
            cv::line(histImage, origin, dest,
                     cv::Scalar(255, 0, 0),
                     2, 8, 0
                     );
        }
        if (dest_path.empty()) {
            cv::namedWindow("calcHist Demo", cv::WINDOW_AUTOSIZE);
            cv::imshow("calcHist Demo", histImage);
            cv::waitKey(0);
        }
        else {
            cv::imwrite("/tmp/" + dest_path, histImage );
        }
    }

    std::vector<std::pair<int, int>>
    HistogramSegmenter::detectPic(cv::Mat &&vertical) const
    {
        auto value = std::vector<std::pair<int, int>>();
        int activation_floor = 0;
        int begin_index = -1;
        int begin_value = 0;

        for (int i = 0; i < vertical.rows; i++) {
            auto current_value = vertical.at<int>(i, 0);
            if (begin_index < 0) {
                if (current_value > activation_floor) {
                    begin_index = i;
                    begin_value = current_value;
                }
            }
            else {
                if (current_value < begin_value) {
                    value.push_back(std::pair<int, int>(begin_index, i));
                    begin_index = -1;
                    begin_value = -1;
                }
            }
        }

        return value;
    }

} /* namespace ocr */


extern "C"
ocr::HistogramSegmenter *constructor() {
    return new ocr::HistogramSegmenter;
}


            /*if (derivate > derivate_floor) {*/
                /*augmentation = true;*/
                /*decrementation = false;*/
                /*change = true;*/
            /*}*/
            /*else if (derivate < -derivate_floor) {*/
                /*augmentation = false;*/
                /*decrementation = true;*/
                /*change = true;*/
            /*}*/
            /*if (change) {*/
                /*std::cout << augmentation << "\t" << decrementation*/
                          /*<< "\t " << derivate << std::endl;*/
            /*}*/

