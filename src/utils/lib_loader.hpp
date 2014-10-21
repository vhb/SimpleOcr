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

#ifdef __APPLE__
#define EXT std::string(".dylib")
#else
#define EXT std::string(".so")
#endif


#include <memory>
#include <vector>
#include <iostream>
#include <utility>
#include <dlfcn.h>
#include <string>

namespace utils {
    class LibLoader {
        public:
            template <typename InstanceType, typename... Params>
            InstanceType &&load(std::string &&lib_path, Params... p) {
                typedef InstanceType (*Jumper)(Params...);
                Jumper l = reinterpret_cast<Jumper>(
                        m_internal.load(std::move(lib_path))
                        );
                auto &&value = l(p...);
                return std::move(value);
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
                    Handle value = dlopen((lib_path + EXT).c_str(), RTLD_LAZY);
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
