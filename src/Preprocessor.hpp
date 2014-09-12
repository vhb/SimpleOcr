#pragma once

namespace ocr {


    class IPreprocessor {
        public:
            virtual ~IPreprocessor() noexcept {}
            virtual void apply() const = 0;
    }

    class PreprocessorManager {
        public:
            typedef std::vector<std::shared_ptr<IPreprocessor>> PreProcessorList

            PreprocessorManager(std::vector<std::string> &&preprocessor_list);
            ~PreprocessorManager() noexcept;

            bool apply(Image &&img) const;
            ProcessorList get_preprocessors() const;
            std::shared_ptr<IPreprocessor> &&get_preprocessor(std::string &&name);

        private:
            PreProcessorList m_proprocessors;
    };
}
