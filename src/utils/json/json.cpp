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


#include "Json.hpp"
#include <iostream>

#warning "TODO: handle comments"

namespace utils {

    Json::Item
    Json::parse(std::string &&str) {
        int k;
        tokenize(std::move(str));
        auto value = _parse(std::move(_tokens), 0, k);
        return value;
    }

    Json::Item
    Json::load(std::ifstream &is) {
        std::string str;
        std::string tmp;
        while (not is.eof()) {
            std::getline(is, tmp);
            str += tmp;
        }
        return parse(std::move(str));
    }

    Json::Item
    Json::load(std::string &&filename) {
        std::ifstream is(filename);
        if (not is)
            throw std::runtime_error("Cannot open " + filename);
        return load(is);
    }

    Json::Item
    Json::load(std::string const &filename) {
        std::ifstream is(filename);
        if (not is)
            throw std::runtime_error("Cannot open " + filename);
        return load(is);
    }

    void
    Json::_check_string(size_t &k, std::string &&token, char delim) {
        size_t i = k + 1;
        while (i < token.size() and (token[i] != delim)) {
            ++i;
        }
        _tokens.push_back(Token(token.substr(k+1, i-k-1), Token::T_STRING));
        k = i + 1;
    }

    void
    Json::_check_comma(size_t &k, std::string &&) {
        _tokens.push_back(Token(",", Token::T_COMMA));
        ++k;
    }

    void
    Json::_check_false(size_t &k, std::string &&token) {
        std::string tok(k + 4 < token.size() ? token.substr(k, 5) : "");
        if (tok != "false")
            throw std::runtime_error("Json:: unknown token \"" + tok + "\".");
        _tokens.push_back(Token("false", Token::T_BOOLEAN));
        k += 5;
    }

    void
    Json::_check_true(size_t &k, std::string &&token) {
        std::string tok(k + 4 < token.size() ? token.substr(k, 5) : "");
        if (tok != "true")
            throw std::runtime_error("Json:: unknown token \"" + tok + "\".");
        _tokens.push_back(Token("true", Token::T_BOOLEAN));
        k += 4;
    }

    void
    Json::_check_null(size_t &k, std::string &&token) {
        std::string tok(k + 3 < token.size() ? token.substr(k, 3) : "");
        if (tok != "null")
            throw std::runtime_error("Json:: unknown token \"" + tok + "\".");
        _tokens.push_back(Token("null", Token::T_BOOLEAN));
        k += 4;
    }

    void
    Json::_check_right_braces(size_t &k, std::string &&) {
        _tokens.push_back(Token("}", Token::T_RIGHT_BRACE));
        ++k;
    }

    void
    Json::_check_left_braces(size_t &k, std::string &&) {
        _tokens.push_back(Token("{", Token::T_LEFT_BRACE));
        ++k;
    }

    void
    Json::_check_right_bracket(size_t &k, std::string &&)
    {
        _tokens.push_back(Token("]", Token::T_RIGHT_BRACKET));
        k++;
    }

    void
    Json::_check_left_bracket(size_t &k, std::string &&)
    {
        _tokens.push_back(Token("[", Token::T_LEFT_BRACKET));
        k++;
    }

    void
    Json::_check_colon(size_t &k, std::string &&)
    {
        _tokens.push_back(Token(":", Token::T_COLON));
        k++;
    }

    bool
    Json::_check_int(size_t &k, std::string &&token, bool push) {
        size_t i = k;
        bool value = false;

        while (token[i] == '-' or token[i] == '+')
            ++i;
        while (token[i] >= '0' and token[i] <= '9') {
            value = true;
            ++i;
        }
        if (push and value) {
            _tokens.push_back(Token(token.substr(k, i - k), Token::T_INT));
        }
        if (value) {
            k = i;
        }
        return value;
    }

    bool
    Json::_check_float(size_t &t, std::string &&token) {
        size_t i = t;
        bool value = _check_int(i, std::move(token), false);
        if (value && token[i] == '.') {
            ++i;
            while (token[i] >= '0' and token[i] <= '9') {
                ++i;
            }
            _tokens.push_back(Token(token.substr(t, i - t), Token::T_FLOAT));
            t = i;
        } else {
            value = false;
        }
        return value;
    }

    void
    Json::tokenize(std::string &&data) {
        std::string token;
        int next = 0;
        size_t size, k;
        int index;

        _tokens.clear();
        while ((index = _skip_spaces(std::move(data), next)) >= 0) {
            next = _next_space(std::move(data), index);
            token = data.substr(index, next - index);
            size = token.size();
            k = 0;
            while (k < size) {
                switch (token[k]) {
                    case '"': _check_string(k, std::move(token), '"');          break;
                    case "'"[0]: _check_string(k, std::move(token), "'"[0]);    break;
                    case ',': _check_comma(k, std::move(token));                break;
                    case 't': _check_true(k, std::move(token));                 break;
                    case 'f': _check_false(k, std::move(token));                break;
                    case 'n': _check_null(k, std::move(token));                 break;
                    case ':': _check_colon(k, std::move(token));                break;
                    case '{': _check_left_braces(k, std::move(token));          break;
                    case '}': _check_right_braces(k, std::move(token));         break;
                    case '[': _check_left_bracket(k, std::move(token));         break;
                    case ']': _check_right_bracket(k, std::move(token));        break;
                    default:
                        if (not _check_float(k, std::move(token)) and not _check_int(k, std::move(token)))
                            throw std::runtime_error("Json: unknown token: [" + token.substr(k) + "]");
                        break;

                }

            }
        }
   }

    inline bool
    _is_space(char c) {
        return (c == ' ' or c == '\t' or c == '\n');
    }

    size_t
    Json::_skip_spaces(std::string &&data, int i) const {
        int size = data.size();
        while (i < size) {
            if (not _is_space(data[i])) {
                return i;
            }
            ++i;
        }
        return size_t(-1);
    }

    size_t
    Json::_next_space(std::string &&data, int i) const {
        int size = data.size();

        while (i <  size)
        {
            if (data[i] == '"')
            {
                ++i;
                while (i < size && (data[i] != '"' || data[i-1] == '\\'))
                    ++i;
            }
            if (data[i] == '\'')
            {
                ++i;
                while (i < size && (data[i] != '\'' || data[i-1] == '\\'))
                    ++i;
            }
            if (_is_space(data[i]))
                return i;
            ++i;
        }
        return size;

    }

    Json::Item
    Json::parse_bool(std::string &&s) const {
        return Json::Item(s == "true" ? true : false);
    }

    Json::Item
    Json::parse_int(std::string &&s) const {
        return Json::Item(std::stoi(s));
    }

    Json::Item
    Json::parse_float(std::string &&s) const {
        return Json::Item(std::stof(s));
    }

    Json::Item
    Json::_parse(std::vector<Token> &&v, int i, int &r) const {
        Json::Item current;
        int j, k;

        switch (v[i].type) {
            {
                case Token::T_LEFT_BRACE:
                    auto tmp = std::unordered_map<std::string, Json::Item>();
                    k = i + 1;
                    while (v[k].type != Token::T_RIGHT_BRACE) {
                        std::string key = v[k].value;
                        k += 2;
                        j = k;
                        Json::Item vv = _parse(std::move(v), k, j);
                        tmp[key] = vv;
                        k = j;
                        if (v[k].type == Token::T_COMMA)
                            ++k;
                    }
                    r = k + 1;
                    current = tmp;
                    break;
            }

            {
                case Token::T_LEFT_BRACKET:
                    auto tmp = std::vector<Json::Item>();
                    k = i + 1;
                    while (v[k].type != Token::T_RIGHT_BRACKET) {
                        j = k;
                        Json::Item vv = _parse(std::move(v), k, j);
                        tmp.push_back(vv);
                        k = j;
                        if (v[k].type == Token::T_COMMA)
                            ++k;
                    }
                    r = k + 1;
                    current = tmp;
                    break;
            }

            {
                case Token::T_STRING:
                    current = v[i].value;
                    r = i + 1;
                    break;
            }

            {
                case Token::T_BOOLEAN:
                    current = parse_bool(std::move(v[i].value));
                    r = i + 1;
                    break;
            }

            {
                case Token::T_NULL:
                    current = NULL;
                    r = i + 1;
                    break;
            }

            {
                case Token::T_UNDEFINDED:
                    current = std::string("undefined");
                    r = i + 1;
                    break;
            }
            {

                case Token::T_INT:
                    current = parse_int(std::move(v[i].value));
                    r = i + 1;
                    break;
            }
            {
                case Token::T_FLOAT:
                    current = parse_float(std::move(v[i].value));
                    r = i + 1;
                    break;
            }
            default:
                break;
        }
        return current;
    }

} /* namespace utils */
