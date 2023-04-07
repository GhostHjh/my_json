#include "Json.h"

#include <fstream>
#include <sstream>

namespace JSON
{


std::ostream& operator << (std::ostream& argv_ostream, const Json_value& argv_value)
{
    if(argv_value._value_type == Type::Nullptr)
        argv_ostream << "Null";
    else if (argv_value._value_type == Type::Bool)
        argv_ostream << (argv_value._value_bool ? "True" : "False");
    else if (argv_value._value_type == Type::Int)
        argv_ostream << argv_value._value_int;
    else if(argv_value._value_type == Type::Double)
        argv_ostream << argv_value._value_double;
    else if(argv_value._value_type == Type::String)
        argv_ostream << '\"' << *(argv_value._value_string) << '\"';
    else if (argv_value._value_type == Type::Array)
    {
        argv_ostream << "[ ";
        for (auto i = (argv_value._value_array)->begin(); i != (argv_value._value_array)->end(); ++i)
        {
            argv_ostream << *i;
            if (i +1 != (argv_value._value_array)->end())
                argv_ostream << ", ";
        }
        argv_ostream << " ]";
    }
    else if (argv_value._value_type == Type::Object)
    {
        argv_ostream << "{ ";
        for (auto i = (argv_value._value_object)->begin(); i != (argv_value._value_object)->end(); ++i)
        {
            argv_ostream << '\"' << i->first << "\" : " << i->second;
           if ((i->first) != (((argv_value._value_object)->rbegin())->first))
                argv_ostream << ", ";           
        }
        argv_ostream << " }";
    }

    return argv_ostream;
}

//std::ostream& operator << (std::ostream& argv_ostream, const Json_value&& argv_value)
//{
//    if(argv_value._value_type == Type::Nullptr)
//        argv_ostream << "Nullptr";
//    else if (argv_value._value_type == Type::Int)
//        argv_ostream << argv_value._value_int;
//    else if(argv_value._value_type == Type::Double)
//        argv_ostream << argv_value._value_double;
//    else if(argv_value._value_type == Type::String)
//        argv_ostream << '\"' <<*(argv_value._value_string) << '\"';
//
//    return argv_ostream;
//}






//Json_value
//构造和析构函数
/*************************************************************************************************************************************************************/

Json_value::Json_value()
    : _value_type(Type::Nullptr), _value_bool(false), _value_int(0), _value_double(0.0), 
        _value_string(nullptr), _value_array(nullptr), _value_object(nullptr)
{}

Json_value::Json_value(const Json_value& argv_value)
    : Json_value()
{    copy(argv_value);    }

Json_value::Json_value(Json_value&& argv_value)
    : Json_value()
{    move(std::move(argv_value));   }


Json_value::Json_value(const Type& argv_type)
    : Json_value()
{
    _value_type = argv_type;
    if(_value_type == Type::String)
        _value_string = new std::string;
    else if (_value_type == Type::Array)
        _value_array = new std::vector<Json_value>;
    else if (_value_type == Type::Object)
        _value_object = new std::map<std::string, Json_value>;
}

Json_value::Json_value(const bool& argv_value)  
    : Json_value()
{   _value_type = Type::Bool; _value_bool = argv_value;   }

Json_value::Json_value(const int& argv_value)
    :Json_value()
{   _value_type = Type::Int; _value_int = argv_value;    }

Json_value::Json_value(const double& argv_value)
{   _value_type = Type::Double; _value_double = argv_value;    }

Json_value::Json_value(const char* argv_value)
    : Json_value(std::string(argv_value))
{}

Json_value::Json_value(const std::string& argv_value)
    : Json_value()
{   _value_type = Type::String; _value_string = new std::string(argv_value);    }

Json_value::Json_value(const std::vector<Json_value>& argv_value)
    : Json_value()
{   _value_type = Type::Array; _value_array = new std::vector<Json_value>(argv_value);    }

Json_value::Json_value(const std::map<std::string, Json_value>& argv_value)
    : Json_value()
{   _value_type = Type::Object; _value_object = new std::map<std::string, Json_value>(argv_value);    }


Json_value& Json_value::operator = (const Json_value& argv_value)
{    clear(); copy(argv_value); return (*this);    }

Json_value& Json_value::operator = (Json_value&& argv_value)
{    clear(); move(std::move(argv_value)); argv_value.clear(); return (*this);    }

Json_value::~Json_value()
{    clear();   }



//比较运算符重载
/*************************************************************************************************************************************************************/

Json_value::operator bool() const
{
    if (_value_type == Type::Int)
        return static_cast<bool>(_value_int);
    else if (_value_type == Type::Double)
        return static_cast<bool>(_value_double);
    else
        return _value_bool;
}

Json_value::operator int() const
{
    if (_value_type == Type::Bool)
        return static_cast<int>(_value_bool);
    else if (_value_type == Type::Double)
        return static_cast<int>(_value_double);
    else
        return _value_int;
}

Json_value::operator double() const
{
    if (_value_type == Type::Bool)
        return static_cast<double>(_value_bool);
    if (_value_type == Type::Int)
        return static_cast<double>(_value_int);
    else
        return static_cast<bool>(_value_double);
}

Json_value::operator std::string() const
{
    if (_value_type == Type::String)
        return *_value_string;
    return std::string();
}


const bool Json_value::operator == (const Json_value& argv_value)
{
    if (_value_type == Type::Nullptr && argv_value._value_type == Type::Nullptr)
        return true;
    
    if ((_value_type == Type::Array && argv_value._value_type != Type::Array) || (_value_type != Type::Array && argv_value._value_type == Type::Array) ||
        (_value_type == Type::Object && argv_value._value_type != Type::Object) || (_value_type != Type::Object && argv_value._value_type == Type::Object) ||
        (_value_type == Type::String && argv_value._value_type != Type::String) || (_value_type != Type::String && argv_value._value_type == Type::String))
        return false;

    if (_value_type == Type::Array)
    {
        if (_value_array->size() != argv_value._value_array->size())
            return false;

        auto a = _value_array->begin();
        auto b = argv_value._value_array->begin();

        for (; a != _value_array->end() && b != argv_value._value_array->end(); ++a, ++b)
        {
            if (*a != *b)
                return false;
        }

        return true;
    }
    else if (_value_type == Type::Object)
    {
        if (_value_object->size() != argv_value._value_object->size())
            return false;

        auto a = _value_object->begin();
        auto b = argv_value._value_object->begin();

        for (; a != _value_object->end() && b != argv_value._value_object->end(); ++a, ++b)
        {
            if ((a->first != b->first) || (a->second != b->second))
                return false;
        }

        return true;
    }
    else if (_value_type == Type::String)
        return *_value_string == *(argv_value._value_string);
    else if (_value_type == Type::Bool)
        return _value_bool == static_cast<bool>(argv_value);   
    else if (_value_type == Type::Int)
        return _value_int == static_cast<int>(argv_value);
    else if (_value_type == Type::Double)
        return _value_double == static_cast<double>(argv_value);
    
    return false;
}

const bool Json_value::operator == (const bool& argv_value)
{   return (_value_type == Type::Bool && _value_bool == argv_value);    }

const bool Json_value::operator == (const int& argv_value)
{    return (_value_type == Type::Int && _value_int == argv_value); }

const bool Json_value::operator == (const double& argv_value)
{    return (_value_type == Type::Double && _value_double == argv_value);   }

const bool Json_value::operator == (const std::string& argv_value)
{    return (_value_type == Type::String) ? (*_value_string == argv_value) : false;    }


const bool Json_value::operator != (const Json_value& argv_value)
{   return !(*this == argv_value);    }

const bool Json_value::operator != (const bool& argv_value)
{   return !(*this == argv_value);    }

const bool Json_value::operator != (const int& argv_value)
{   return !(*this == argv_value);    }

const bool Json_value::operator != (const double& argv_value)
{   return !(*this == argv_value);    }

const bool Json_value::operator != (const std::string& argv_value)
{   return !(*this == argv_value);    }


const bool Json_value::operator < (const int& argv_value)
{    return ((_value_type == Type::Int && _value_int < argv_value) || (_value_type == Type::Double && _value_double < argv_value)); }

const bool Json_value::operator < (const double& argv_value)
{    return (( _value_type == Type::Double && _value_double < argv_value) || (_value_type == Type::Int && _value_int < argv_value));    }

const bool Json_value::operator < (const char* argv_value)
{   return *this < (std::string(argv_value));    }

const bool Json_value::operator < (const std::string& argv_value)
{
    if (_value_type == Type::String)
    {
        int find_index = argv_value.find(*_value_string);
        return (find_index < argv_value.size() && find_index >= 0);
    }
    return false;
}


const bool Json_value::operator > (const int& argv_value)
{    return ((_value_type == Type::Int && _value_int > argv_value) || (_value_type == Type::Double && _value_double > argv_value)); }

const bool Json_value::operator > (const double& argv_value)
{    return (( _value_type == Type::Double && _value_double > argv_value) || (_value_type == Type::Int && _value_int > argv_value));    }

const bool Json_value::operator > (const char* argv_value)
{    return *this == (std::string(argv_value)); }

const bool Json_value::operator > (const std::string& argv_value)
{
    if (_value_type == Type::String)
    {
        int find_index = _value_string->find(argv_value);
        return (find_index < _value_string->size() && find_index >= 0);
    }
    return false;
}



//对外函数
/*************************************************************************************************************************************************************/

const Type& Json_value::value_type()
{    return _value_type;    }

const bool Json_value::is_bool()
{    return (_value_type == Type::Bool);  }

const bool Json_value::is_int()
{    return (_value_type == Type::Int);  }

const bool Json_value::is_double()
{    return (_value_type == Type::Double);  }

const bool Json_value::is_string()
{    return (_value_type == Type::String);  }

const bool Json_value::is_array()
{    return (_value_type == Type::Array);   }

const bool Json_value::is_object()
{    return (_value_type == Type::Object);  }


void Json_value::insert(Json_value argv_value)
{
    if (_value_type != Type::Array)
    {
        clear();
        _value_type = Type::Array;
        _value_array = new std::vector<Json_value>;
    }
    _value_array->emplace_back(argv_value);
}

void Json_value::insert(std::pair<std::string, Json_value> argv_value)
{
    if (_value_type != Type::Object)
    {
        clear();
        _value_type = Type::Object;
        _value_object = new std::map<std::string, Json_value>;
    }
    _value_object->emplace(argv_value);
}

Json_value Json_value::operator[] (const int& argv_index)
{
    if (_value_type  == Type::Array && _value_array->size() <= argv_index)
        return _value_array[argv_index];
    return Json_value();
}

Json_value Json_value::operator[] (const char* argv_key)
{    return (*this)[std::string(argv_key)];    }

Json_value Json_value::operator[] (const std::string& argv_key)
{
    if (_value_type == Type::Object && _value_object->count(argv_key) == 1)
        return (*_value_object)[argv_key];
    return Json_value();
}


void Json_value::clear()
{
    if (_value_type == Type::Nullptr)
        return;
    //if (_value_type == Type::Int)
    //    _value_int = 0;
    //else if (_value_type == Type::Double)
    //    _value_double = 0.0;
    _value_bool = false;
    _value_int = 0;
    _value_double = 0.0;
    if (_value_type == Type::String)
        delete(_value_string);
    else if (_value_type == Type::Array)
        delete(_value_array); 
    else if (_value_type == Type::Object)
        delete(_value_object);

    _value_type = Type::Nullptr;
}



//私有函数
/*************************************************************************************************************************************************************/

void Json_value::copy(const Json_value& argv_value)
{
    if (argv_value._value_type == Type::Nullptr)
    {   clear(); return;    }

    _value_type = argv_value._value_type;
    _value_bool = argv_value._value_bool;
    _value_int = argv_value._value_int;
    _value_double = argv_value._value_double;
    if (_value_type == Type::String)
        _value_string = new std::string(*(argv_value._value_string));
    else if (_value_type == Type::Array)
        _value_array = new std::vector(*(argv_value._value_array)); 
    else if (_value_type == Type::Object)
        _value_object = new std::map<std::string, Json_value>(*(argv_value._value_object));
}

void Json_value::move(Json_value&& argv_value)
{
    if (argv_value._value_type == Type::Nullptr)
    {    clear(); return;    }

    _value_type = argv_value._value_type;
    _value_bool = argv_value._value_bool;
    _value_int = argv_value._value_int;
    _value_double = argv_value._value_double;
    if (_value_type == Type::String)
        _value_string = argv_value._value_string, argv_value._value_string = nullptr;
    else if (_value_type == Type::Array)
        _value_array = argv_value._value_array, argv_value._value_array = nullptr; 
    else if (_value_type == Type::Object)
        _value_object = argv_value._value_object, argv_value._value_object = nullptr;
}

/*************************************************************************************************************************************************************/






//serialization
//构造函数
/*************************************************************************************************************************************************************/
Serialization::Serialization()
    : _json_str(), _json_str_index(0), _parser_succeed(false)
{}

Serialization::Serialization(const Serialization& argv_Serialization)
    : Serialization()
{    copy(argv_Serialization);    }

Serialization::Serialization(Serialization&& argv_Serialization)
    : Serialization()
{    move(std::move(argv_Serialization));    }

const Serialization& Serialization::operator = (const Serialization& argv_Serialization)
{    clear(); copy(argv_Serialization); return (*this);    }

const Serialization& Serialization::operator = (Serialization&& argv_Serialization)
{    clear(); move(std::move(argv_Serialization)); return (*this);    }




//公开函数
/*************************************************************************************************************************************************************/

const Json_value Serialization::parser(const std::string& argv_str)
{   _json_str = argv_str; skip_space(); return parser();    }

const Json_value Serialization::parser(const char* argv_str)
{   return parser(std::string(argv_str));    }

const std::string Serialization::parser(const Json_value& argv_value)
{
    std::stringstream tmp_sstring;
    tmp_sstring << argv_value;
    _json_str = tmp_sstring.str();

    return _json_str;
}

const bool& Serialization::parser_succeed()
{   return _parser_succeed;    }

void Serialization::clear()
{   _json_str.clear(); _json_str_index = 0; _parser_succeed = false;    }



//私有函数
/*************************************************************************************************************************************************************/
void Serialization::skip_space()
{
    for (; (_json_str.size() >= _json_str_index) && (_json_str[_json_str_index] == ' ' || _json_str[_json_str_index] == '\r' || _json_str[_json_str_index] == '\n'|| _json_str[_json_str_index] == '\t'); )
        ++_json_str_index;
}

const char& Serialization::get_nospace_ch()
{   skip_space(); return _json_str[_json_str_index];    }

const Json_value Serialization::parser()
{
    skip_space();

    if ((_json_str[_json_str_index] == 'N' || _json_str[_json_str_index] == 'n'))
        return parser_Nullptr();
    else if ((_json_str[_json_str_index] == 'T' || _json_str[_json_str_index] == 't'))
        return parser_Bool_True();
    else if ((_json_str[_json_str_index] == 'F' || _json_str[_json_str_index] == 'f'))
        return parser_Bool_False();
    else if ((_json_str[_json_str_index] >= '0' && _json_str[_json_str_index] <= '9') || _json_str[_json_str_index] == '-')
        return parser_Int_Doublue();
    else if (_json_str[_json_str_index] == '\"')
        return parser_String();
    else if (_json_str[_json_str_index] == '[')
        return parser_Array();
    else if (_json_str[_json_str_index] == '{')
        return parser_Object();
    return Json_value();
}

const Json_value Serialization::parser_Nullptr()
{
    if ((_json_str.size() - _json_str_index >= 4) && (_json_str.compare(_json_str_index, 4, "NULL") || _json_str.compare(_json_str_index, 4, "Null") || _json_str.compare(_json_str_index, 4, "null")))
    {   _parser_succeed = true; _json_str_index +=4;    }

    _parser_succeed = true;
    return Json_value();
}

const Json_value Serialization::parser_Bool_True()
{
    if ((_json_str.size() - _json_str_index >= 4) && _json_str.compare(_json_str_index, 4, "TRUE") || _json_str.compare(_json_str_index, 4, "True") || _json_str.compare(_json_str_index, 4, "true"))
    {   _parser_succeed = true; _json_str_index += 4; return Json_value(true);    }
    
    _parser_succeed = true;
    return Json_value();
}

const Json_value Serialization::parser_Bool_False()
{
    if ((_json_str.size() - _json_str_index >= 5) && _json_str.compare(_json_str_index, 5, "FALSE") || _json_str.compare(_json_str_index, 5, "False") || _json_str.compare(_json_str_index, 5, "false"))
    {   _parser_succeed = true; _json_str_index +=5; return Json_value(false);    }

    _parser_succeed = true;
    return Json_value();
}

const Json_value Serialization::parser_Int_Doublue()
{
    int index_head = _json_str_index;
    int& index = _json_str_index;
    
    if (_json_str[index] == '-')
        ++index;
    
    for (; index < _json_str.size() && _json_str[index] >= '0' && _json_str[index] <= '9';)
        ++index;
    
    if (index == _json_str.size() || _json_str[index] != '.')
    {
        _parser_succeed = true;
        return Json_value(atoi(_json_str.c_str() + index_head));
    }
    else
    {
        ++index;
        for (; index < _json_str.size() && _json_str[index] >= '0' && _json_str[index] <= '9';)
            ++index;

        _parser_succeed = true;
        return Json_value(atof(_json_str.c_str() + index_head));
    }
}

const Json_value Serialization::parser_String()
{
    int index_head = ++_json_str_index;
    int& index = _json_str_index;

    for (; index < _json_str.size() && _json_str[index] != '\"';)
        ++index;
    
    if (_json_str[index] == '\"')
    {   ++index; _parser_succeed = true; return Json_value(std::string(_json_str.begin() +index_head, (_json_str.begin() +index -1)));    }
    
    _parser_succeed = true;
    return Json_value();
}

const Json_value Serialization::parser_Array()
{
    int& index = ++_json_str_index;
    Json_value tmp_Json_value(Type::Array);
    for (; index < _json_str.size() && _json_str[index] != ']';)
    {
        tmp_Json_value.insert(parser());
        skip_space();
        
        if (_json_str[index] == ',')
            ++index;
    }

    if (_json_str[index] == ']')
    {   ++index; _parser_succeed = true; return tmp_Json_value;    }

    _parser_succeed = true;
    return Json_value();
}

const Json_value Serialization::parser_Object()
{
    int& index = ++_json_str_index;
    Json_value tmp_json_value(Type::Object);
    Json_value tmp_key;
    Json_value tmp_value;
    
    for (; index < _json_str.size() && _json_str[index] != '}';)
    {
        tmp_key = parser();
        skip_space();

        if (_json_str[index] == ':')
            ++index;
        else
            return Json_value();
        
        tmp_value = parser();
        skip_space();

        if (_json_str[index] == ',')
            ++index;
        
        if (tmp_key.is_string())
            tmp_json_value.insert(std::pair<std::string, Json_value>(tmp_key, tmp_value));
    }

    if (_json_str[index] == '}')
    {   ++index; _parser_succeed = true; return tmp_json_value;    }
    
    
    return Json_value();
}


void Serialization::copy(const Serialization& argv_Serialization)
{
    _json_str = argv_Serialization._json_str;
    _json_str_index = argv_Serialization._json_str_index;
    _parser_succeed = argv_Serialization._parser_succeed;
}

void Serialization::move(Serialization&& argv_Serialization)
{
    _json_str = std::move(argv_Serialization._json_str);
    _json_str_index = argv_Serialization._json_str_index;
    _parser_succeed = argv_Serialization._parser_succeed;
    argv_Serialization.clear();
}

/*************************************************************************************************************************************************************/






//Json
//构造和析构函数
/*************************************************************************************************************************************************************/

Json::Json()
    : _value(Type::Nullptr)
{}

Json::Json(const Json& argv_json)
    : Json()
{   copy(argv_json);    }

Json::Json(Json&& argv_json)
    : Json()
{   move(std::move(argv_json));    }


Json::Json(const Json_value& argv_valeu)
    : _value(argv_valeu)
{}

Json::Json(const Type& argv_json_type)
    : _value(argv_json_type)
{}

Json::Json(const std::string& argv_str, const Json_create_mode& argv_create_mode)
    : Json()
{
    if (argv_create_mode == Json_create_mode::String_create)
    {
        _value = _serialization.parser(argv_str);
    }
    else if (argv_create_mode == Json_create_mode::File_create)
    {
        std::ifstream file_fd(argv_str, std::ios::in);
        if (file_fd.is_open())
        {
            std::string tmp_json_str;
            while(file_fd.eof())
            {
                char file_buff[1024]{0};
                file_fd.read(file_buff, 1024);
                tmp_json_str += file_buff;
            }
            _value = _serialization.parser(tmp_json_str);
        }
    }
}

const Json& Json::operator = (const Json& argv_json)
{    clear(); copy(argv_json); return (*this);    }

const Json& Json::operator = (Json&& argv_json)
{    clear(); move(std::move(argv_json)); return (*this);    }



//私有函数
/*************************************************************************************************************************************************************/

void Json::clear()
{   _value.clear(); _serialization.clear();    }

void Json::copy(const Json& argv_json)
{   _value = argv_json._value; _serialization = argv_json._serialization;    }

void Json::move(Json&& argv_json)
{   _value = std::move(argv_json._value); _serialization = std::move(argv_json._serialization);    }



//公开函数
/*************************************************************************************************************************************************************/

const Json_value& Json::get_value()
{   return _value;    }

void Json::set_value(const Json_value& argv_value)
{   _value = argv_value;    }

//const Json_value& Json::get_value(int argv_index)
//{   return (_value[argv_index]);    }
//
//const Json_value& Json::get_value(const std::string& argv_key)
//{   return (_value[argv_key]);    }


void Json::set_value(const Type& argv_json_type)
{   _value = Json_value(argv_json_type);    }

void Json::set_value(const std::string& argv_str, const Json_create_mode& argv_create_mode)
{
    if (argv_create_mode == Json_create_mode::String_create)
    {
        _value = _serialization.parser(argv_str);
    }
    else if (argv_create_mode == Json_create_mode::File_create)
    {
        std::ifstream file_fd(argv_str, std::ios::in);
        if (file_fd.is_open())
        {
            std::string tmp_json_str;
            while(!file_fd.eof())
            {
                char file_buff[1024]{0};
                file_fd.read(file_buff, 1024);
                tmp_json_str += file_buff;
            }
            _value = _serialization.parser(tmp_json_str);
        }
    }   
}

/*************************************************************************************************************************************************************/




}


