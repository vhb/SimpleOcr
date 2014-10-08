#pragma once

#include <utils/lib_loader.hpp>
#include <utils/json/json.hpp>
#include <utils/NonCopyable.hpp>

namespace ocr {

    class ModuleManager : public utils::LibLoader, private utils::NonCopyable {

        public:
            ModuleManager() = default;
            ~ModuleManager() = default;

            ModuleManager(ModuleManager const &) = delete;
            ModuleManager &operator=(ModuleManager const &) = delete;

            template<typename T>
            std::shared_ptr<T> &&load_module(utils::Json::Map &&json_object) {
                using namespace utils;
                auto path = Json::cast<std::string>(json_object["path"]);
                auto args = JSON_CAST(Map, json_object["args"]);
                auto obj = this->load<T*>(std::move(path), args);
                return std::move(std::shared_ptr<T>(obj));
            }

    };
} // namespace ocr
