// #include "test2.cpp"
#include "j_consegna.cpp"
// #include "json_prec.cpp"
// #include "json.hpp"
//! AGGIUNGERE CONTROLLI PER GRAMMATICA ERRATA
#include <chrono>
#include <sstream>
#include <algorithm>
using namespace std::chrono;
#include <utility>
#include <cassert>
#include <iomanip>

using namespace std;

int main(){

try
{
    json data;
    int count = 50;
    double total=0;
    for (int i = 0; i < count; i++)
    {
        ifstream file("/home/filippo/projects_c++/json/src/output/other_tests/large-file.json");
        stringstream ss;
        ss<<file.rdbuf();
        std::ofstream out("/home/filippo/projects_c++/json/src/output/other_tests/large-file_output.json");
        auto start = high_resolution_clock::now();
        try{
            ss>>data;
        }catch (const json_exception& exc){
            out<<"exception :"<<exc.msg<<endl;
        }
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.close();
        auto stop = high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<microseconds>(stop - start);
        double transferRate = static_cast<double>(fileSize) / (duration.count() * 1e6); 
        total+=transferRate;
        std::cout << std::fixed << std::setprecision(10);
        std::cout << i+1 <<": Data transfer rate: " << transferRate << " MB/s" << std::endl;
    }

    double median = total/count;
    std::cout << "Data transfer rate median: " << median << " MB/s" << std::endl;



}
catch(const json_exception& e)
{
    std::cout << e.msg << '\n';
}



/*
    try
    {
        json j;
        // j.set_list();
        // json k;
        // k.set_number(324);
        // j.push_back(k);
        // j.push_back(k);
        cin>>j;
        // json n = j;
        // cout<<"j: "<<j<<endl;
        // cout<<"n: "<<n<<endl;
    }
    catch(const json_exception& e)
    {
        std::cerr << e.msg << '\n';
    }
    
// */
    /*
    try
    {    
        json j;
        ifstream file("/home/filippo/projects_c++/json/src/output/custom_test/rewrite/55.json");
        stringstream ss;
        ss<<file.rdbuf();
        ss>>j;
        json k;
        cout<<"--k: "<<k<<endl;
        cout<<"--j: "<<j<<endl;
        // cout<<sizeof(j)<<endl;
        // k=j;
        // cout<<"k: "<<k<<endl;
        // cout<<"j: "<<j<<endl;
        // k=j;
        // cout<<"--k: "<<k<<endl;
        // cout<<"--j: "<<j<<endl; 
        k=move(j);
        // cout<<"--k: "<<k<<endl;
        cout<<"--k: "<<k<<endl;
    }
    catch(const json_exception& e)
    {
        cout << e.msg << '\n';
    }
    */
 /*

    json j;

    try{
        ifstream file("/home/filippo/projects_c++/json/src/output/custom_test/rewrite/55.json");
        stringstream ss;
        ss<<file.rdbuf();
        cout<<"reading"<<endl;
        ss>>j;
        json k;
        k=move(j);
        cout<<"writing"<<endl;
        ofstream out("/home/filippo/projects_c++/json/src/output/custom_test/rewrite/out.json");
        out<<k;
        // cout<<j<<endl;
        out.close();
        file.close();
        // cin>>j;
    }
    catch(const json_exception& e)
    {
        std::cout << e.msg<< endl;
    }

    try//!!!!
    {
        ifstream file("/home/filippo/projects_c++/json/src/output/custom_test/rewrite/out.json");
        stringstream ss;
        ss<<file.rdbuf();
        cout<<"reading"<<endl;
        ss>>j;
        json k;
        k=move(j);
        cout<<"writing"<<endl;
        ofstream out("/home/filippo/projects_c++/json/src/output/custom_test/rewrite/55.json");
        out<<k;
        // cout<<j;
        out.close();
        file.close();
        // json k;
        // k=move(j);
        // cout<<k;
    }
    catch(const json_exception& e)
    {
        std::cout << e.msg << '\n';
    }

    // */



    /*
    try{
    std::string newFile;
    for (int i=1; i<101;i++){
        // if(i==){
        newFile = "/home/filippo/projects_c++/json/src/output/custom_test/json_test/"+std::to_string(i)+".txt";
        // newFile = "/home/filippo/projects_c++/json/src/output/test_of/test/"+std::to_string(i)+".json";
        // newFile = "/home/filippo/projects_c++/json/src/output/test_open/"+std::to_string(i)+".json";
        std::cout<<"Apro il file "<<i<<std::endl;
        std::ifstream file(newFile);
        if (!file.is_open()) {
            std::cout << "Failed to open the file." << std::endl;
            return 1;
        }
        std::stringstream ss;
        ss << file.rdbuf();
        file.close();

        json data;
        std::ofstream out("/home/filippo/projects_c++/json/src/output/custom_test/test_output/"+std::to_string(i)+".json");
        auto start = high_resolution_clock::now();
        try{
            ss>>data;
        }catch (const json_exception& exc){
            out<<"exception :"<<exc.msg<<endl;
        }
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        std::cout << "Read : "<< duration.count() << " microseconds" << endl;
        
        auto start2 = high_resolution_clock::now();
        try{
            out << data << std::endl;
        }catch (const json_exception& exc){
            out<<"exception :"<<exc.msg<<endl;
        }
        auto stop2 = high_resolution_clock::now();
        auto duration2 = duration_cast<microseconds>(stop2 - start2);
        std::cout << "Write : "<< duration2.count() << " microseconds" << endl;
        out.close();
        // }
    }
    }catch(const json_exception& e){
        cout<<e.msg<<endl;
    }
    // */

    /*
    try{
        json j;
        std::cin >> j;
        cout<<j<<endl;
        json& y = *(++j.begin_list());
        std::cout << y["quarta chiave"]["a"];
        json z;
        z.set_dictionary();
        pair<string, json> tmp;
        tmp.first = "c";
        tmp.second.set_number(5);
        z.insert(tmp);
        tmp.first = "d";
        tmp.second.set_number(6);
        z.insert(tmp);
        // cout<<z<<"z\n";
        (*(++j.begin_list()))["prima chiave"] = z;
        cout<<endl;
        cout<<j<<endl;
        
    }catch(const json_exception& e){
        std::cout << e.msg << '\n';
    }
    // */

    return 0;
}