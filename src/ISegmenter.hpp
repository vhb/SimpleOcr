#pragma once

#include <Word.hpp>

namespace ocr {
    class ISegmenter {
        public:
            virtual ~ISegmenter() noexcept {}
            virtual std::vector<Word> &&apply() const = 0;
    };
} /* namespace ocr */
