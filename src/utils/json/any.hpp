// A generic json parser
// Copyright (C) 2014 Vhb
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

#include <ostream>
#include <typeinfo>
#include <stdexcept>
#include <memory>
#include <type_traits>

class Any {
    private:

        /**
         * @class IData
         * Simple interface to store Data without template
         * parameters.
         */
        struct IData {
            virtual ~IData() {}
            virtual bool isPointer() const = 0;
            virtual std::type_info const &getTypeInfo() const = 0;
            virtual std::ostream &print(std::ostream &) const = 0;
        };

        /**
         * @class Data
         * Simple data structure to hide the type of the data.
         */
        template<typename T>
            struct Data : IData {

                /**
                 * The real data.
                 */
                T data;

                /**
                 * Default empty constructor.
                 */
                Data() {}

                /**
                 * Copy constructor.
                 */
                Data(Data const &other)
                    : data(other.data) {}

                /**
                 * Construct from value.
                 */
                Data(T const &v)
                    : data(v) {}

                /**
                 * Empty destructor.
                 */
                ~Data() {}

                /**
                 * Assignement operator.
                 */
                Data &operator=(Data const &other) {
                    if (this != &other)
                        data = other.data;
                    return *this;
                }

                /**
                 * Assign from value.
                 */
                void operator=(T const &v) {
                    data = v;
                }

                /**
                 * Check if the data is a pointer or not.
                 */
                bool isPointer() const {
                    return std::is_pointer<T>::value;
                }

                /**
                 * Return the type_info structure for this data.
                 */
                std::type_info const &getTypeInfo() const {
                    return typeid(T);
                }

                /**
                 * Print this data with the given stream.
                 */
                inline std::ostream &print(std::ostream &os) const {
                    return os;
                }

            };

    public:

        /**
         * Empty default constructor.
         */
        Any() {}

        /**
         * Construct from value.
         */
        template<typename T>
            Any(T const &v)
            : _data(new Data<T>(v)) {}

        /**
         * Copy constructor.
         */
        Any(Any const &other)
            : _data(other._data) {}

        /**
         * Empty destructor.
         */
        ~Any() {}

        /**
         * Assign the given value.
         */
        template<typename T>
            void operator=(T const &v) {
                _data = std::shared_ptr<IData>(new Data<T>(v));
            }

        /**
         * Assignement operator.
         */
        Any &operator=(Any const &other) {
            if (this != &other)
                _data = other._data;
            return *this;
        }

        /**
         * Check if the data is a pointer.
         */
        bool isPointer() const {
            return _data->isPointer();
        }

        /**
         * Check if the type of this any has type `T`.
         */
        template<typename T>
            bool hasType() const {
                return _data && typeid(T).hash_code() == _data->getTypeInfo().hash_code();
            }

        /**
         * Return the data.
         */
        template<typename T>
            T &get() {
                if ((_data->isPointer() && !std::is_pointer<T>::value) ||
                        !hasType<T>())
                    throw std::bad_cast();
                return static_cast<Data<T>*>(_data.get())->data;
            }

        /**
         * Return the data.
         */
        template<typename T>
            operator T &() {
                return get<T>();
            }

        /**
         * Return the data.
         */
        template<typename T>
            T const &get() const {
                if (!_data ||
                        (_data->isPointer() && !std::is_pointer<T>::value) ||
                        !hasType<T>())
                    throw std::bad_cast();
                return static_cast<Data<T>*>(_data.get())->data;
            }

        /**
         * Return the data.
         */
        template<typename T>
            operator T const &() const {
                return get<T>();
            }

        /**
         * Check if the any contains any data.
         */
        bool operator!() const {
            return _data != NULL;
        }

    private:
        friend std::ostream &operator<<(std::ostream &os, Any const &v) {
            if (v._data)
                return v._data->print(os);
            os << "undefined";
            return os;
        }

        std::shared_ptr<IData> _data;

};

#define GENERATE_STREAM_OPERATOR(type) \
    template<> \
inline std::ostream &Any::Data<type>::print(std::ostream &os) const { \
    return os << data; \
}

    GENERATE_STREAM_OPERATOR(char)
    GENERATE_STREAM_OPERATOR(unsigned char)
    GENERATE_STREAM_OPERATOR(int)
    GENERATE_STREAM_OPERATOR(size_t)
    GENERATE_STREAM_OPERATOR(float)
    GENERATE_STREAM_OPERATOR(double)
    GENERATE_STREAM_OPERATOR(char *)
    GENERATE_STREAM_OPERATOR(std::streambuf *)

#undef GENERATE_DEFAULT_STREAM_OPERATOR

inline std::string &&toString(Any const &any) {
    std::stringstream s;
    std::string str;
    s << any;
    s >> str;
    return std::move(str);
}
