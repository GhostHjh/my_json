#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

//#include "str_json.hpp"


using std::cout;
using std::endl;
using std::stringstream;
using std::string;
using std::vector;
using std::map;


class json
{
public:
    enum type_t
    {
        json_null = 0,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_vector,
        json_obj
    };

private:
    union value_t
    {
        bool                m_bool;
        int                 m_int;
        double              m_double;
        string*             m_string;
        vector<json>*       m_vector;
        map<string, json>*  m_obj;
    };

public:
    json(type_t argv_type);
    json();
    json(const json& argv_json);
    json(json&& argv_json);
    ~json();

    json(bool argv_value);
    json(int argv_value);
    json(double argv_value);
    json(const char* argv_value);
    json(const string& argv_value);


    void copy(const json& argv_json);
    void move(json&& argv_json);
    void clear();

    void push_back(const json& argv_json);
    void push_back(json&& argv_json);
    void remove();
    void remove(int argv_size);

    void push_back(const string& argv_key, const json& argv_value);
    void push_back(const string& argv_key, json&& argv_value);
    void remove(const char* argv_key);
    void remove(const string& argv_key);

    //json& operator = (json&& argv_json);
    json& operator = (const json& argv_json);
    json& operator = (json&& argv_json);
    json& operator [] (int argv_indx);
    json& operator [] (const char* argv_indx);
    json& operator [] (const string& argv_indx);

    operator bool() const;
    operator int() const;
    operator double() const;
    operator string() const;

    //bool operator == (json argv_json);
    bool operator == (const json& argv_json) const;
    bool operator != (const json& argv_json) const;
    bool operator == (const bool& argv_bool) const ;
    bool operator == (const int& argv_int) const;   
    bool operator == (const double& argv_double) const;
    bool operator == (const char* argv_cstr) const;
    bool operator == (const string& argv_string) const;


    void str_to_json(const string& argv_json_str);
    void file_to_json(const string& argv_file_name);

    string str();
    void show();

    json::type_t get_type() const;

private:
    type_t m_type;
    value_t m_value;
};



//通过类型构造
json::json(type_t argv_type) : m_type(argv_type)
{
    if (argv_type == json_bool)
        m_value.m_bool = false;
    else if (argv_type == json_int)
        m_value.m_int = 0;
    else if (argv_type == json_double)
        m_value.m_double = 0.0;
    else if (argv_type == json_string)
        m_value.m_string = new string();
    else if (argv_type == json_vector)
        m_value.m_vector = new vector<json>();
    else if (argv_type == json_obj)
        m_value.m_obj = new map<string, json>();
}

//无参类型构造(委托构造)
json::json() : json(type_t::json_null) {}

//左值构造
json::json(const json& argv_json) : m_type(type_t::json_null)
{
    copy(argv_json);
}

//右值构造
json::json(json&& argv_json) : m_type(type_t::json_null)
{
    clear();
    m_type = argv_json.m_type;
    if (m_type == json_bool)
        m_value.m_bool = argv_json.m_value.m_bool;
    else if (m_type == json_int)
        m_value.m_int = argv_json.m_value.m_int;
    else if (m_type == json_double)
        m_value.m_double = argv_json.m_value.m_double;
    else if (m_type == json_string)
    {
        m_value.m_string = argv_json.m_value.m_string;
        argv_json.m_value.m_string = nullptr;
        //(m_value.m_string)->swap(*(argv_json.m_value.m_string));

    } 
    else if (m_type == json_vector)
    {
        m_value.m_vector = argv_json.m_value.m_vector;
        argv_json.m_value.m_vector = nullptr;
        //(m_value.m_vector)->swap(*(argv_json.m_value.m_vector));
    }
    else if (m_type == json_obj)
    {
        m_value.m_obj = argv_json.m_value.m_obj;
        argv_json.m_value.m_obj = nullptr;
        //(m_value.m_obj)->swap(*(argv_json.m_value.m_obj));
    }
}

//析构
json::~json(){    clear();  }

//用值构造
/*************************************************************************************/
json::json(bool argv_value)         : m_type(type_t::json_bool)     {    m_value.m_bool = argv_value;   }
json::json(int argv_value)          : m_type(type_t::json_int)      {    m_value.m_int = argv_value;    }
json::json(double argv_value)       : m_type(type_t::json_double)   {    m_value.m_double = argv_value;    }
json::json(const char* argv_value)  : m_type(type_t::json_string)   {    m_value.m_string = new string(argv_value);    }
json::json(const string& argv_value): m_type(type_t::json_string)   {    m_value.m_string = new string(argv_value);    }
/*************************************************************************************/




//深拷贝
void json::copy(const json& argv_json)
{
    m_type = argv_json.m_type;
    if (m_type == json_bool)
        m_value.m_bool = argv_json.m_value.m_bool;
    else if (m_type == json_int)
        m_value.m_int = argv_json.m_value.m_int;
    else if (m_type == json_double)
        m_value.m_double = argv_json.m_value.m_double;
    else if (m_type == json_string)
        m_value.m_string = new string(*(argv_json.m_value.m_string));
    else if (m_type == json_vector)
        m_value.m_vector = new vector<json>(*(argv_json.m_value.m_vector));
    else if (m_type == json_obj)
        m_value.m_obj = new map<string, json>(*(argv_json.m_value.m_obj));
}

//移动
void json::move(json&& argv_json)
{
    clear();
    m_type = argv_json.m_type;
    if (m_type == json_bool)
        m_value.m_bool = argv_json.m_value.m_bool;
    else if (m_type == json_int)
        m_value.m_int = argv_json.m_value.m_int;
    else if (m_type == json_double)
        m_value.m_double = argv_json.m_value.m_double;
    else if (m_type == json_string)
    {
        m_value.m_string = argv_json.m_value.m_string;
        argv_json.m_value.m_string = nullptr;
        //(m_value.m_string)->swap(*(argv_json.m_value.m_string));

    } 
    else if (m_type == json_vector)
    {
        m_value.m_vector = argv_json.m_value.m_vector;
        argv_json.m_value.m_vector = nullptr;
        //(m_value.m_vector)->swap(*(argv_json.m_value.m_vector));
    }
    else if (m_type == json_obj)
    {
        m_value.m_obj = argv_json.m_value.m_obj;
        argv_json.m_value.m_obj = nullptr;
        //(m_value.m_obj)->swap(*(argv_json.m_value.m_obj));
    }
}

//清空数据
void json::clear()
{
    if (m_type == json_bool)
    {
        m_value.m_bool = false;
    }
    else if (m_type == json_int)
    {
        m_value.m_int = 0;
    }    
    else if (m_type == json_double)
    {
        m_value.m_double = 0.0;
    }  
    else if (m_type == json_string && m_value.m_string != nullptr)
    {
        delete(m_value.m_string);
        m_value.m_string = nullptr;
    }  
    else if (m_type == json_vector && m_value.m_vector != nullptr)
    {
        for (auto a = (m_value.m_vector)->begin(); a != (m_value.m_vector)->end(); ++a)
        {
            a->clear();
        }
        delete(m_value.m_vector);

        m_value.m_vector = nullptr;
    }
    else if (m_type == json_obj && m_value.m_obj != nullptr)
    { 
        for (auto a = (m_value.m_obj)->begin(); a != (m_value.m_obj)->end(); ++a)
        {
            (a->second).clear();
        }
        delete(m_value.m_obj);  

        m_value.m_obj = nullptr; 
    }

    m_type = json_null;
}


//设置json::type_t::vector类型添加元素
void json::push_back(const json& argv_json)
{
    if (m_type != json_vector)
    {
        clear();
        m_type = json_vector;
        m_value.m_vector = new vector<json>;
    }

    (m_value.m_vector)->push_back(argv_json);
}

//设置json::type_t::vector类型添加元素
void json::push_back(json&& argv_json)
{
    if (m_type != json_vector)
    {
        clear();
        m_type = json_vector;
        m_value.m_vector = new vector<json>;
    }

    (m_value.m_vector)->push_back(argv_json);
    //show();
}

//设置json::type_t::vector类型删除元素
void json::remove()
{
    if (m_type != json_vector)
        throw std::logic_error("非vector类型, 不能remove");
    
    if ((m_value.m_vector)->size() > 0)
        (m_value.m_vector)->pop_back();
}

//设置json::type_t::vector类型删除多个元素
void json::remove(int argv_size)
{
    if (argv_size < 1)
        throw std::logic_error("不能删除0个或更少的元素");

    for (int i = 0; i < argv_size; ++i)
        remove();
}


//设置json::type_t::obj添加元素
void json::push_back(const string& argv_key, const json& argv_value)
{
    if (m_type != json_obj)
    {
        clear();
        m_type = json_obj;
        m_value.m_obj = new map<string, json>;
    }

    (*(m_value.m_obj))[argv_key] = argv_value;
}

//设置json::type_t::obj右值添加元素
void json::push_back(const string& argv_key, json&& argv_value)
{
    if (m_type != json_obj)
    {
        clear();
        m_type = json_obj;
        m_value.m_obj = new map<string, json>;
    }

    (*(m_value.m_obj))[argv_key] = argv_value;
}

//设置json::type_t::obj删除元素
void json::remove(const char* argv_key)
{
    remove(string(argv_key));
}

//设置json::type_t::obj删除元素
void json::remove(const string& argv_key)
{
    if ((m_value.m_obj)->count(argv_key) == 1)
        (m_value.m_obj)->erase(argv_key);
}


//设置json用=时做深拷贝
json& json::operator = (const json& argv_json)
{
    copy(argv_json);
    return *this;
}

//设置json用=一个右值时做移动拷贝
json& json::operator = (json&& argv_json)
{
    clear();
    cout << argv_json.m_type;
    //move(argv_json);
    m_type = argv_json.m_type;
    if (m_type == json_bool)
        m_value.m_bool = argv_json.m_value.m_bool;
    else if (m_type == json_int)
        m_value.m_int = argv_json.m_value.m_int;
    else if (m_type == json_double)
        m_value.m_double = argv_json.m_value.m_double;
    else if (m_type == json_string)
    {
        m_value.m_string = argv_json.m_value.m_string;
        argv_json.m_value.m_string = nullptr;

    } 
    else if (m_type == json_vector)
    {
        m_value.m_vector = argv_json.m_value.m_vector;
        argv_json.m_value.m_vector = nullptr;
    }
    else if (m_type == json_obj)
    {
        m_value.m_obj = argv_json.m_value.m_obj;
        argv_json.m_value.m_obj = nullptr;
    }
    
    return *this;
}

//设置json::type_t::vector可以使用下标(内存偏移地址访问)
json& json::operator [] (int argv_indx)
{
    if (m_type != json_vector)
    {
        clear();
        m_type = json_vector;
        m_value.m_vector = new vector<json>;
    }

    if (argv_indx < 0)
        throw std::logic_error("不能使用小于0的下标");

    if (argv_indx > ((m_value.m_vector)->size() *2) -1)
        throw std::logic_error("内存越界");
    
    if (argv_indx <= (m_value.m_vector)->size() *2 && argv_indx > (m_value.m_vector)->size() -1)
    {
        for (int i = (m_value.m_vector)->size(); i <= argv_indx; ++i)
            (m_value.m_vector)->push_back(json());
    }

    return (m_value.m_vector)->at(argv_indx);
}

//设置json::type_t::obj可以使用下标(内存偏移地址访问)
json& json::operator [] (const char* argv_key)
{
    return (*this)[string(argv_key)];
}

//设置json::type_t::obj可以使用下标(内存偏移地址访问)
json& json::operator [] (const string& argv_key)
{
    if (m_type != json_obj)
    {
        clear();
        m_type = json_obj;
        m_value.m_obj = new map<string, json>;
    }

    return (*(m_value.m_obj))[argv_key];
}

//类型转换
/*************************************************************************************/
json::operator bool() const
{
    if (m_type == json_bool)
        return m_value.m_bool;
    
    throw std::logic_error("转换类型不正确");
}
json::operator int() const
{
    if (m_type == json_int)
        return m_value.m_int;

    throw std::logic_error("转换类型不正确");
}
json::operator double() const
{
    if (m_type == json_double)
        return m_value.m_double;
    
    throw std::logic_error("转换类型不正确");
}
json::operator string() const
{
    if (m_type == json_string)
        return *(m_value.m_string);
    
    throw std::logic_error("转换类型不正确");
}
/*************************************************************************************/

//用于和其它类型对比
/*************************************************************************************/
bool json::operator == (const json& argv_json) const
{
    if (argv_json.m_type == json_bool)
    {
        return (argv_json.m_value.m_bool == m_value.m_bool) ? true : false;
    }
    else if (argv_json.m_type == json_int)
    {
        return (argv_json.m_value.m_int == m_value.m_int) ? true : false;
    }
    else if (argv_json.m_type == json_double)
    {
        return (argv_json.m_value.m_double == m_value.m_double) ? true : false;
    }
    else if (argv_json.m_type == json_string)
    {
        return (*(argv_json.m_value.m_string) == *(m_value.m_string)) ? true : false;
    }
    else if (argv_json.m_type == json_vector)
    {
        if ((argv_json.m_value.m_vector)->size() == (m_value.m_vector)->size())
        {
            auto a = (argv_json.m_value.m_vector)->begin();
            auto b = (m_value.m_vector)->begin();
            auto a_end = (argv_json.m_value.m_vector)->end();
            auto b_end = (m_value.m_vector)->end();

            while( a != a_end && b != b_end)
            {
                if (*a == *b)
                    ++a, ++b;
                else
                    return false;
            }
        }
    }
    else if (argv_json.m_type == json_obj)
    {
        if ((argv_json.m_value.m_obj)->size() == (m_value.m_obj)->size())
        {
            auto a = (argv_json.m_value.m_obj)->begin();
            auto b = (m_value.m_obj)->begin();
            auto a_end = (argv_json.m_value.m_obj)->end();
            auto b_end = (m_value.m_obj)->end();

            while( a != a_end && b != b_end)
            {
                if (a->first == b->first && a->second == b->second)
                    ++a, ++b;
                else
                    return false;
            }
        }
    }
    else if (argv_json.m_type == json_null)
    {
        return (argv_json.m_type == m_type) ? true : false; 
    }
    
    return false;
}
bool json::operator != (const json& argv_json) const
{
    return !((*this) == argv_json);
}
bool json::operator == (const bool& argv_bool) const 
{
    if (m_type != json_bool)
        return false;

    return (argv_bool == m_value.m_bool) ? true : false;
}
bool json::operator == (const int& argv_int) const
{
    if (m_type != json_int)
        return false;

    return (argv_int == m_value.m_int) ? true : false;
}   
bool json::operator == (const double& argv_double) const
{
    if (m_type != json_double)
        return false;

    return (argv_double == m_value.m_double) ? true : false;
}
bool json::operator == (const char* argv_cstr) const
{
    return operator ==(string(argv_cstr));
}
bool json::operator == (const string& argv_string) const
{
    if (m_type != json_string)
        return false;

    return (argv_string == *(m_value.m_string)) ? true : false;
}
/*************************************************************************************/

void json::str_to_json(const string& argv_json_str)
{
    
}

void json::file_to_json(const string& argv_file_name)
{

}



//获取json_value类型
json::type_t json::get_type() const
{
    return m_type;
}

//获取拼接好的字符串
string json::str()
{
    stringstream ss;
    
    if (m_type == json_bool)
    {
        if (m_value.m_bool)
            return string("true");
        else
            return string("false");
    }     
    else if (m_type == json_int)
    {
        ss << m_value.m_int;
        return string(ss.str());
    }
    else if (m_type == json_double)
    {
        ss << m_value.m_double;
        return string(ss.str());
    }
    else if (m_type == json_string)
    {
        return string('\"' + *(m_value.m_string) + '\"');
    }
    else if (m_type == json_vector)
    {
        string str_tmp = "[ ";
        for (auto a = (m_value.m_vector)->begin(); a != (m_value.m_vector)->end(); ++a)
        {
            if (a != (m_value.m_vector)->begin())
                str_tmp += ", ";
            str_tmp += a->str();
        }
        str_tmp += " ]";
        return str_tmp;
    }
    else if (m_type == json_obj)
    {
        string str_tmp = "{ ";
        for (auto a = (m_value.m_obj)->begin(); a != (m_value.m_obj)->end(); ++a)
        {
            if (a != (m_value.m_obj)->begin())
                str_tmp += ", ";
            str_tmp +=  '\"' + a->first + "\" : ";
            str_tmp += a->second.str();
        }
        str_tmp += " }";
        return str_tmp;
    }

    return string("nullptr");
}

//显示
void json::show()
{
    if (m_type == json_bool)
    {
        if (m_value.m_bool)
            cout << "true";
        else
            cout << "false";
    }     
    else if (m_type == json_int)
    {
        cout << m_value.m_int;
    }
    else if (m_type == json_double)
    {
        cout << m_value.m_double;
    }
    else if (m_type == json_string)
    {
        cout << '\"' << (m_value.m_string)->c_str() << '\"'; 
    }
    else if (m_type == json_vector)
    {
        cout << "[ ";
        for (auto a = (m_value.m_vector)->begin(); a != (m_value.m_vector)->end(); ++a)
        {
            if (a != (m_value.m_vector)->begin())
                cout << ", ";
            a->show();
        }
        cout << " ]";    
    }
    else if (m_type == json_obj)
    {
        cout << "{\n";
        for (auto a = (m_value.m_obj)->begin(); a != (m_value.m_obj)->end(); ++a)
        {
            if (a != (m_value.m_obj)->begin())
                cout << ", \n";
            cout << "\t\"" << a->first << "\" : ";
            a->second.show();
        }
        cout << "\n}";
    }
    else
    {
        cout << "nullptr";
    }
}











