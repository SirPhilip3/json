#include "json.hpp"

using namespace std;

struct El{
    double num;
	bool boo;
	std::string s;
    bool Null;//? valore nullo

    json list; // la lista è un json

    struct Dict{
        std::pair<std::string, json> El_dic; //dictionary
        El* next;
    };
    Dict dic;
   
    El* next;
};

typedef El* pEl;

struct json::impl{
    pEl head;
    pEl tail;
    // leggere un elemento json basta append alla fine
};


// default constructor
json::json(){
    pimpl = new impl;
    pimpl->head=nullptr;
    pimpl->tail=nullptr;
}

json::~json(){
    pimpl->head=nullptr; // dummy solo per fara andare il programma
}




json&  E(std::istream& is, json& j);

json& get_lista(std::istream& is, json& j){
    // legge una lista può leggere dei dizionari
    // finchè non si chiude la lista

    char p = 0;
    if(is) p=is.peek(); // per peek deve essere valido
    else return j;
    if(p!=']'){
        // is.putback(p);
        // cout<<"inside list"<<endl;
        E(is, j);
        get_lista(is, j);
    }

    return j;
}

json& get_dictionary(istream& is, json& j){
    // legge un pair di key e El che può essere dicitionary e lista se 
    char p = 0;
    if(is) p=is.peek();
    else return j;
    if(p!='}'){
        // is.putback(p);
        // leggi stringa poi leggi il valore legato a quella stringa
        // cout<<"inside dic"<<endl;
        E(is, j);
        get_dictionary(is, j);
    }
    
    return j;
}

json& number(istream& is, json& j){
  // is.putback(p);
    double num = 0;
    is>>num;
    std::cout<<num<<" --num"<<'\n';



    // while(is){

    // }
    is.ignore(10, ','); // ignora il rest0 , spazi vuoti etc fino a ,
    // j.set_number(num);
}

json& strings(istream& is, json& j){
    std::string s;
    char p=0;
    bool is_acceptable=false;
    while(p!='"'||is_acceptable){
        // is_acceptable=false;
        is>>p;
        s+=p;
        if(p=='\\'){
            if(is.peek()=='"'){
                is_acceptable=true;
            }    
        }

        if(p=='"'&&is_acceptable) {is_acceptable=false; p=0;}
    }
    // std::getline(is, s);// non va bene devo leggere ogni singolo char fino ad una " senza / davanti 
    s.erase(s.length()-1);
    std::cout<<"--"<<s<<'\n';
    is.ignore(10, ',');// ignora il rest0 , spazi vuoti etc fino a ,
}

json& null(istream& is, json& j){
    std::cout<<"--null"<<'\n'; // skippo alla prossima riga
    is.ignore(40,','); // ignoro i prossimi char che saranno la critta null e passo alla prossima linea
}

json& boolean(istream& is, json& j){
    char p=0;
    is>>p;
    // use is.boolalpha
    if(p=='t'){
        std::cout<<"--true"<<'\n';
        is.ignore(40,',');// ignora il rest0 , spazi vuoti etc fino a ,
    } 
    else if(p=='f'){
        std::cout<<"--false"<<'\n';
        is.ignore(50, ',');// ignora il rest0 , spazi vuoti etc fino a ,
    }
}

json& E(istream& is, json& j){
    // leggo un elemento della lista / dizionario
    char p=0;
    is>>p;
    if(p>='0' && p<='9'){
        is.putback(p);
        number(is, j);
    }else if(p=='['){
        std::cout<<"--["<<'\n';
        get_lista(is, j);
        std::cout<<"--]"<<'\n';
    }else if(p=='{'){
        std::cout<<"--{"<<'\n';
        get_dictionary(is, j);
        std::cout<<"--}"<<'\n';
    }else if(p!='"'){
        // o è null o è un buleano
        if (p=='n'){
            null(is, j);
        } 
        else {
            is.putback(p);
            boolean(is, j);
        }
    }else{
        strings(is, j);
    }

    return j;
}


// cin
istream& operator>>(istream& is, json& j){

    char p = 0;
    is >> p;
    if(p=='['){
        get_lista(is, j);
    }else if(p=='{'){
        get_dictionary(is, j);
    }
    return is;
}

int main(){
    json j;
    while (cin)
    {
        cin>>j;
    }
    return 0;
}