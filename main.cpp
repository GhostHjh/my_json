#include <iostream>
#include <fstream>
//#include "str_json.hpp"
//#include "json.hpp"
#include "to_json.hpp"

using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;
using std::filebuf;



int main(int argc, char** argv)
{

    //ifstream file_read;
    //ofstream file_write;
    //file_read.open("tmp.txt");
    //file_write.open("tmp.txt", std::ios::out | std::ios::app);
    
    

    //file_read.clear();
    //file_write.clear();
    // json tmp_json();
    // tmp_json.str_to_json("1111");
    // cout << tmp_json.get_type() << endl;
    // cout << tmp_json.str() << endl;

    // json m_json = to_json(" [ 123, 222, 333 ] ").str_to_json();
    // m_json.show();
    //to_json("  {  \"aaa\"  :123.7  ,   \"bbb\"   :456,\"ccc\":789, \"ddd\":[ 111, 222, \"444\"] , \"eee\" :  \"value\"}").str_to_json().show();
    json m_json = to_json("G:\\cx\\my_json\\tmp.txt").file_to_json();
    if (m_json["employee"]["married"] == true)
        cout << "成功\t" << m_json["employee"]["married"].str() << endl;
    m_json.show();


    //json m_json = json(true);


    return 0;
}




