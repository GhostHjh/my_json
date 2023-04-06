#include <iostream>
#include <thread>
#include "Json.h"

void func_a(int&& a)
{
    std::cout << a << "\n\n\n";
}

void func_b(int&& b)
{
    func_a(std::move(b));
}


int main(int argc, const char** argv)
{
    JSON::Json_value tmp;
    tmp.insert(std::make_pair("a", 10000));
    tmp.insert(std::make_pair("b", 20000));
    tmp.insert(std::make_pair("c", 30000));
    tmp.insert(std::make_pair("d", 40000));
    //tmp.insert("string");
    JSON::Json_value tmp2;
    tmp2.insert(std::make_pair("b", 20000));
    tmp.insert(std::make_pair("e", tmp2));

    //std::cout << "hello world!\n" << tmp << "\n" << tmp["a"] << "\n";
    //JSON::Json m_json("aFalse", JSON::Json_create_mode::String_create);
    //std::cout << JSON::Serialization().parser("  [ 127, \"999999\", True ]  ");
    JSON::Json tmp_json;
    tmp_json.set_value("F:\\cx\\my_json_plus\\build\\compile_commands.json", JSON::Json_create_mode::File_create);
    std::cout << tmp_json.get_value();
    //std::this_thread::sleep_for(std::chrono::seconds(50));
    return 0;
}



