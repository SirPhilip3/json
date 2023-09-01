#include <iostream>
using namespace std;

short int check_open_closed(const char item, string& LIFO){
    if(LIFO.empty()){
        if(item==']'||item=='}'){
            cout<<"launch error"<<endl;
        }else LIFO+=item;
    }else{
        if(LIFO.back()==item-2){
            LIFO.erase(LIFO.size()-1);
        }else if(item-LIFO.back()==34||item-LIFO.back()==-34){
            return 2;
        }else LIFO+=item;
        cout<<LIFO+'\n';
        if(LIFO.empty()) return 0;// empty = 0
        else return 1; // not empty = 1
    }

}


string kl (string j){
    string ceck;
    for(auto i: j){
        if(check_open_closed(i,ceck)==2) return "error";
    }

    
    return "ok";
}


int main(){
    string k;
    cin>>k;
    string m = kl(k);
    cout<<m;

}