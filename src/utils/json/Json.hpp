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

//#include <boost/any.hpp>

#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include "any.hpp"

#define JSON_CAST(TYPE, OBJ) utils::Json::cast<utils::Json::TYPE>(OBJ)

namespace utils {

    class Json {
        public:
            template <typename TypeTo, typename... Args>
            static TypeTo & cast(Args&&... args) // -> decltype(any_cast<TypeTo>(std::forward<Args>(args)...))
            {
                return any_cast<TypeTo>(std::forward<Args>(args)...);
            }

            typedef Any Item;
            typedef std::unordered_map<std::string, Item> Map;
            typedef std::vector<Item> Vector;
            typedef int Int;
            typedef float Float;
            typedef std::string String;

            class Token
            {
                public:
                    enum Type {
                        T_UNDEFINDED,
                        T_STRING,
                        T_INT,
                        T_FLOAT,
                        T_LEFT_BRACE,
                        T_RIGHT_BRACE,
                        T_RIGHT_BRACKET,
                        T_LEFT_BRACKET,
                        T_COLON,
                        T_COMMA,
                        T_BOOLEAN,
                        T_NULL
                    };

                    Token(std::string &&value, Type type)
                        : value(value), type(type)
                    {
                    }

                    std::string value;
                    Type type;
            };

            Item parse(std::string &&str);
            Item load(std::string &&filename);
            Item load(std::string const &filename);
            Item load(std::ifstream &is);

        private:
            Item m_content;

            void tokenize(std::string &&data);
            Item _parse(std::vector<Token> &&tokens, int i, int &r) const;

            void _check_string(size_t &k, std::string &&token, char delim);
            void _check_comma(size_t &k, std::string &&s);
            void _check_false(size_t &k, std::string &&s);
            void _check_true(size_t &k, std::string &&s);
            void _check_null(size_t &k, std::string &&s);
            void _check_right_braces(size_t &k, std::string &&);
            void _check_left_braces(size_t &k, std::string &&);
            void _check_right_bracket(size_t &k, std::string &&);
            void _check_left_bracket(size_t &k, std::string &&);
            void _check_colon(size_t &k, std::string &&);
            bool _check_int(size_t &t, std::string &&token, bool push = true);
            bool _check_float(size_t &t, std::string &&token);
            size_t _skip_spaces(std::string &&data, int i) const;
            size_t _next_space(std::string &&data, int i) const;

            Item parse_bool(std::string &&s) const;
            Item parse_int(std::string &&s) const;
            Item parse_float(std::string &&s) const;

            std::vector<Token> _tokens;
    };

    inline std::ostream &
    operator<<(std::ostream &is, Json::Token::Type t)
    {
        static char const * v[] = { "T_UNDEFINDED", "T_STRING",
            "T_INT", "T_FLOAT", "T_LEFT_BRACE", "T_RIGHT_BRACE", "T_RIGHT_BRACKET",
            "T_LEFT_BRACKET", "T_COLON", "T_COMMA", "T_BOOLEAN", "T_NULL" };
        std::string value = v[static_cast<int>(t)];
        is << value;
        return is;
    }

    template <typename T>
    inline T &
    get_item(utils::Json::Map &j, std::string const &s)
    {
        //utils::Json::Map &m = any_cast<utils::Json::Map>(j);
        utils::Json::Map::iterator i = j.find(s);
        if (i == j.end()) {
            throw std::runtime_error("Invalid key: [" + s + std::string("]"));
        }
        return any_cast<T>(i->second);
    }

    template <typename T>
    inline T const &
    get_item(utils::Json::Map const &j, std::string const &s)
    {
        //utils::Json::Map const &m = j; // any_cast<utils::Json::Map const>(j);
        j.find(s);
        utils::Json::Map::const_iterator i = j.find(s);
        if (i == j.end()) {
            throw std::runtime_error("Invalid key: [" + s + std::string("]"));
        }
        return any_cast<T const>(i->second);
    }

} /* namespace utils */
