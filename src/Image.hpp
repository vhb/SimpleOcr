#pragma once

#include <opencv2/opencv.hpp>

#include <utils/NonCopyable.hpp>

namespace ocr {

#warning "TODO: implement image"
    class Image
    {
        private:
            Image(std::string &&image_path, bool lazy = false);

            void writeImage(std::string &&dest_path);

        private:
            void load();

            std::string m_img_path;
            cv::Mat m_matrix;
    };
} // namespace ocr
