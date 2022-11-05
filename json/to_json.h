#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "json.h"

using std::ifstream;
using std::string;
using std::stringstream;


class to_json
{
public:
    to_json();
    to_json(const string& argv_str);

    json str_to_json();
    json file_to_json();

private:
    bool str_is_ok();
    bool index_is_ok();

    char get_char();
    char get_next_char();


    json    str_json_nullptr();
    json    str_json_bool();
    json    str_json_int_or_double();
    string  str_json_str();
    json    str_json_vector();
    json    str_json_obj();

private:
    string json_str;
    int json_str_index;

    ifstream file_in;
    stringstream file_str;
};



