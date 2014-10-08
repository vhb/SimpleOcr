#pragma once

#include <opencv2/core.hpp>

namespace ocr {

    // implement a range enumerable dataset
    class Dataset {
        public:
            typedef std::tuple<std::string, cv::Mat> Data;

            // Load the data described in `json_path`
            Dataset(std::string &&json_path);
            ~Dataset() noexcept;

            std::vector<Data> const &get_datas() const;
            std::string const &get_json_path() const;

        private:
            std::string m_json_path;
            std::vector<Data> m_datas;
    };
}
