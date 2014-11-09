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
#include <sstream>
#include <typeinfo>
#include <stdexcept>
#include <memory>
#include <type_traits>


class Any {
    private:
        struct IData {
            virtual ~IData() {}
            virtual bool isPointer() const = 0;
            virtual std::type_info const &getTypeInfo() const = 0;
            virtual std::ostream &print(std::ostream &) const = 0;
        };

        template<typename T>
        struct Data : IData {

            T data;

            Data() {
                //std::cout << "Data::Data()" << std::endl;
            }

            Data(Data const &other)
                : data(other.data) {
            //std::cout << "Data::Data(Data const &)" << std::endl;

                }

            Data(T const &v)
                : data(v) {}

            ~Data() {}

            Data &operator=(Data const &other) {
                if (this != &other)
                    data = other.data;
                return *this;
            }

            void operator=(T const &v) {
                data = v;
            }

            bool isPointer() const {
                return std::is_pointer<T>::value;
            }

            std::type_info const &getTypeInfo() const {
                return typeid(T);
            }

            inline std::ostream &print(std::ostream &os) const {
                return os;
            }

        };

    public:
        Any(){
        };

        template<typename T>
        Any(T const &v)
            : _data(new Data<T>(v))
        {
        }

        Any(Any const &other)
            : _data(other._data)
        {
        }

        ~Any() {}

        template<typename T>
        void operator=(T const &v) {
            _data = std::shared_ptr<IData>(new Data<T>(v));
        }

        Any &operator=(Any const &other) {
            if (this != &other)
                _data = other._data;
            return *this;
        }

        bool isPointer() const {
            return _data->isPointer();
        }

        template<typename T>
        bool hasType() const {
            return _data && typeid(T).hash_code() == _data->getTypeInfo().hash_code();
        }

        template<typename T>
        T &get() {
            if ((_data->isPointer() && !std::is_pointer<T>::value) ||
                    !hasType<T>())
                throw std::bad_cast();
            return static_cast<Data<T>*>(_data.get())->data;
        }

        template<typename T>
        operator T &() {
            return get<T>();
        }

        template<typename T>
        T const &get() const {
            if (!_data || (_data->isPointer() && !std::is_pointer<T>::value) ||
                    !hasType<T>())
                throw std::bad_cast();
            return static_cast<Data<T>*>(_data.get())->data;
        }

        template<typename T>
        operator T const &() const {
            return get<T>();
        }

        bool operator!() const {
            auto value = _data == NULL;
            std::cout << _data << "\t " << value << std::endl;
            return value;
        }

        std::shared_ptr<IData> const &
        get_data() const
        {
            return _data;
        }

        char const *
        getTypeName() const
        {
            return _data->getTypeInfo().name();
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

template<typename T>
inline T &
any_cast(Any & operand)
{
    return operand.get<T>();
}

template<typename T>
inline T const &
any_cast(const Any & operand)
{
    return operand.get<T>();
}


inline std::string &&
toString(Any const &any)
{
    std::stringstream s;
    std::string str;
    s << any;
    s >> str;
    return std::move(str);
}
