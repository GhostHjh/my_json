#include <iostream>
#include "json.hpp"

using std::cout;
using std::endl;


int main(int argc, char** argv)
{
    // json tmp_json(json::type_t::json_int);
    // tmp_json = 1;

    json tmp_json_2(json::type_t::json_vector);
    tmp_json_2.push_back("asdasdasdasdasdasd");
    tmp_json_2.push_back(1);

    // tmp_json_2.push_back(tmp_json);
    // tmp_json_2[3] = "bbbbbb";

    // json tmp_json_3(json::type_t::json_obj);
    // tmp_json_3["111"] = "str11111";
    // tmp_json_3["222"] = "str22222";
    // tmp_json_3["333"] = "str33333";
    // tmp_json_3["444"] = true;
    // tmp_json_3["555"] = false;
    // tmp_json_3["666"] = tmp_json_2;


    tmp_json_2.show();
    tmp_json_2.remove();
    tmp_json_2.show();
    //cout << tmp_json_3.str();



    return 0;
}




