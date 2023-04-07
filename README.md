# 一个 json 对象

## 目录结构

> ```shell
> │  CMakeLists.txt
> │  main.cpp
> │
> ├─include
> │      Json.h
> │
> └─src
>         Json.cpp
> ```

## 使用解析

### 0. 介绍
>
> 1. Json_vale
> > __Json_value__ 是用于存储值的对象
> 2. Serialization
> > __Serialization__ 是一个用于将 "字符串" 解析为 "__Json_value__" 的工具
> 3. Json
> > __Json__ 则是一个将 __Json_value__ 和 __Serialization__ 都封好, 且添加了新功能的对象

### 1.json值的类型

> * 基础类型
>   ```c++
>   //Json()一共有 7 种类型
>   Type::Nullptr  为    nullptr                            (空类型)
>   Type::Bool     为    bool                               (布尔类型)
>   Type::Int      为    int                                (整数类型)
>   Type::Double   为    double                             (浮点类型)
>   Type::String   为    std::string                        (字符串类型)
>   Type::Array    为    vector<value>                 (value不定长数组)
>   Type::Object   为    std::map<std::string, value>  (value对象, 由string类型做key, value类型做value)
>
>   ```

### 2.构造方式

> * 使用 __类型__ 构造
>
>   ```c++
>   //Type为一个内置的枚举, 可以拥有 Nullptr, Bool, Int, Double, String, Array, Object)
>   Json(Type _type); 
>
>
>   //可以直接使用变量进行构造
>   Json(true);
>   Json(123);
>   Json(123.123);
>   Json("string");
>
>
>   //也可以使用 "=" 来构造或者重新赋值
>   Json() = true;
>   Json() = 123;
>   Json() = 123.123;
>   Json() = "string";
>
>   //创建 vector 类型
>   Json(std::vecotr<Json()>);
>
>   //创建 Object 类型
>   Json(std::map<std::string, Json_value>);
>
>   ```
> * 使用 __json格式字符串__ 或 使用包含 __json格式字符串的文件__ 串构造
>
>   ```c++
>   //使用一段json字符串构造, json将读取这个字符串并解析它
>   //Json_create_mode为一个枚举, 它有两个值, String_create, File_create
>   //分别表示将前面的字符串认定为 Json格式字符串, 保存了Json格式字符串的文件
>   Json("字符串", Json_create_mode _mode);
>   ```

### 3.获取值的方法

> * 使用 "=" 获取值
>
>   ```c++
>   //直接使用 "=" 来获取基础类型的值
>   bool   tmp_bool   = Json(ture);
>   int    tmp_int    = Json(123);
>   double tmp_double = Json(123.123);
>   string tmp_string = Json("string");
>
>
>   //获取 "数组" 或者 "Object" 中的值
>   int    tmp_int    = Json()[int值];     //数组
>   int    tmp_int    = Json()["string值"] //Object
>   ```
> * 使用自带的函数来获取值
>
>   ```c++
>   bool   tmp_bool   = Json(ture).get_value();
>   int    tmp_int    = Json(123).get_value();
>   double tmp_double = Json(123.123).get_value();
>   string tmp_string = Json("string").get_value();
>   ```

### 4.显示当前json对象的内容

> * 直接显示
>
>   ```c++
>   //Json_value对 << 做了运算符重载
>   std::cout << Json().get_value();
>   ```
> * 获取Json_value为字符串
>
>   ```c++
>   string tmp_str = Json().get_jsonstr();
>   ```
