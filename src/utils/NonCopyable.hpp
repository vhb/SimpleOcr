#pragma once

namespace utils {
    class NonCopyable {
        public:
            NonCopyable() = default;
            ~NonCopyable() = default;
            NonCopyable(NonCopyable const &) = delete;
            NonCopyable(NonCopyable &&) = delete;

            NonCopyable &operator=(NonCopyable const &) = delete;
            NonCopyable &operator=(NonCopyable &&) = delete;
    };
}
