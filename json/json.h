#pragma once
#include <iostream>
#include <sstream>
#include <string>
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
    ~json();

    json(bool argv_value);
    json(int argv_value);
    json(double argv_value);
    json(const char* argv_value);
    json(const string& argv_value);
    json(const json& argv_json);
    json(json&& argv_json);


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






