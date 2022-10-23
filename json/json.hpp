#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

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
    union vlaue_t
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

    json(bool argv_vlaue);
    json(int argv_vlaue);
    json(double argv_vlaue);
    json(const char* argv_vlaue);
    json(string argv_vlaue);

    ~json();

    json::type_t get_type();

    void clear();

    void push_back(const json& argv_json);
    void remove();
    void remove(int argv_size);

    void push_back(string argv_key, json argv_vlaue);
    void remove(const char* argv_key);
    void remove(const string& argv_key);

    //json& operator = (json&& argv_json);
    json& operator = (json argv_json);
    json& operator [] (int argv_indx);
    json& operator [] (const char* argv_indx);
    json& operator [] (const string& argv_indx);

    string str();
    void show();


private:
    type_t m_type;
    vlaue_t m_vlaue; 
};

//通过类型构造
json::json(type_t argv_type) : m_type(argv_type)
{
    if (argv_type == json_bool)
        m_vlaue.m_bool = false;
    else if (argv_type == json_int)
        m_vlaue.m_int = 0;
    else if (argv_type == json_double)
        m_vlaue.m_double = 0.0;
    else if (argv_type == json_string)
        m_vlaue.m_string = new string();
    else if (argv_type == json_vector)
        m_vlaue.m_vector = new vector<json>();
    else if (argv_type == json_obj)
        m_vlaue.m_obj = new map<string, json>();
}

//无参类型构造(委托构造)
json::json() : json(type_t::json_null) 
{
}

//用值构造
/*************************************************************************************/
json::json(bool argv_vlaue) : m_type(type_t::json_bool)
{
    m_vlaue.m_bool = argv_vlaue;
}
json::json(int argv_vlaue) : m_type(type_t::json_int)
{
    m_vlaue.m_int = argv_vlaue;
}
json::json(double argv_vlaue) : m_type(type_t::json_double)
{
    m_vlaue.m_double = argv_vlaue;
}
json::json(const char* argv_vlaue) : m_type(type_t::json_string)
{
    m_vlaue.m_string = new string(argv_vlaue);
}
json::json(string argv_vlaue) : m_type(type_t::json_string)
{
    m_vlaue.m_string = new string(argv_vlaue);
}
/*************************************************************************************/

json::~json()
{
    clear();
}

json::type_t json::get_type()
{
    return m_type;
}

//清空数据
void json::clear()
{
    if (m_type == json_bool)
    {
        m_vlaue.m_bool = false;
    }
    else if (m_type == json_int)
    {
        m_vlaue.m_int = 0;
    }    
    else if (m_type == json_double)
    {
        m_vlaue.m_double = 0.0;
    }  
    else if (m_type == json_string)
    {
        delete(m_vlaue.m_string);
    }  
    else if (m_type == json_vector)
    {
        if (m_vlaue.m_vector != nullptr)
        {
            for (auto a = (m_vlaue.m_vector)->begin(); a != (m_vlaue.m_vector)->end(); ++a)
            {
                a->clear();
            }
            delete(m_vlaue.m_vector);
        }
    }
    else if (m_type == json_obj)
    {
        if (m_vlaue.m_obj != nullptr)
        {
            for (auto a = (m_vlaue.m_obj)->begin(); a != (m_vlaue.m_obj)->end(); ++a)
            {
                (a->second).clear();
            }
            delete(m_vlaue.m_obj);
        }
    }

    m_type = json_null;
}

//设置json::type_t::vector类型添加元素
void json::push_back(const json& argv_vlaue)
{
    if (m_type != json_vector)
    {
        clear();
        m_type = json_vector;
        m_vlaue.m_vector = new vector<json>;
    }

    m_vlaue.m_vector->push_back(argv_vlaue);
}

//设置json::type_t::vector类型删除元素
void json::remove()
{
    if (m_type != json_vector)
        throw std::logic_error("非vector类型, 不能remove");
    
    if ((m_vlaue.m_vector)->size() > 0)
        (m_vlaue.m_vector)->pop_back();
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
void json::push_back(string argv_key, json argv_vlaue)
{
    (*(m_vlaue.m_obj))[argv_key] = argv_vlaue;
}

//设置json::type_t::obj删除元素
void json::remove(const char* argv_key)
{
    remove(string(argv_key));
}

//设置json::type_t::obj删除元素
void json::remove(const string& argv_key)
{
    if ((m_vlaue.m_obj)->count(argv_key) == 1)
        (m_vlaue.m_obj)->erase(argv_key);
}


// json& json::operator = (json&& argv_json)
// {
//     this->m_type = argv_json.m_type;
//     argv_json.m_type = json::type_t::json_null;

//     if (m_type == json_bool)
//         m_vlaue.m_bool = argv_json.m_vlaue.m_bool;
//     else if (m_type == json_int)
//         m_vlaue.m_int = argv_json.m_vlaue.m_int;
//     else if (m_type == json_double)
//         m_vlaue.m_double = argv_json.m_vlaue.m_double;
//     else if (m_type == json_string)
//     {
//         m_vlaue.m_string = argv_json.m_vlaue.m_string;
//         argv_json.m_vlaue.m_string = nullptr;
//     }   
//     else if (m_type == json_vector)
//     {
//         m_vlaue.m_vector = argv_json.m_vlaue.m_vector;
//         argv_json.m_vlaue.m_vector = nullptr;
//     }
//     else if (m_type == json_obj)
//     {
//         m_vlaue.m_obj = argv_json.m_vlaue.m_obj;
//         argv_json.m_vlaue.m_obj = nullptr;
//     }

//     argv_json.clear();

//     return (*this);
// }

json& json::operator = (json argv_json)
{
    *this = argv_json;
    return (*this);
}


//设置json::type_t::vector可以使用下标(内存偏移地址访问)
json& json::operator [] (int argv_indx)
{
    if (m_type != json_vector)
    {
        clear();
        m_type = json_vector;
        m_vlaue.m_vector = new vector<json>;
    }

    if (argv_indx < 0)
        throw std::logic_error("不能使用小于0的下标");

    if (argv_indx > ((m_vlaue.m_vector)->size() *2) -1)
        throw std::logic_error("内存越界");
    
    if (argv_indx <= (m_vlaue.m_vector)->size() *2 && argv_indx > (m_vlaue.m_vector)->size() -1)
    {
        for (int i = (m_vlaue.m_vector)->size(); i <= argv_indx; ++i)
            (m_vlaue.m_vector)->push_back(json());
    }

    return (m_vlaue.m_vector)->at(argv_indx);
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
        m_vlaue.m_obj = new map<string, json>;
    }

    return (*(m_vlaue.m_obj))[argv_key];
}

//获取拼接好的字符串
string json::str()
{
    stringstream ss;
    
    if (m_type == json_bool)
    {
        if (m_vlaue.m_bool)
            return string("true");
        else
            return string("false");
    }     
    else if (m_type == json_int)
    {
        ss << m_vlaue.m_int;
        return string(ss.str());
    }
    else if (m_type == json_double)
    {
        ss << m_vlaue.m_int;
        return string(ss.str());
    }
    else if (m_type == json_string)
    {
        return string('\"' + *(m_vlaue.m_string) + '\"');
    }
    else if (m_type == json_vector)
    {
        string str_tmp = "[ ";
        for (auto a = (m_vlaue.m_vector)->begin(); a != (m_vlaue.m_vector)->end(); ++a)
        {
            if (a != (m_vlaue.m_vector)->begin())
                str_tmp += ", ";
            str_tmp += a->str();
        }
        str_tmp += " ]";
        return str_tmp;
    }
    else if (m_type == json_obj)
    {
        string str_tmp = "{ ";
        for (auto a = (m_vlaue.m_obj)->begin(); a != (m_vlaue.m_obj)->end(); ++a)
        {
            if (a != (m_vlaue.m_obj)->begin())
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
        if (m_vlaue.m_bool)
            cout << "true";
        else
            cout << "false";
    }     
    else if (m_type == json_int)
    {
        cout << m_vlaue.m_int;
    }
    else if (m_type == json_double)
    {
        cout << m_vlaue.m_double;
    }
    else if (m_type == json_string)
    {
        cout << '\"' << *(m_vlaue.m_string) << '\"'; 
    }
    else if (m_type == json_vector)
    {
        cout << "[ ";
        for (auto a = (m_vlaue.m_vector)->begin(); a != (m_vlaue.m_vector)->end(); ++a)
        {
            if (a != (m_vlaue.m_vector)->begin())
                cout << ", ";
            a->show();
        }
        cout << " ]";    
    }
    else if (m_type == json_obj)
    {
        cout << "{\n";
        for (auto a = (m_vlaue.m_obj)->begin(); a != (m_vlaue.m_obj)->end(); ++a)
        {
            if (a != (m_vlaue.m_obj)->begin())
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











