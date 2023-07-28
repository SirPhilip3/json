#include "json.hpp"

using namespace std;

//? STRUCT

struct El{
    bool i_num=false;
    double num;
	int boo=-1;
	std::string s;
    bool Null=true;
    std::pair<std::string, json> dic;

    json list; 
    json Dic;

    El* next = nullptr;
};

typedef El* pEl;

struct json::impl{
    pEl head;
    pEl tail;
};

//? CONSTRUCTOR / DESTRUCTOR

// default constructor
json::json(){
    pimpl = new impl;
    pimpl->head=nullptr;
    pimpl->tail=nullptr;
}

//destructor
json::~json(){

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
        pimpl = nullptr;
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
        if(j.is_list()){
            set_list(); //? potrebbe essere inutile
            push_back(pc->list); //! devo aggiungere un elemento se la lista è vuota
        }else{
            set_dictionary();
            pEl tmp = pc->Dic.pimpl->head;
            while (tmp!=nullptr)
            {
                insert(tmp->dic);
                tmp=tmp->next;
            }
        }
        pc=pc->next;
    }
    
}

//? MOVE SEMANTICS

// move constructor
json::json(json&& j){
    *this=move(j);
}

//move assignment operator
json& json::operator=(json&& j){
    if(this!=&j){
        delete pimpl;
        pimpl = j.pimpl;
        j.pimpl = nullptr;
    }
    return *this;
}

//? OPERATORS

//copy assignment operator
json& json::operator=(json const& j){

    if(this != &j){

        pEl tmp = pimpl->head;
        while (tmp!=nullptr)
        {
            pimpl->head = pimpl->head->next;
            delete tmp;
            tmp = pimpl->head;
        }
        
        pimpl->head=nullptr;
        pimpl->tail=nullptr;

        pEl current = j.pimpl->head;

        while (current != nullptr) {

            pEl newNode = new El;
            newNode->i_num = current->i_num;
            newNode->num = current->num;
            newNode->boo = current->boo;
            newNode->s = current->s;
            newNode->Null = current->Null;
            newNode->dic = current->dic;
            newNode->list = current->list;
            newNode->Dic = current->Dic;
            newNode->next = nullptr;

            if(pimpl->tail!=nullptr){
                pimpl->tail->next = newNode;
            }else{
                pimpl->head = newNode;
            }
            pimpl->tail = newNode;

            current = current->next;
        }
    }

    return *this;

}

//? TIPO DI JSON

bool json::is_dictionary() const{
    if(pimpl->head==nullptr) return true;
    return pimpl->head->Dic.pimpl!=nullptr&&pimpl->head->Dic.pimpl->head!=nullptr;
}

bool json::is_list() const{
    if(pimpl->head==nullptr) return true;
    return pimpl->head->list.pimpl!=nullptr&&pimpl->head->list.pimpl->head!=nullptr;
}

bool json::is_string() const{
    return !pimpl->head->s.empty();
}

bool json::is_number() const{
    if(this==nullptr) return false;
    return pimpl->head->i_num;
}

bool json::is_bool() const{
    return pimpl->head->boo!=-1;
}

bool json::is_null() const{
    return pimpl->head->Null;
}

double& json::get_number(){
    if (is_number()){
        return pimpl->head->num;
    }else cout<<"no number found \n";
}

double const& json::get_number() const{
    if (is_number()){
        return pimpl->head->num;
    }else cout<<"no number found \n";
}

bool& json::get_bool(){
    if (is_bool()){
        return (bool&)pimpl->head->boo;
    }else cout<<"no bool found \n";
}

bool const& json::get_bool() const{
    if (is_bool()){
        return (bool&)pimpl->head->boo;
    }else cout<<"no bool found \n";
}

string& json::get_string(){
    if(is_string()){
        return pimpl->head->s;
    }else cout<<"no string found \n";
}

string const& json::get_string() const{
    if(is_string()){
        return pimpl->head->s;
    }else cout<<"no string found \n";
}

//! forward declaration
json dell(istream& is);

//!
json const& json::operator[](std::string const& s) const{
    if(is_dictionary()){
        pEl tmp = pimpl->head->Dic.pimpl->head;
        bool found = false;
        while (tmp != nullptr&&!found)
        {
            if(tmp->dic.first==s) {found=true; return tmp->dic.second;}
            tmp=tmp->next;           
        }

    }else cout<<"not a dictionary \n";

}

//!
json& json::operator[](std::string const& s){

    if(is_dictionary()){

        pEl tmp = pimpl->head->Dic.pimpl->head;
        bool found = false;
        while (tmp != nullptr&&!found)
        {
            if(tmp->dic.first==s) {found=true; return tmp->dic.second;}
            tmp=tmp->next;           
        }

        pair<std::string, json> p;
        p.first=s; 
        p.second.set_list(); // devo creare un json cmoe se fosse una lista
        p.second.pimpl->head->list.set_null();
        insert(p);
        tmp=pimpl->head->Dic.pimpl->tail;
        return tmp->dic.second;

    }else cout<<"not a dictionary \n";

}

//? ITERATORS

// list iterator
struct json::list_iterator{

    using iterator_category=std::forward_iterator_tag;
    using value_type = json;
    using pointer = json*;
    using reference = json&;

    list_iterator(pEl ptr) : m_ptr(ptr){}

    reference operator*() const; // operator dereference 
    pointer operator->() const; // useful for structs
    // const because they do not modify the pointer
    list_iterator& operator++(); // prefix ++
    list_iterator operator++(int /*dummy*/); // postfix ++
    bool operator==(list_iterator const& rhs) const;
    bool operator!=(list_iterator const& rhs) const;

    private :
        pEl m_ptr;

};

json::list_iterator json::begin_list(){

    return{pimpl->head};

}

json::list_iterator json::end_list(){

    return{pimpl->tail->list.pimpl->tail->next};

}

bool json::list_iterator::operator==(list_iterator const& rhs) const{

    return m_ptr == rhs.m_ptr;

}

bool json::list_iterator::operator!=(list_iterator const& rhs) const{

    return !(*this==rhs);

}

json::list_iterator& json::list_iterator::operator++(){

    m_ptr = m_ptr->list.pimpl->head->next;
    return *this;

}

json::list_iterator json::list_iterator::operator++(int /*dummy*/){

    list_iterator tmp = *this;
    m_ptr = m_ptr->next;
    return tmp;

}

json::list_iterator::reference json::list_iterator::operator*() const{

    return m_ptr->list;    

}


json::list_iterator::pointer json::list_iterator::operator->() const{

    return &(m_ptr->list);

}

//const list iterator
struct json::const_list_iterator{
    
    using iterator_category=std::bidirectional_iterator_tag;
    using value_type = const json;
    using pointer = json const*;
    using reference = json const&;

    const_list_iterator(pEl ptr) : m_ptr(ptr){}

    private :
        El const* m_ptr;

};

json::const_list_iterator json::begin_list() const{

    return{pimpl->head->list.pimpl->head};

}

json::const_list_iterator json::end_list() const{

    return{pimpl->tail->list.pimpl->tail->next};

}

//? SETTING

void json::set_list(){

    if(pimpl!=nullptr){
        pEl pc = pimpl->head;
        
        while (pc!=nullptr)
        {
            pc=pc->next;
            delete pimpl->head;
            pimpl->head = pc;
        }

    }

    // pimpl->head = nullptr;
    // pimpl->tail = nullptr;

    pEl n = new El; 
    n->next=nullptr;
    // n->list.pimpl->head=nullptr;
    // n->list.pimpl->tail=nullptr;
    n->Null=false;

    n->list.pimpl->head=new El;
    n->list.pimpl->head->Null=true;
    n->list.pimpl->head->i_num=true;//! netrambi true significa che è un json.list vuoto
    n->list.pimpl->tail=n->list.pimpl->head;

    pimpl->head=n;
    pimpl->tail=n;

}

void json::set_dictionary(){

    if(pimpl!=nullptr){
        pEl pc = pimpl->head;
        
        while (pc!=nullptr)
        {
            pc=pc->next;
            delete pimpl->head;
            pimpl->head = pc;
        }

    }
    
    // pimpl->head = nullptr;
    // pimpl->tail = nullptr;

    pEl n = new El;
    n->next=nullptr;
    // n->Dic.pimpl->head=nullptr;
    // n->Dic.pimpl->tail=nullptr;
    n->Null=false;

    pimpl->head=n;
    pimpl->tail=n;

}

void json::set_string(string const& x){
    if(pimpl!=nullptr){
        pEl pc = pimpl->head;
        
        while (pc!=nullptr)
        {
            pc=pc->next;
            delete pimpl->head;
            pimpl->head = pc;
        }

    }

    // pimpl->head = nullptr;
    // pimpl->tail = nullptr;

    pEl n = new El;
    n->next=nullptr;
    n->s=x;
    n->Null=false;

    pimpl->head=n;
    pimpl->tail=n;

    
}

void json::set_bool(bool x){
    
    if(pimpl!=nullptr){
        pEl pc = pimpl->head;
        
        while (pc!=nullptr)
        {
            pc=pc->next;
            delete pimpl->head;
            pimpl->head = pc;
        }
    }

    // pimpl->head = nullptr;
    // pimpl->tail = nullptr;

    pEl n = new El;
    n->next=nullptr;
    n->boo=x;
    n->Null=false;

    pimpl->head=n;
    pimpl->tail=n;

          
}

void json::set_number(double x){
    
    if(pimpl!=nullptr){
        pEl pc = pimpl->head;
        
        while (pc!=nullptr)
        {
            pc=pc->next;
            delete pimpl->head;
            pimpl->head = pc;
        }
    }

    // pimpl->head = nullptr;
    // pimpl->tail = nullptr;

    pEl n = new El;
    n->next = nullptr;
    n->num = x;
    n->i_num = true;
    n->Null = false;

    pimpl->head = n;
    pimpl->tail = n;

}

void json::set_null(){
 
    if(pimpl!=nullptr){
        pEl pc = pimpl->head;
        
        while (pc!=nullptr)
        {
            pc=pc->next;
            delete pimpl->head;
            pimpl->head = pc;
        }

    }

    // pimpl->head = nullptr;
    // pimpl->tail = nullptr;

    pEl n = new El;
    n->next=nullptr;
    n->Null=true;

    pimpl->head=n;
    pimpl->tail=n;
    
}


//push_front 
// NOT USED FOR NOW
void json::push_front(json const& j){ // equivale a prepend
    
    pEl newNode = new El;
    newNode->list = j;//! shallow copy
    newNode->next = pimpl->head;

    if (pimpl->head == nullptr) {
        pimpl->tail = newNode;
    }
    pimpl->head = newNode;

}


//push_back
void json::push_back(json const& j){ // alla fine equivale a append

    pEl newNode = new El;
    newNode->i_num = j.pimpl->head->i_num; //!invalid read test15
    newNode->num = j.pimpl->head->num;
    newNode->boo = j.pimpl->head->boo;
    newNode->s = j.pimpl->head->s;
    newNode->Null = j.pimpl->head->Null;
    newNode->dic = j.pimpl->head->dic; 
    newNode->list = j.pimpl->head->list;
    newNode->Dic = j.pimpl->head->Dic;
    newNode->next = nullptr;

    // set_list(); SE FACCIO COSì TUTTE LE LISTE CON PIù DI 2 ELEMENTI SARà SEMPRE ELIMINATO A 1 + NON RISOVE LEAKS

    if (pimpl->head->list.pimpl->head->Null && pimpl->head->list.pimpl->head->i_num  ) {

        delete pimpl->head->list.pimpl->head;
        pimpl->head->list.pimpl->head=nullptr;
        pimpl->head->list.pimpl->tail=nullptr;
        pimpl->head->list.pimpl->head = newNode;
        pimpl->tail->list.pimpl->tail = newNode;
    } else {
        pimpl->tail->list.pimpl->tail->next = newNode;
        pimpl->tail->list.pimpl->tail = newNode;
    }

}


//inserisci un elemento in un dizionario
void json::insert(std::pair<std::string, json> const& d){

    pEl n = new El;
    n->next=nullptr;
    n->Null= false;
    n->dic.first = d.first;
    n->dic.second = json(d.second); //! copy constructor

    if(pimpl->tail->Dic.pimpl->tail!=nullptr){
        pimpl->tail->Dic.pimpl->tail->next=n;
        }
    else
        pimpl->head->Dic.pimpl->head=n;
    pimpl->tail->Dic.pimpl->tail=n;
}


//? forward declaration
json E(std::istream& is);

json get_lista(std::istream& is){
    // legge una lista può leggere dei dizionari
    // finchè non si chiude la lista
    json j;
    j.set_list();
    is>>std::skipws;
    // is>>std::ws;

    char p=(is>>std::ws).peek();
    while(p!=']'){

        if(p=='\n') {
            is.ignore(1,'\n');
            if(is) p=(is>>std::ws).peek(); //!controllo BHO
        }
        if(p==']'||p=='}') return j;

        if(p!=','){
            // if(p=='{'||p=='[') is.putback(p); //? fixes loop with [[..]]
            j.push_back(E(is));  //! crea prob test15
        }else is>>p;

        if(is) p=(is>>std::ws).peek(); // per peek deve essere valido
        else return j;
    }

    return j;
}

string dic_str(istream& is){
    std::string s;
    char p=0;
    bool is_acceptable=false;
    is>>p;
    if(p=='{') {
        is>>p;}
    if(p=='"') {
        p=0;}

    is>>std::noskipws;

    while(p!='"'||is_acceptable){

        if(p=='\n') {is.ignore(1,'\n'); p=(is>>std::ws).peek();}
        if(p=='}') return s;

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
    //?
    is>>std::skipws;
    std::cout<<"--"<<s<<'\n';
    is.ignore(std::numeric_limits<std::streamsize>::max(), ':');// ignora il rest0 , spazi vuoti etc fino a ,
    return s;
}


json get_lista_single(std::istream& is){

    json j;
    j.set_list();
    // is>>std::skipws;
    // is>>std::ws;

    char p=(is>>std::ws).peek();

    if(p=='\n') {
        is.ignore(1,'\n'); 
        if(is) p=(is>>std::ws).peek();
    } //! p is never read

    j.push_back(E(is));

    return j;

}


json dell(istream& is){

    char p=0;
    json j;
    // is>>std::skipws;
    // is>>std::ws;//! skips ws for peek

    p=(is>>std::ws).peek();

    if(p=='['||p=='{'){
        // is.putback(p);
        j = move(E(is));    
    }else{
        // is.putback(p);
        j = move(get_lista_single(is));
    }   

    return j;
}


json get_dictionary(istream& is){

    json d; 
    d.set_dictionary();
    std::pair<string , json> El_dict;
    is>>std::skipws;
    // is>>std::ws;

    char p=(is>>std::ws).peek();

    while(p!='}'){
        // cout<<"inside dic"<<endl;

        if(p!=','){
            // if(p=='{'||p=='[') is.putback(p);
            El_dict.first = dic_str(is);
            El_dict.second = move(dell(is)); 

            d.insert(El_dict);

            El_dict.second.~json(); //? elimino ad ogni iterazione la memoria allocata con la new per un json prima non la eliminavo la sovrascrivevo

        }else is>>p;// devo consumare ,

        if(is) {
            p=(is>>std::ws).peek();// si ferma a , bho
            if(p=='\n') {is.ignore(1,'\n'); p=(is>>std::ws).peek();}
        }
        else return d;
    
        if(p=='}') return d;
    }

    return d;
}


json number(istream& is){

    json j;
    double num = 0;
    is>>std::skipws;
    // is>>std::ws;

    is>>num;
    //?
    std::cout<<num<<" --num \n";
    j.set_number(num);

    char n = (is>>std::ws).peek();
    // char n = is.peek();
    if(n=='\n') {is.ignore(1, '\n'); n=(is>>std::ws).peek();}
    if(n=='}'||n==']') return j;
    is.ignore(std::numeric_limits<std::streamsize>::max(), ',');

    return j;
}

json strings(istream& is){
    std::string s;
    json j;
    char p=0;
    bool is_acceptable=false;

    is>>std::noskipws;

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

    s.erase(s.length()-1);
    
    is>>std::skipws;
    // is>>std::ws;
    std::cout<<"--"<<s<<'\n';

    j.set_string(s); // potrei chiamarlo
    char n = (is>>std::ws).peek();
    // char n = is.peek();
    if(n=='\n') {is.ignore(1, '\n'); n=(is>>std::ws).peek();}
    if(n=='}'||n==']') return j;
    is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
    return j;
}

json null(istream& is){
    //?
    std::cout<<"--null \n"; 
    json j;
    is>>std::skipws;
    // is>>std::ws;
    j.set_null();

    is.ignore(3);
    char n = (is>>std::ws).peek();
    // char n = is.peek();
    if(n=='\n') {is.ignore(1, '\n'); n=(is>>std::ws).peek();}
    if(n=='}'||n==']') return j;
    is.ignore(std::numeric_limits<std::streamsize>::max(),','); 
    return j;
}

json boolean(istream& is){
    char p=0;
    json j;
    is>>std::skipws;
    // is>>std::ws;
    is>>p;

    if(p=='t'){
        //?
        std::cout<<"--true \n";
        j.set_bool(true);
        is.ignore(3);
    } 
    else if(p=='f'){
        //?
        std::cout<<"--false \n";
        j.set_bool(false);
        is.ignore(4);
    }

    char n = (is>>std::ws).peek();
    // char n = is.peek();
    if(n=='\n') {is.ignore(1, '\n'); n=(is>>std::ws).peek();}
    if(n=='}'||n==']') return j;
    is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
    return j;
}


json E(istream& is){

    char p=0;
    json j;
    is>>std::skipws;
    // is>>std::ws;

    is>>p;
    if(p=='\n') {is>>p;}
    if((p>='0' && p<='9')||p=='-'){
        is.putback(p);

        j=move(number(is));

    }else if(p=='['){
        //?
        std::cout<<"--[ \n";
        j=move(get_lista(is));
        is>>p;
        //?
        std::cout<<"--] \n";
    }else if(p=='{'){
        //?
        std::cout<<"--{ \n";
        j=move(get_dictionary(is));
        is>>p;
        //?
        std::cout<<"--} \n";
    }else if(p!='"'){
        // o è null o è un buleano
        if (p=='n'){
            j=move(null(is));
        } 
        else {
            is.putback(p);
            j=move(boolean(is));
        }
    }else{
        j=move(strings(is));

    }

    return j;
}

// cin
istream& operator>>(istream& is, json& j){
    char p = 0;
    is>>std::skipws;
    // is>>std::ws;

    is >> p;
    if(p=='['){
        j=move(get_lista(is));
    }else if(p=='{'){
        j=move(get_dictionary(is));
    }
    
    //?
    cout<<"end of stream \n";
    return is;
}

std::ostream& operator<<(std::ostream& lhs, json const& rhs){

    // devo usare gli iterator

}

int main(){
    json j;
    cin>>j;
    //?
    cout<<"end of stream \n";

    // json& k = *(++j.begin_list());
    // cout<<k.get_number();
    // json k=j["s"];
    // k=j["cio"];

    // double a = k.get_number();
    // cout<<"a"<<a;
    return 0;
}