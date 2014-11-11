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

#include <utils/lib_loader.hpp>
#include <utils/json/Json.hpp>
#include <utils/NonCopyable.hpp>

namespace ocr {

    class ModuleManager
        : public utils::LibLoader, private utils::NonCopyable
    {
        public:
            ModuleManager() = default;
            ~ModuleManager() = default;
            ModuleManager(ModuleManager const &) = delete;
            ModuleManager &operator=(ModuleManager const &) = delete;

            template<typename T>
            std::shared_ptr<T> load_module(utils::Json::Map const &json_object) {
                using namespace utils;
                auto path = get_item<std::string>(json_object, "path");
                auto args = get_item<std::string>(json_object, "args");
                //auto path = Json::cast<std::string>(json_object["path"]);
                //auto args = JSON_CAST(Map, json_object["args"]);
                auto obj = this->load<T*>(std::move(path), args);
                return std::shared_ptr<T>(obj);
            }

    };
} // namespace ocr
