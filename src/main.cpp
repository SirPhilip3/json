#include "j_consegna.cpp"
// #include "json.hpp"
//! AGGIUNGERE CONTROLLI PER GRAMMATICA ERRATA
#include <chrono>
#include <sstream>
#include <algorithm>
using namespace std::chrono;
#include <utility>
#include <cassert>

int main(){
    // /*
    try
    {
        json j;
        cin>>j;
        cout<<j;
    }
    catch(const json_exception& e)
    {
        std::cout << e.msg<< '\n';
    }
    // */
    /*
    try{
    std::string newFile;
    for (int i=1; i<37;i++){
        // if(i==){
        newFile = "/home/filippo/projects_c++/json/src/output/test_of/test/"+std::to_string(i)+".json";
        // newFile = "/home/filippo/projects_c++/json/src/output/test_ceck_comma/"+std::to_string(i)+".json";
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
        std::ofstream out("/home/filippo/projects_c++/json/src/output/test_of/test_output/"+std::to_string(i)+".json");
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
    */
    





















    // json original;
    // try {
    //     //{"K":54654,"j":435,"p":[45,456,12313,567,"cioa",true,false,null],"l":"v"}
    //     cin>>original; 
    // }catch (const json_exception& exc){
    //     std::cout<<"exception :"<<exc.msg<<endl;
    // }
    // try
    // {
    //     json& y = *(++original.begin_list());
    //     std::cout << y["quarta chiave"]["a"];
    //     // json z;
    //     // z.set_dictionary();
    //     // std::pair<string, json> tmp;
    //     // tmp.first = "c";
    //     // tmp.second.set_number(5);
    //     // z.insert(tmp);
    //     // tmp.first ="d";
    //     // tmp.second.set_number(6);
    // }
    // catch(const json_exception& e)
    // {
    //     std::cout << e.msg << '\n';
    // }
    // try
    // {
    //     json j;
    //     // j.set_list();
    //     cin>>j;
    //     // json tmp;2
    //     // tmp.set_number(324);
    //     // // j.push_back(tmp);
    //     // // cout<<j<<endl;
    //     // // j.set_bool(true);
    //     // // // cin>>j;
    //     // j.set_list();
    //     // j.push_back(tmp);
    //     // tmp.5set_number(24);
    //     // j.push_back(tmp);
    //     // const json k = j;
    //     // cout<<j<<endl;
    //     cout<<j;

    //     // json& y = *(++j.begin_list());
    //     // std::cout << y["quarta chiave"]["a"];
    //     // json z;
    //     // z.set_dictionary();
    //     // pair<string, json> tmp;
    //     // tmp.first = "c";
    //     // tmp.second.set_bool(true);
    //     // z.insert(tmp);
    //     // tmp.first = "d";
    //     // tmp.second.set_bool(false);
    //     // z.insert(tmp);
    //     // cout<<z<<"z\n";
    //     // (*(++j.begin_list()))["prima chiave"] = z;
    //     // json moved = move(z);
    //     // cout<<moved<<"moved";
    //     // z.set_list();
    //     // json tmo;
    //     // tmo.set_number(4);
    //     // z.push_back(tmo);
    //     // tmo.set_bool(true);
    //     // z.push_front(tmo);5
    //     // tmo.set_string("nfe");
    //     // z.push_back(tmo);
    //     // cout<<"list z"<<z<<endl;
    // }
    // catch(const json_exception& e)
    // {
    //     std::cout << e.msg << '\n';
    // }
    
    // json j;
    // std::cin >> j;
    // json& y = *(++j.begin_list());
    // std::cout << y["quarta chiave"]["a"];
    // json z;
    // z.set_dictionary();
    // pair<string, json> tmp;
    // tmp.first = "c";
    // tmp.second.set_number(5);
    // z.insert(tmp);
    // tmp.first = "d";
    // tmp.second.set_number(6);
    // z.insert(tmp);
    // (*(++j.begin_list()))["prima chiave"] = z;

    
    
    // json moved; //! non crea uninitialized value
    // try
    // {
    //     json moved = move(original);
    // }
    // catch(const json_exception& e)
    // {
    //     std::cout<<"exception :"<<e.msg<<endl;
    // }
      //! crea uninitialized values

    // sort(j.begin_list(), j.end_list());
    // cout<<j;

    
    
    // json j;
    // auto start = high_resolution_clock::now();
    // try{
    //     cin>>j;
    // }catch (const json_exception& exc){
    //     std::cout<<"exception :"<<exc.msg<<endl;
    // }
    // auto stop = high_resolution_clock::now();
	
	// auto duration = duration_cast<microseconds>(stop - start);

	// std::cout << "Read : "<< duration.count() << " microseconds" << endl;
    
    // auto start2 = high_resolution_clock::now();

    // std::ofstream out("output.json");
    // try{
    //     out<<j;
    // }catch (const json_exception& exc){
    //     cout<<"exception :"<<exc.msg<<endl;
    // }
    // auto stop2 = high_resolution_clock::now();
	
	// auto duration2 = duration_cast<microseconds>(stop2 - start2);

	// std::cout << "Write : "<< duration2.count() << " microseconds" << endl;
    

    return 0;
}