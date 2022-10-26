#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "json.hpp"

using namespace std;


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


//构造函数
to_json::to_json() : json_str_index(0) {}
to_json::to_json(const string& argv_str) : json_str(argv_str), json_str_index(0) {}


//检测字符串是否准备好了
bool to_json::str_is_ok() { return json_str.size() == 0 ? false : true; }

//检测index是否超过了字符串末尾
bool to_json::index_is_ok() { return json_str_index <= json_str.size() ? true : false;  }


//获取从当前字符开始的第一个非空字符(包括当前字符)
char to_json::get_char()
{
    while (json_str[json_str_index] == ' ' || json_str[json_str_index] == '\t' || json_str[json_str_index] == '\n' || json_str[json_str_index] == '\r')
    {
        if (!index_is_ok())
            throw std::logic_error("字符越界");

        ++json_str_index;
    }

    return json_str[json_str_index];
}

//获取从当前字符开始的下一个非空字符
char to_json::get_next_char()
{
    ++json_str_index;

    if (!index_is_ok())
            throw std::logic_error("字符越界");
    
    return get_char();
}


//解析字符串返回一个json对象
json to_json::str_to_json()
{
    get_char();

    while (index_is_ok())
    {
        if (json_str[json_str_index] == '[')
        {
            return str_json_vector();
        }
        else if (json_str[json_str_index] == '{')
        {
            return str_json_obj();
        }
        else if (json_str[json_str_index] == '\"')
        {
            return json(str_json_str());
        }
        else if (json_str[json_str_index] == 't' || json_str[json_str_index] == 'T' || json_str[json_str_index] == 'f' || json_str[json_str_index] == 'F')
        {
            return str_json_bool();
        }
        else if (json_str[json_str_index] == 'n' || json_str[json_str_index] == 'N')
        {
            return str_json_nullptr();
        }
        else if ((json_str[json_str_index] >= '0' && json_str[json_str_index] <= '9') || json_str[json_str_index] == '-')
        {
            return str_json_int_or_double();
        }
        else
            throw std::logic_error("给予的字符串不符合要求");
    }

    return json();
}

//解析文件返回一个json对象
json to_json::file_to_json()
{
    file_in.open(json_str, std::ios::in);
    
    if (!file_in.is_open())
        return json(false);

    file_str << file_in.rdbuf();
    file_in.close();
    json_str = file_str.str();
    file_str.clear();
    return  str_to_json();
}

//通过解析字符串获取一个json::nullptr对象
json to_json::str_json_nullptr()
{
    while (index_is_ok())
    {
        if ( (json_str.compare(json_str_index, 4, "null") == 0) || (json_str.compare(json_str_index, 4, "Null") == 0) || (json_str.compare(json_str_index, 4, "NULL") == 0))
        {
            json_str_index += 4;
            return json();
        }
        else if (json_str.compare(json_str_index, 7, "nullptr") == 0 || (json_str.compare(json_str_index, 7, "Nullptr") == 0) || (json_str.compare(json_str_index, 7, "NULLPTR") == 0))
        {
            json_str_index += 7;
            return json();
        }
        
        throw std::logic_error("str_json_nullptr 不完整的类型");
    }

    return json();
}

//通过解析字符串获取一个json::bool对象
json to_json::str_json_bool()
{
    while (index_is_ok())
    {
        if ( (json_str.compare(json_str_index, 4, "true") == 0) || (json_str.compare(json_str_index, 4, "True") == 0) || (json_str.compare(json_str_index, 4, "TRUE") == 0))
        {
            json_str_index += 4;
            return json(true);
        }
        else if (json_str.compare(json_str_index, 5, "false") == 0 || (json_str.compare(json_str_index, 5, "False") == 0) || (json_str.compare(json_str_index, 5, "FALSE") == 0))
        {
            json_str_index += 5;
            return json(false);
        }
        
        throw std::logic_error("str_json_bool 不完整的类型");
    }

    return json();
}

//通过解析字符串获取一个json::int对象
json to_json::str_json_int_or_double()
{
    int tmp_indx = json_str_index;
    
    if (json_str[json_str_index] == '-')
        ++json_str_index; 
    
    if (!index_is_ok())
            throw std::logic_error("字符越界");

    if (json_str[json_str_index] < '0' && json_str[json_str_index] > '9')
        throw std::logic_error("str_json_nullptr 不完整的类型");
    
    while (json_str[json_str_index] >= '0' && json_str[json_str_index] <= '9')
        ++json_str_index;
    
    if (json_str[json_str_index] != '.')
        return json(std::atoi(json_str.c_str() + tmp_indx));
    else
        ++json_str_index;
    
    if (!index_is_ok())
            throw std::logic_error("字符越界");

    if (json_str[json_str_index] < '0' && json_str[json_str_index] > '9')
        throw std::logic_error("str_json_nullptr 不完整的类型");
    
    while (json_str[json_str_index] >= '0' && json_str[json_str_index] <= '9')
        ++json_str_index;

    return json(std::atof(json_str.c_str() + tmp_indx));
}

//通过解析字符串获取一个json::str对象
string to_json::str_json_str()
{
    ++json_str_index;
    string return_str;

    while (index_is_ok())
    {
        if (json_str[json_str_index] == '\"')
        {   ++json_str_index;
            return return_str;
        }
        else if (json_str[json_str_index] == '\\')
        {
            ++json_str_index;

            if (json_str[json_str_index] == 'n')
                return_str += '\n';
            else if (json_str[json_str_index] == 't')
                return_str += '\t';
            else if (json_str[json_str_index] == 'r')
                return_str += '\r';
            else if (json_str[json_str_index] == 'b')
                return_str += '\b';
            else if (json_str[json_str_index] == 'f')
                return_str += '\f';
            else
                return_str += '\\' + json_str[json_str_index];
        }
        else
            return_str += json_str[json_str_index];
        
        ++json_str_index;
    }

    throw std::logic_error("str_json_str 不完整的类型");
    return json();
}

//通过解析字符串获取一个json::vector对象
json to_json::str_json_vector()
{
    json tmp_json(json::json_vector);
    get_next_char();

    while (index_is_ok())
    {
        if (json_str[json_str_index] == ']')
        {
            ++json_str_index;
            return json(tmp_json);
            //return tmp_json;
        }  
        else if (json_str[json_str_index] == ',' || json_str[json_str_index] == '\n'  || json_str[json_str_index] == '\r' || json_str[json_str_index] == ' ' || json_str[json_str_index] == '\t')
            get_next_char();
        else
            tmp_json.push_back(str_to_json());
    }

    throw std::logic_error("str_json_vector 不完整的类型");

    return json();
}

//通过解析字符串获取一个json::obj对象
json to_json::str_json_obj()
{
    json tmp_json(json::json_obj);
    string key;
    get_next_char();

    while (index_is_ok())
    {
        if (json_str[json_str_index] == '}')
        {
            return json(tmp_json);
        }
        else if (json_str[json_str_index] == '\"')
            key = str_json_str();
        else if (json_str[json_str_index] == ',' || json_str[json_str_index] == '\n'  || json_str[json_str_index] == '\r' || json_str[json_str_index] == ' ' || json_str[json_str_index] == '\t')
            get_next_char();  
        else
        {
            ++json_str_index;
            tmp_json.push_back(key, str_to_json());
        }
            
    }

    throw std::logic_error("str_json_obj 不完整的类型");

    return json();
}



