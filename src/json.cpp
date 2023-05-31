#include "json.hpp"

using namespace std;

// struct Dict{
//     Dict* next;
// };

struct El{
    bool i_num=false;
    double num;
	int boo=-1;
	std::string s;
    bool Null=false;//? valore nullo
    std::pair<std::string, json> dic; //dictionary

    json list; // la lista è un json
    json Dic;

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
//! openai
    // if (pimpl != nullptr) {
    //     pEl pc = pimpl->head;
    //     pEl next = nullptr;

    //     while (pc != nullptr) {
    //         next = pc->next;
    //         delete pc;
    //         pc = next;
    //     }

    //     pimpl->head = nullptr;
    //     pimpl->tail = nullptr;
    //     delete pimpl;
    //     pimpl = nullptr;
    // }


//! originale
    if(pimpl!=nullptr){
        pEl pc = pimpl->head;
        
        while (pc!=nullptr)
        {
            pc=pc->next;
            delete pimpl->head;
            pimpl->head = pc;
        }

        pimpl->head = nullptr;
        pimpl->tail = nullptr;

        delete pimpl;
        pimpl=nullptr;
    }

}

//copy constructor
json::json(json const& j){
    
    pimpl = new impl;
    pimpl->head=nullptr;
    pimpl->tail=nullptr;

    pEl pc = j.pimpl->head; 
    while (pc!=nullptr)
    {
        if(is_list()) push_front(j);
        else insert(j.pimpl->head->dic);
        pc=pc->next;
    }
    
}

// mouve constructor
json::json(json&& j){
    *this=move(j);
}

json& json::operator=(json const& j){

    if(this != &j){

        pEl tmp = pimpl->head;
        while (tmp!=nullptr)
        {
            tmp = tmp->next;
            delete pimpl->head;
            pimpl->head=tmp;
        }
        
        pimpl->head=nullptr;
        pimpl->tail=nullptr;

        // if(pimpl->head==nullptr) push_front(j);
        // if(j.is_list()||j.is_bool()||j.is_null()||j.is_string()||j.is_number()){
        ////     if(pimpl->tail!=nullptr)
        ////         pimpl->tail->next=j.pimpl->head;
        ////     else
        ////         pimpl->head=j.pimpl->head;

        ////     pimpl->tail=j.pimpl->tail;
        // push_front(j); // push front in list di json //! tmp da capire cambia per list e dictionary
        // }else {
        //    // if(pimpl->tail->Dic.pimpl->tail!=nullptr)
        //    //     pimpl->tail->Dic.pimpl->tail->next=j.pimpl->head;
        //    // else
        //    //     pimpl->head->Dic.pimpl->head=j.pimpl->head;

        //    // pimpl->tail->Dic.pimpl->tail=j.pimpl->tail;

        //     if(pimpl->tail!=nullptr)
        //         pimpl->tail->next=j.pimpl->head;
        //     else
        //         pimpl->head=j.pimpl->head;

        //     pimpl->tail=j.pimpl->tail;
        // }

        if(pimpl->tail!=nullptr) //! questo ramo non viene mai eseguito
            pimpl->tail->next=j.pimpl->head;
        else
            pimpl->head=j.pimpl->head;

        pimpl->tail=j.pimpl->tail;

        // if(is_list()) push_front(j);
        // else insert(j.pimpl->head->dic);
    }

    return *this;

}

json& json::operator=(json&& j){
    if(this!=&j){
        delete pimpl;
        pimpl = j.pimpl;
        j.pimpl = nullptr;
    }
    return *this;
}

//! da ripensare is list e dictionary
bool json::is_dictionary() const{
    if(pimpl->head==nullptr) return true;
    return !pimpl->head->dic.first.empty();
}

bool json::is_list() const{
    if(pimpl->head==nullptr) return true;
    // if(pimpl->head->list.pimpl==nullptr) return false;
    // if(pimpl->head->list.pimpl->head==nullptr) return false;

    return pimpl->head->list.pimpl!=nullptr&&pimpl->head->list.pimpl->head!=nullptr; // se è stata inizializzata una lista allora è una lista 
}

bool json::is_string() const{
    return !pimpl->head->s.empty();
}

bool json::is_number() const{
    return pimpl->head->i_num;
}

bool json::is_bool() const{
    return pimpl->head->boo!=-1;
}

bool json::is_null() const{
    return pimpl->head->Null;
}

double& json::get_number(){

}

double const& json::get_number() const{

}

bool& json::get_bool(){

}

bool const& json::get_bool() const{

}

string& json::get_string(){

}

string const& json::get_string() const{

}


//! per ogni set dovrei eliminare lo sato del json 
void json::set_list(){

    // if(pimpl!=nullptr){
    //     pEl pc = pimpl->head;
        
    //     while (pc!=nullptr)
    //     {
    //         pc=pc->next;
    //         delete pimpl->head;
    //         pimpl->head = pc;
    //     }

    // }

    pEl n = new El;
    n->next=nullptr;
    n->list.pimpl->head=nullptr;
    n->list.pimpl->tail=nullptr;

    pimpl->head=n;
    pimpl->tail=n;

    //! n non posso eliminarlo

    // pimpl->head = new El;
    // pimpl->head->next=nullptr;
    // pimpl->head->list.pimpl->head=nullptr;
    // pimpl->head->list.pimpl->tail=nullptr;
    // pimpl->tail=pimpl->head;

}

void json::set_dictionary(){

    // if(pimpl!=nullptr){
    //     pEl pc = pimpl->head;
        
    //     while (pc!=nullptr)
    //     {
    //         pc=pc->next;
    //         delete pimpl->head;
    //         pimpl->head = pc;
    //     }

    // }
    
    pEl n = new El;
    n->next=nullptr;
    n->Dic.pimpl->head=nullptr;
    n->Dic.pimpl->tail=nullptr;

    pimpl->head=n;
    pimpl->tail=n;

}

void json::set_string(string const& x){
    //! json se lista pimpl alla head 
    // if(pimpl!=nullptr){
    //     pEl pc = pimpl->head;
        
    //     while (pc!=nullptr)
    //     {
    //         pc=pc->next;
    //         delete pimpl->head;
    //         pimpl->head = pc;
    //     }

    // }

    pEl n = new El;
    n->next=nullptr;
    n->s=x;

    pimpl->head=n;
    pimpl->tail=n;

    // if(pimpl->head==nullptr){
    //     pimpl->head=n;
    //     pimpl->tail=n;
    // }
    
}

void json::set_bool(bool x){
    
    // if(pimpl!=nullptr){
    //     pEl pc = pimpl->head;
        
    //     while (pc!=nullptr)
    //     {
    //         pc=pc->next;
    //         delete pimpl->head;
    //         pimpl->head = pc;
    //     }
    // }

    pEl n = new El;
    n->next=nullptr;
    n->boo=x;

    pimpl->head=n;
    pimpl->tail=n;

    
    // if(pimpl->head==nullptr){
    //     pimpl->head=n;
    // }
    // pimpl->head->boo=x;        
}

void json::set_number(double x){
    
    // if(pimpl!=nullptr){
    //     pEl pc = pimpl->head;
        
    //     while (pc!=nullptr)
    //     {
    //         pc=pc->next;
    //         delete pimpl->head;
    //         pimpl->head = pc;
    //     }

    // }

    pEl n = new El;
    n->next=nullptr;
    n->num=x;
    n->i_num=true;

    pimpl->head=n;
    pimpl->tail=n;

    // if(pimpl->head==nullptr){
    //     pimpl->head=n;
    // }
    // pimpl->head->num=x;  

}

void json::set_null(){
 
    // if(pimpl!=nullptr){
    //     pEl pc = pimpl->head;
        
    //     while (pc!=nullptr)
    //     {
    //         pc=pc->next;
    //         delete pimpl->head;
    //         pimpl->head = pc;
    //     }

    // }

    pEl n = new El;
    n->next=nullptr;
    n->Null=true;

    pimpl->head=n;
    pimpl->tail=n;
    
    // if(pimpl->head==nullptr){
    //     pimpl->head=n;
    // }
    // pimpl->head->Null=true;  
}


void json::push_front(json const& j){
    
    if(this==&j) return ;

    // pimpl->tail->list.pimpl->head;

    if(pimpl->tail->list.pimpl->tail!=nullptr)
        pimpl->tail->list.pimpl->tail->next=j.pimpl->head;
    else
        pimpl->head->list.pimpl->head=j.pimpl->head;

    pimpl->tail->list.pimpl->tail=j.pimpl->tail;

    // pimpl->tail->list.pimpl->tail->next=nullptr;

    // if(pimpl->head->list.pimpl);

}

void json::push_back(json const& j){

}

void json::insert(std::pair<std::string, json> const& d){

    pEl n = new El;
    n->next=nullptr;
    n->dic.first = d.first;
    n->dic.second = d.second;
    // if(d.second.is_list()||d.second.is_null()||d.second.is_bool()||d.second.is_string()||d.second.is_number()){
    //     n->dic.second.set_list();//!tmp
    //     // n->next = nullptr;
    //     n->dic.second.push_front(d.second);//!tmp deve cambiare a senconda di lista o no
    // }else {
    //     // std::cout<<"is disctionary "<<d.first<<std::endl;
    //     // n->dic.second.set_dictionary();
    //     // devo solo aggiungere d.second
    //     n->dic.second=d.second;
    // }

    // n->dic.second = d.second;
    std::cout<<n->dic.first<<std::endl;

    // json t;
    // t.pimpl->head=n;
    // t.pimpl->tail=n;
    // in tutto questo non sto modificando un casso 
    // if(this==&t) return ;

    // pimpl->tail->list.pimpl->head;

    if(pimpl->tail->Dic.pimpl->tail!=nullptr){
        pimpl->tail->Dic.pimpl->tail->next=n;
        // t.pimpl->head->dic.second.pimpl->head->list.pimpl->head->next=nullptr;//!wtf 
        }
    else
        pimpl->head->Dic.pimpl->head=n;
    pimpl->tail->Dic.pimpl->tail=n;

    // if(pimpl->head!=nullptr) pimpl->tail->next=n;
    // else 
    //     pimpl->head=n;  
    // pimpl->tail = n;
    return;
}


//! forward declaration potrebbe non servirmi
json E(std::istream& is);

json get_lista(std::istream& is){
    // legge una lista può leggere dei dizionari
    // finchè non si chiude la lista
    json j;
    json j_tmp;
    j.set_list();
    char p = 0;
    while(p!=']'){
        if(p!=','){
            if(p=='{'||p=='[') is.putback(p);
            j_tmp=E(is);
            j.push_front(j_tmp); // se ho una lista dentro una lista devo popolare list 
            j_tmp.set_list();
            // j.push_front(move(E(is)));
        }else is>>p;
        // get_lista(is);
        
        //! potrebbero essere inutili basta mettere a nullptr ma non posso accere a pimpl da metodo pubblico
        // j_tmp.set_dictionary();
        j_tmp.~json();

        if(is) p=is.peek(); // per peek deve essere valido
        else return j;
    }

    // j_tmp.~json();

    return j;
}

string dic_str(istream& is){
    std::string s;
    char p=0;
    bool is_acceptable=false;
    is>>p;
    if(p=='"') {p=0;}
    while(p!='"'||is_acceptable){
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
    is.ignore(10, ':');// ignora il rest0 , spazi vuoti etc fino a ,
    return s;
}

json get_dictionary(istream& is){
    // legge un pair di key e El che può essere dicitionary e lista se 
    json d;
    d.set_dictionary();
    std::pair<string , json> El_dict;
    char p = 0;

    while(p!='}'){
        // cout<<"inside dic"<<endl;
        if(p!=','){
            if(p=='{'||p=='[') is.putback(p);
            El_dict.first = dic_str(is);
            El_dict.second = E(is);
            //! all'ultimo passsaggio el_dic viene eliminato e quindi casino alla fine 
            //! stessa cosa per get_lista
            d.insert(El_dict);

            El_dict.second.set_dictionary();
            // d.insert({key,value});
        }else is>>p;// devo consumare ,

        //!fix
        //! non elimino la memoria

        // El_dict.second.set_list();
        El_dict.second.~json();

        if(is) p=is.peek();// si ferma a , bho
        else return d;
    }

    // El_dict.second.~json();

    return d;
}


json number(istream& is){
  // is.putback(p);
    json j;
    double num = 0;
    is>>num;
    std::cout<<num<<" --num"<<'\n';
    is.ignore(10, ','); // ignora il rest0 , spazi vuoti etc fino a ,
    j.set_number(num);
    return j;
}

json strings(istream& is){
    std::string s;
    json j;
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
    j.set_string(s); // potrei chiamarlo 
    is.ignore(10, ',');// ignora il rest0 , spazi vuoti etc fino a ,
    return j;
}

json null(istream& is){
    std::cout<<"--null"<<'\n'; // skippo alla prossima riga
    is.ignore(40,','); // ignoro i prossimi char che saranno la critta null e passo alla prossima linea
    json j;
    j.set_null();
    return j;
}

json boolean(istream& is){
    char p=0;
    json j;
    is>>p;
    // use is.boolalpha
    if(p=='t'){
        std::cout<<"--true"<<'\n';
        j.set_bool(true);
        is.ignore(40,',');// ignora il rest0 , spazi vuoti etc fino a ,
    } 
    else if(p=='f'){
        std::cout<<"--false"<<'\n';
        j.set_bool(false);
        is.ignore(50, ',');// ignora il rest0 , spazi vuoti etc fino a ,
    }
    return j;
}

json E(istream& is){
    // leggo un elemento della lista / dizionario
    char p=0;
    json j;
    is>>p;
    if(p=='\n') { is>>p;}
    if(p>='0' && p<='9'){
        is.putback(p);
        // j.push_front(number(is));
        j=number(is);

    }else if(p=='['){
        std::cout<<"--["<<'\n';
        // j.set_list();
        // j.push_front(move(get_lista(is)));
        j=get_lista(is);
        is>>p;
        std::cout<<"--]"<<'\n';
    }else if(p=='{'){
        std::cout<<"--{"<<'\n';
        // j.push_front(get_);
        j=get_dictionary(is);
        is>>p;
        std::cout<<"--}"<<'\n';
    }else if(p!='"'){
        // o è null o è un buleano
        if (p=='n'){
            // j.push_front(null(is));
            j=null(is);
        } 
        else {
            is.putback(p);
            // j.push_front(boolean(is));
            j=boolean(is);
        }
    }else{
        // j.push_front(strings(is));
        j=strings(is);

    }

    return j;
}

// cin
istream& operator>>(istream& is, json& j){
    char p = 0;
    is >> p;
    if(p=='['){
        // j.set_list();
        // j.push_front(move(get_lista(is)));
        j=get_lista(is);
    }else if(p=='{'){
        // j.set_dictionary();
        j=get_dictionary(is);
    }
    return is;
}

int main(){
    json j;
    cin>>j;
    cout<<"end of stream \n";
    return 0;
}