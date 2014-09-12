#pragma once

namespace ocr {
    class ISegmenter {
        virtual ~ISegmenter() noexcept {}
        virtual std::vector<Word> &&apply() const = 0;
    };
} /* namespace ocr */
