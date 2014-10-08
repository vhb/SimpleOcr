#pragma once

namespace ocr {
    class IFeatureExtractor {
        public:
            virtual ~IFeatureExtractor() noexcept {}
            virtual cv::Mat extract(Image &&img) const = 0;

    };
}
