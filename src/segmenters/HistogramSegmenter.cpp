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
    ssize_t
    HistogramSegmenter::apply(Image &&img) const
    {
        cv::Mat mat = img.getCurrentMatrix();
        cv::Mat horizontal(mat.cols, 1, CV_32S);
        horizontal = cv::Scalar::all(0);
        cv::Mat vertical(mat.rows, 1, CV_32S);
        vertical = cv::Scalar::all(0);

        // je dois passer sur chqaue colone,
        //  compter le nombre de pixels qui ne sont pas a 0
        for(int row = 0; row < mat.rows; ++row) {
            auto item = cv::countNonZero(mat(cv::Rect(0, row, mat.cols, 1)));
            horizontal.at<int>(row, 0) = item;
        }

        detectPic(std::move(horizontal));
        /*plotHistogram(std::move(horizontal));*/
        return 0;
    }

    char const *
    HistogramSegmenter::name() const
    {
        return "HistogramSegmenter";
    }

    void
    HistogramSegmenter::plotHistogram(cv::Mat &&mat,
                                      std::string &&dest_path) const
    {
        int hist_w = 512;
        int hist_h = 700;
        double bin_w = double(hist_w) / double(mat.cols);
        std::cout << bin_w << std::endl;

        cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

        for (int i = 1; i < mat.rows; i++) {
            auto origin = cv::Point(bin_w * (i - 1),
                                    hist_h - cvRound(mat.at<int>(i - 1, 0)));
            auto dest = cv::Point(bin_w * i,
                                  hist_h - cvRound(mat.at<int>(i, 0)));
            cv::line(histImage, origin, dest,
                     cv::Scalar(255, 0, 0), 2, 8, 0
                     );
        }
        if (dest_path.empty()) {
            cv::namedWindow("calcHist Demo", cv::WINDOW_AUTOSIZE );
            cv::imshow("calcHist Demo", histImage );
            cv::waitKey(0);
        }
        else {
            cv::imwrite("/tmp/" + dest_path, histImage );
        }
    }

    std::vector<cv::Point>
    HistogramSegmenter::detectPic(cv::Mat &&vertical,
                                  cv::Mat &&horizontal) const
    {
        // TODO: vertical and horizontal have the same size ?
        auto value = std::vector<cv::Point>();
        auto augmentation = false;
        auto decrementation = false;
        int derivate_floor = 3;

        for (int i = 1; i < vertical.rows; i++) {
            bool change = false;
            auto tmp = vertical.at<int>(i - 1, 0);
            auto tmp2 = vertical.at<int>(i, 0);
            auto derivate = tmp2 - tmp;
            if (derivate > derivate_floor) {
                augmentation = true;
                decrementation = false;
                change = true;
            }
            else if (derivate < -derivate_floor) {
                augmentation = false;
                decrementation = true;
                change = true;
            }
            if (change) {
                std::cout << augmentation << "\t" << decrementation
                          << "\t " << derivate << std::endl;
            }
        }

        return value;
    }

} /* namespace ocr */


extern "C"
ocr::HistogramSegmenter *constructor() {
    return new ocr::HistogramSegmenter;
}
