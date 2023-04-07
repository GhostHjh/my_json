#pragma once
#include <iostream>
#include <map>
#include <vector>

namespace JSON
{
enum Type
{
    Nullptr = 0,
    Bool,
    Int,
    Double,
    String,
    Array,
    Object
};

class Json_value
{
public:
    Json_value();
    Json_value(const Json_value& argv_value);
    Json_value(Json_value&& argv_value);
    
    Json_value(const Type& argv_type);
    Json_value(const bool& argv_value);
    Json_value(const int& argv_value);
    Json_value(const double& argv_value);
    Json_value(const char* argv_value);
    Json_value(const std::string& argv_value);
    Json_value(const std::vector<Json_value>& argv_value);
    Json_value(const std::map<std::string, Json_value>& argv_value);

    Json_value& operator = (const Json_value& argv_value);
    Json_value& operator = (Json_value&& argv_value);

    ~Json_value();


public:
    operator bool() const;
    operator int() const;
    operator double() const;
    operator std::string() const;

    const bool operator == (const Json_value& argv_value);
    const bool operator == (const bool& argv_value);
    const bool operator == (const int& argv_value);
    const bool operator == (const double& argv_value);
    const bool operator == (const char* argv_value);
    const bool operator == (const std::string& argv_value);

    const bool operator != (const Json_value& argv_value);
    const bool operator != (const bool& argv_value);
    const bool operator != (const int& argv_value);
    const bool operator != (const double& argv_value);
    const bool operator != (const char* argv_value);
    const bool operator != (const std::string& argv_value);
    
    const bool operator < (const int& argv_value);
    const bool operator < (const double& argv_value);
    const bool operator < (const char* argv_value);
    const bool operator < (const std::string& argv_value);
    
    const bool operator > (const int& argv_value);
    const bool operator > (const double& argv_value);
    const bool operator > (const char* argv_value);
    const bool operator > (const std::string& argv_value);

    friend std::ostream& operator << (std::ostream& argv_ostream, const Json_value& argv_value);
    //friend std::ostream& operator << (std::ostream& argv_ostream, const Json_value&& argv_value);


public:
    const Type& value_type();
    const bool is_bool();
    const bool is_int();
    const bool is_double();
    const bool is_string();
    const bool is_array();
    const bool is_object();

    void insert(Json_value argv_value);
    void insert(std::pair<std::string, Json_value> argv_value);

    Json_value operator[] (const int& argv_index);
    Json_value operator[] (const char* argv_key);
    Json_value operator[] (const std::string& argv_key);

    void clear();


private:
    void copy(const Json_value& argv_value);
    void move(Json_value&& argv_value);


private:
    Type                                _value_type;
    
    bool                                _value_bool;
    int                                 _value_int;
    double                              _value_double;
    std::string*                        _value_string;
    std::vector<Json_value>*            _value_array;
    std::map<std::string, Json_value>*  _value_object;
};




class Serialization
{
public:
    Serialization();
    Serialization(const Serialization& argv_Serialization);
    Serialization(Serialization&& argv_Serialization);

    const Serialization& operator = (const Serialization& argv_Serialization);
    const Serialization& operator = (Serialization&& argv_Serialization);


public:
    const Json_value parser(const std::string& argv_str);
    const Json_value parser(const char* argv_str);
    const std::string parser(const Json_value& argv_value);
    const bool& parser_succeed();
    void clear();


private:
    void skip_space();
    const char& get_nospace_ch();
    const Json_value parser();

    void copy(const Serialization& argv_Serialization);
    void move(Serialization&& argv_Serialization);


private:
    const Json_value parser_Nullptr();
    const Json_value parser_Bool_True();
    const Json_value parser_Bool_False();
    const Json_value parser_Int_Doublue();
    const Json_value parser_String();
    const Json_value parser_Array();
    const Json_value parser_Object();


private:
    std::string _json_str;
    int _json_str_index;
    bool _parser_succeed;
};





enum Json_create_mode
{
    String_create = 0,
    File_create
};

class Json
{
public:
    Json();
    Json(const Json& argv_json);
    Json(Json&& argv_json);
    Json(const Json_value& argv_value);
    Json(const Type& argv_json_type);
    Json(const std::string& argv_str, const Json_create_mode& argv_create_mode);

    const Json& operator = (const Json& argv_json);
    const Json& operator = (Json&& argv_json);


public:
    const Json_value& get_value();
    //const Json_value& get_value(int argv_index);
    //const Json_value& get_value(const std::string& argv_key);
    void set_value(const Json_value& argv_value);
    void set_value(const Type& argv_json_type);
    void set_value(const std::string& argv_str, const Json_create_mode& argv_create_mode);


private:
    void clear();
    void copy(const Json& argv_json);
    void move(Json&& argv_json);


private:
    Serialization _serialization;


private:
    Json_value _value;    
};

}





