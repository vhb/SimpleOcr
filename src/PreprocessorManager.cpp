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


#include <PreprocessorManager.hpp>
#include <utils/json/Json.hpp>
#include <stdexcept>

namespace ocr {

    void
    PreprocessorManager::load_preprocessor(std::vector<utils::Json::Item> const &pl)
    {
        for (auto const &i : pl) {
            auto const &item = JSON_CAST(Map const, i);
            auto tmp = this->load_module<IPreprocessor>(item);
            m_preprocessors.push_back(tmp);
        }
    }

    bool
    PreprocessorManager::apply(Image &img) const
    {
        std::cout << "Preprocessing" << std::endl;
        for (auto const &i : m_preprocessors) {
            std::cout << "\t Applying: " << i->name() << std::endl;
            i->apply(img);
        }
        return true;
    }

    cv::Mat
    PreprocessorManager::apply(cv::Mat const &mat) const
    {
        cv::Mat tmp = mat;
        for (auto const &i : m_preprocessors) {
            tmp = i->apply(tmp);
        }
        return tmp;
    }

    PreprocessorManager::PreprocessorList const &
    PreprocessorManager::get_preprocessors() const
    {
        return m_preprocessors;
    }

    std::shared_ptr<IPreprocessor> const &
    PreprocessorManager::get_preprocessor(std::string &&name) const
    {
        return *std::find_if(m_preprocessors.begin(), m_preprocessors.end(),
                         [&] (std::shared_ptr<IPreprocessor> const &it) {
                            return it->name() == name;
                         });
    }

} // namespace ocr
