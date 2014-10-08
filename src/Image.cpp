#include <Image.hpp>

namespace ocr {
    Image::Image(std::string &&image_path, bool lazy)
        : m_img_path(image_path)
    {
        if (not lazy)
            this->load();
    }

    void
    Image::load()
    {
        cv::Mat image = cv::imread(m_img_path, 1);
        if (image.empty()) {
            throw std::runtime_error("coucou");
        }
        m_matrix = image;
    }

    void
    Image::writeImage(std::string &&dest_path)
    {
        std::vector<int> compression_params;
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(9);


        imwrite(dest_path, m_matrix, compression_params);
    }
} // namespace ocr
