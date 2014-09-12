#pragma once

namespace ocr {
    class IFeatureExtractor {
        public:
            virtual ~IFeatureExctractor() noexcept {}
            virtual cv::Mat extract(Image &&img) const = 0;

    };
}
