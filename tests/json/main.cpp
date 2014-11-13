
#include <cassert>
#include <iostream>
#include <utils/json/Json.hpp>

void
simple_test() {
    utils::Json j;
    std::string test_json("{'a' : 'toto42', 'b': 1}");
    auto i = j.parse(std::move(test_json));
    auto a = utils::Json::cast<utils::Json::Map>(i);

    assert (utils::Json::cast<int>(a["b"]) == 1);
    assert (utils::Json::cast<std::string>(a["a"]) == "toto42");
}

void
recursive_test() {
    utils::Json j;
    std::string test_json("{'a' : 'toto42', 'b': {'foo': 'bar'}}");
    utils::Json::Item i = j.parse(std::move(test_json));
    auto map  = utils::Json::cast<utils::Json::Map>(i);

    assert (utils::Json::cast<std::string>(map["a"]) == "toto42");
    auto nested = utils::Json::cast<utils::Json::Map>(map["b"]);
    assert (utils::Json::cast<std::string>(nested["foo"]) == "bar");
}

void
vector_test() {
    utils::Json j;
    std::string test_json("{'a' : 'toto42', 'b': ['foo', 'bar']}");
    utils::Json::Item i = j.parse(std::move(test_json));
    auto map  = utils::Json::cast<utils::Json::Map>(i);

    assert (utils::Json::cast<std::string>(map["a"]) == "toto42");
    auto vector = utils::Json::cast<utils::Json::Vector>(map["b"]);
    assert (utils::Json::cast<std::string>(vector[0]) == "foo");
    assert (utils::Json::cast<std::string>(vector[1]) == "bar");
}

void test_float() {
    utils::Json j;
    std::string test_json("{'a': 2.0}");
    typedef std::unordered_map<std::string, Any> Map;
    Map map = j.parse(std::move(test_json)).get<Map>();
    std::cout << "end" << std::endl;
    //std::cout << "value\t" << utils::Json::cast<float const>(map["a"]) << std::endl;
    std::cout << &map << std::endl;
    std::cout << "coucou" << std::endl;
    std::cout << map["a"] << std::endl;
    std::cout << "pourquoi" << std::endl;
    assert (utils::Json::cast<float const>(map["a"]) == 2.0);
}

int
main(void)
{
    //simple_test();
    //recursive_test();
    //vector_test();
    test_float();
    return 0;
}
