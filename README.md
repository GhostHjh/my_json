## 这是一个很简单的 json 解析器

# 1.json值的类型

   > * 基础类型
   >   ```c++
   >   //m_json()一共有 7 种类型
   >   json_null     为    nullptr (空类型)
   >   json_bool     为    bool    (布尔类型)
   >   json_int      为    int     (整数类型)
   >   json_double   为    double  (浮点类型)
   >   json_string   为    string  (字符串类型)
   >   json_vector   为    vector  (json类型的不定长数组)
   >   json_obj      为    Object  (json类型的对象, 由string类型做key, json类型做value)
   >
   >   ```
   >
# 2.构造方式

   > * 使用 __类型__ 构造
   >
   >   ```c++
   >   //type_t为一个内置的枚举, 可以拥有值 json_null, json_bool, json_int, json_double, json_string, json_vector, json_obj)
   >   m_json(type_t); 
   >
   >
   >   //可以直接使用变量进行构造
   >   m_json(true);
   >   m_json(123);
   >   m_json(123.123);
   >   m_json("string");
   >
   >
   >   //也可以使用 "=" 来构造(注意, 使用这个方法时, 会销毁原来的值)
   >   m_json() = true;
   >   m_json() = 123;
   >   m_json() = 123.123;
   >   m_json() = "string";
   >
   >   //创建 vector 类型
   >   m_json()[int值] = m_json();
   >
   >   //创建 Object 类型
   >   m_json()[string值] = m_json();
   >
   >   ```
   > * 使用 __json格式字符__ 串构造
   >
   >   ```c++
   >   //使用一段json字符串构造, json将读取这个字符串并解析它
   >   m_json().str_to_json("json格式字符串");
   >   ```
   > * 使用 __json格式文件__ 构造
   >
   >   ```c++
   >   //使用一个json文件来构造, json将读取这个文件并解析它
   >   m_json().file_to_json("json文件路径");
   >   ```
   >
# 3.获取值的方法

   > * 使用 "=" 获取值
   >
   >   ```c++
   >   //直接使用 "=" 来获取基础类型的值
   >   bool   tmp_bool   = m_json(ture);
   >   int    tmp_int    = m_json(123);
   >   double tmp_double = m_json(123.123);
   >   string tmp_string = m_json("string");
   >
   >
   >   //获取 "数组" 或者 "Object" 中的值
   >   int    tmp_int    = m_json()[int值];     //数组
   >   int    tmp_int    = m_json()["string值"] //Object
   >   ```
   > * 使用自带的函数来获取值
   >
   >   ```c++
   >   bool   tmp_bool   = m_json(ture).to_bool();
   >   int    tmp_int    = m_json(123).to_int();
   >   double tmp_double = m_json(123.123).to_double();
   >   string tmp_string = m_json("string").to_str();
   >   ```
   >
# 4.显示当前json对象的内容

   > * 直接显示
   >
   >   ```c++
   >   m_json().show();
   >   ```
   > * 获取内容为有格式的字符串
   >
   >   ```c++
   >   string tmp_str = m_json().str();
   >   ```
   >
