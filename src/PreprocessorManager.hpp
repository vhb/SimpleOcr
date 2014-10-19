// Simple OCR program
// Copyright (C) 2014 vhb
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#pragma once

#include <IPreprocessor.hpp>
#include <ModuleManager.hpp>
#include <utils/json/json.hpp>

namespace ocr {

    class PreprocessorManager : private ModuleManager {
        public:
            typedef std::vector<std::shared_ptr<IPreprocessor>> PreprocessorList;

            // In fact except a std::vector<std::string>
            void load_preprocessor(std::vector<utils::Json::Item> &&preprocessor_list);

            bool apply(Image &&img) const;
            PreprocessorList get_preprocessors() const;
            std::shared_ptr<IPreprocessor *> &&get_preprocessor(std::string &&name);

        private:
            PreprocessorList m_preprocessors;
    };
}
