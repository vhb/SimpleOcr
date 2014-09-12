#pragma once

namespace ocr {
    class IClassifier {
        public:
            virtual ~IClassifier() noexcept {}
            virtual std::string &&classify() const = 0;
            virtual bool train(Dataset &&) = 0;
    };
}
