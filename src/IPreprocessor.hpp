#pragma once

namespace ocr {

    class IPreprocessor {
        public:
            virtual ~IPreprocessor() noexcept {}
            virtual void apply(Image &) const = 0;
    }
} // namespace Ocr
