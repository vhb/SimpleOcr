#pragma once

#include <memory>
#include <vector>
#include <utility>
#include <dlfcn.h>
#include <string>

namespace utils {
    class LibLoader {
        public:
            template <typename InstanceType, typename... Params>
            InstanceType load(std::string &&lib_path, Params... p) {
                typedef InstanceType (*Jumper)(Params...);
                Jumper l = reinterpret_cast<Jumper>(m_internal.load(std::move(lib_path)));
                return l(p...);
            }

        private:
            struct Internal {
                typedef void * Handle;

                Internal() = default;
                ~Internal() {
                    for (auto i : m_handles) {
                        realease_lib(i);
                    }
                }

                Internal(Internal const &) = delete;
                Internal(Internal &&) = delete;
                Internal &operator=(Internal &&) = delete;

                void
                realease_lib(Handle h) {
                    dlclose(h);
                }

#warning "TODO: Handle .so/.dll"

                void *
                load(std::string &&lib_path) {
                    Handle value = value = dlopen((lib_path + std::string(".dylib")).c_str(), RTLD_LAZY);
                    if (not value)
                        throw std::runtime_error(dlerror());
                    m_handles.push_back(value);
                    Handle func_ptr = dlsym(value, "constructor");
                    if (not func_ptr)
                        throw std::runtime_error(dlerror());
                    return func_ptr;
                }

            private:
                std::vector<Handle> m_handles;
            };

            Internal m_internal;
    };

} // namespace utils
