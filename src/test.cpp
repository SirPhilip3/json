
#include "json.hpp"

using namespace std;

struct json::impl{
    //? mi dice di che tipo è il json stra utile per is_...
    enum Tipo {Null, Bool, Number, String, List, Dictionary };
    Tipo tipo = Null;

    double number_val = 0;
	bool bool_val = false;
	std::string string_val = "";

    struct NodeL
    {
        json val;
        NodeL* next;
    };

    typedef NodeL* PNodeL;

    PNodeL headL;
    PNodeL tailL;

    struct NodeD
    {
        std::pair<std::string, json> val;
        NodeD* next;
    };

    typedef NodeD* PNodeD;

    PNodeD headD;
    PNodeD tailD;
};

json::json(){
    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Null;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;
}

template <typename T>
void destroyT(T& pc){

    while (pc!=nullptr)
    {
        T tmp = pc->next;
        delete pc;
        pc = tmp;
    }
    
}

//destructor
json::~json(){

    if(pimpl!=nullptr){

        if(is_list()){
            impl::PNodeL pc = pimpl->headL;
            destroyT(pc);
            pimpl->headL = nullptr;
            pimpl->tailL = nullptr;
        }else if(is_dictionary()){
            impl::PNodeD pc = pimpl->headD;
            destroyT(pc);
            pimpl->headD = nullptr;
            pimpl->tailD = nullptr;
        }

        delete pimpl;
        pimpl = nullptr;
    }

}

//copy constructor
json::json(json const& j){

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Null;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;

    if(j.is_list()){
        impl::PNodeL pc = j.pimpl->headL;
        while (pc!=nullptr){
            push_back(pc->val);
            pc=pc->next;
        }
    }else if(j.is_dictionary()) {
        impl::PNodeD pc = j.pimpl->headD;
        while (pc!=nullptr){ //! con dictionary all'iterno
            insert(pc->val);
            pc=pc->next;
        }
    }else{

        pimpl->tipo = j.pimpl->tipo;
        pimpl->number_val = j.pimpl->number_val;
        pimpl->bool_val = j.pimpl->bool_val;
        pimpl->string_val = j.pimpl->string_val;

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

        if(pimpl!=nullptr){
            //! duped
            if(is_list()){
                impl::PNodeL pc = pimpl->headL;
                destroyT(pc);
                pimpl->headL = nullptr;
                pimpl->tailL = nullptr;
            }else if(is_dictionary()){
                impl::PNodeD pc = pimpl->headD;
                destroyT(pc);
                pimpl->headD = nullptr;
                pimpl->tailD = nullptr;
            }
        }else pimpl = new impl;

        pimpl->tipo = j.pimpl->tipo;
        pimpl->number_val = j.pimpl->number_val;
        pimpl->bool_val = j.pimpl->bool_val;
        pimpl->string_val = j.pimpl->string_val;
        
        if(j.is_list()){
            impl::PNodeL pc = j.pimpl->headL;
            while (pc!=nullptr){
                push_back(pc->val); //! non posso usare altrimenti memory leaks
                pc=pc->next;
            }
        }else if(j.is_dictionary()) {
            impl::PNodeD pc = j.pimpl->headD;
            while (pc!=nullptr){ 
                insert(pc->val);
                pc=pc->next;
            }
        }
    }

    return *this;

}

// //? TIPO DI JSON

bool json::is_dictionary() const{
    return pimpl->tipo==impl::Tipo::Dictionary;
}

bool json::is_list() const{
    return pimpl->tipo==impl::Tipo::List;
}

bool json::is_string() const{
    return pimpl->tipo==impl::Tipo::String;
}

bool json::is_number() const{
    return pimpl->tipo==impl::Tipo::Number;
}

bool json::is_null() const{
    return pimpl->tipo==impl::Tipo::Null;
}

bool json::is_bool() const{
    return pimpl->tipo==impl::Tipo::Bool;
}

double& json::get_number(){
    if (is_number()){
        return pimpl->number_val;
    }else cout<<"no number found \n";
}

double const& json::get_number() const{
    if (is_number()){
        return pimpl->number_val;
    }else cout<<"no number found \n";
}

bool& json::get_bool(){
    if (is_bool()){//!
        return pimpl->bool_val;
    }else cout<<"no bool found \n";
}

bool const& json::get_bool() const{
    if (is_bool()){//!
        return pimpl->bool_val;
    }else cout<<"no bool found \n";
}

string& json::get_string(){
    if(is_string()){
        return pimpl->string_val;
    }else cout<<"no string found \n";
}

string const& json::get_string() const{
    if(is_string()){
        return pimpl->string_val;
    }else cout<<"no string found \n";
}

//! non posso modificare il json però posso comunque ritornare il valore corretto se trovato
json const& json::operator[](std::string const& s) const{
    if(is_dictionary()){
        impl::PNodeD tmp = pimpl->headD;
        bool found = false;
        while (tmp != nullptr&&!found)
        {
            if(tmp->val.first==s) {found=true; return tmp->val.second;}
            tmp=tmp->next;           
        }

    }else cout<<"not a dictionary \n";

}

//! se non trovo ele devo metterne uno vuoto nel diozionario
json& json::operator[](std::string const& s){

    if(is_dictionary()){

        impl::PNodeD tmp = pimpl->headD;
        bool found = false;

        while (tmp != nullptr&&!found)
        {
            if(tmp->val.first==s) found=true;
            else tmp = tmp->next;   
        }

        if(found) return tmp->val.second;

        pair<std::string, json> p;
        p.first=s; 
        p.second.set_null(); // devo creare un json come se fosse una lista
        insert(p);
        return pimpl->tailD->val.second;

    }else cout<<"not a dictionary \n";

}

//? SETTING

void json::set_list(){

    this->~json();

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::List;
    pimpl->headL = new impl::NodeL;
    pimpl->headL->next=nullptr;
    pimpl->headL->val.pimpl->tipo = impl::Tipo::Null;
    pimpl->tailL = pimpl->headL;

    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;

}

void json::set_dictionary(){

    this->~json();

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Dictionary;
    pimpl->headD = new impl::NodeD;
    pimpl->headD->next=nullptr;
    pimpl->headD->val.second.pimpl->tipo = impl::Tipo::Null;
    pimpl->tailD = pimpl->headD;

    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;

}

void json::set_string(string const& x){

    this->~json();

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::String;
    pimpl->string_val = x;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;

}

void json::set_bool(bool x){
    
    this->~json();

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Bool;
    pimpl->bool_val = x;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;
          
}

void json::set_number(double x){
    
    this->~json();

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Number;
    pimpl->number_val = x;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;

}

void json::set_null(){
 
    this->~json();

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Null;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;
    
}


// push_front 
// NOT USED FOR NOW
void json::push_front(json const& j){ // equivale a prepend
    
    pimpl->tipo = impl::Tipo::List;//! better placement

    impl::PNodeL newNode = new impl::NodeL;
    newNode->val = j;
    newNode->next = pimpl->headL;

    if (pimpl->headL == nullptr) {
        pimpl->tailL = newNode;
    }
    pimpl->headL = newNode;

}


//push_back
void json::push_back(json const& j){ // alla fine equivale a append

    pimpl->tipo = impl::Tipo::List;//! better placement

    impl::PNodeL tmp = new impl::NodeL;
    tmp->val = j;
    tmp->next = nullptr;

    if(pimpl->tailL!=nullptr){
        pimpl->tailL->next = tmp;
        pimpl->tailL = tmp;
    }else {
        pimpl->headL = tmp;
        pimpl->tailL = tmp;
    }

}


//inserisci un elemento in un dizionario
void json::insert(std::pair<std::string, json> const& d){

    pimpl->tipo= impl::Tipo::Dictionary;//! better placement

    impl::PNodeD tmp = new impl::NodeD;  
    tmp->val = d;
    tmp->next = nullptr;

    if(pimpl->tailD!=nullptr){
        pimpl->tailD->next = tmp;
        pimpl->tailD = tmp;
    }else {
        pimpl->headD = tmp;
        pimpl->tailD = tmp;
    }
}


// //? forward declaration
json E(std::istream& is);

json dell(istream& is);

json get_lista(std::istream& is){
    // legge una lista può leggere dei dizionari
    // finchè non si chiude la lista
    json j;
    // j.set_list();
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
            j.push_back(E(is));  
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

//!useless
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

//!useless
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
    // d.set_dictionary();
    std::pair<string , json> El_dict;
    is>>std::skipws;
    // is>>std::ws;

    char p=(is>>std::ws).peek();

    while(p!='}'){
        // cout<<"inside dic"<<endl;

        if(p!=','){
            // if(p=='{'||p=='[') is.putback(p);
            El_dict.first = dic_str(is);
            El_dict.second = move(E(is)); 

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


// list iterator
//? iteratorre cosa deve fare?
//! 
struct json::list_iterator{

    using iterator_category=std::forward_iterator_tag;//!  per bidirectiornal ho bisogno di prev nella lista
    using value_type = json;
    using pointer = json*; //prima json*
    using reference = json&; //prima json&

    list_iterator(impl::PNodeL ptr) : m_ptr(ptr){}

    reference operator*() const;  
    pointer operator->() const; 
    list_iterator operator+(int n);
    list_iterator& operator++(); // prefix ++
    list_iterator operator++(int /*dummy*/); // postfix ++
    bool operator==(list_iterator const& rhs) const;
    bool operator!=(list_iterator const& rhs) const;

    private :
        impl::PNodeL m_ptr; // valore di ritorno begin_list()

};

json::list_iterator json::begin_list(){
    return{pimpl->headL};
}

json::list_iterator json::end_list(){
    return{pimpl->tailL->next};
}

bool json::list_iterator::operator==(list_iterator const& rhs) const{
    return m_ptr == rhs.m_ptr;
}

bool json::list_iterator::operator!=(list_iterator const& rhs) const{
    return !(*this==rhs);
}

json::list_iterator json::list_iterator::operator+(int n){

    list_iterator temp = *this;
    for(int i = 0; i<n; ++i){
        ++temp;
    }
    return temp;

}

json::list_iterator& json::list_iterator::operator++(){
    m_ptr = m_ptr->next;
    return *this;
}

json::list_iterator json::list_iterator::operator++(int /*dummy*/){
    list_iterator tmp = *this;
    ++(*this);
    return tmp;
}

json::list_iterator::reference json::list_iterator::operator*() const {
    return m_ptr->val;    
}

json::list_iterator::pointer json::list_iterator::operator->() const{
    return &(m_ptr->val);
}


struct json::const_list_iterator{

    using iterator_category=std::forward_iterator_tag;//!  per bidirectiornal ho bisogno di prev nella lista
    using value_type = const json;
    using pointer = json const*; //prima json*
    using reference = json const& ; //prima json&

    const_list_iterator(impl::PNodeL ptr) : m_ptr(ptr){}

    reference operator*() const;  
    pointer operator->() const; 
    const_list_iterator operator+(int n);
    const_list_iterator& operator++(); // prefix ++
    const_list_iterator operator++(int /*dummy*/); // postfix ++
    bool operator==(const_list_iterator const& rhs) const;
    bool operator!=(const_list_iterator const& rhs) const;

    private :
        impl::PNodeL m_ptr; // valore di ritorno begin_list()

};

json::const_list_iterator json::begin_list() const{
    return{pimpl->headL};
}

json::const_list_iterator json::end_list() const{
    return{pimpl->tailL->next};
}

bool json::const_list_iterator::operator==(const_list_iterator const& rhs) const{
    return m_ptr == rhs.m_ptr;
}

bool json::const_list_iterator::operator!=(const_list_iterator const& rhs) const{
    return !(*this==rhs);
}

json::const_list_iterator json::const_list_iterator::operator+(int n){

    const_list_iterator temp = *this;
    for(int i = 0; i<n; ++i){
        ++temp;
    }
    return temp;

}

json::const_list_iterator& json::const_list_iterator::operator++(){
    m_ptr = m_ptr->next;
    return *this;
}

json::const_list_iterator json::const_list_iterator::operator++(int /*dummy*/){
    const_list_iterator tmp = *this;
    ++(*this);
    return tmp;
}

json::const_list_iterator::reference json::const_list_iterator::operator*() const {
    return m_ptr->val;    
}

json::const_list_iterator::pointer json::const_list_iterator::operator->() const{
    return &(m_ptr->val);
}


struct json::dictionary_iterator{

    using iterator_category=std::forward_iterator_tag;//!  per bidirectiornal ho bisogno di prev nella lista
    using value_type = pair<string, json>; //!!!!!!!!!! va bene per output però per lettura e modifica????????
    using pointer = pair<string, json>*; //prima json* //!!!!!!!!!!!!
    using reference = pair<string, json>& ; //prima json& //!!!!!!!!!!!!!!!

    dictionary_iterator(impl::PNodeD ptr) : m_ptr(ptr){}

    reference operator*() const; 
    pointer operator->() const; 
    dictionary_iterator operator+(int n);
    dictionary_iterator& operator++(); // prefix ++
    dictionary_iterator operator++(int /*dummy*/); // postfix ++
    bool operator==(dictionary_iterator const& rhs) const;
    bool operator!=(dictionary_iterator const& rhs) const;

    private :
        impl::PNodeD m_ptr; // valore di ritorno begin_list()

};

json::dictionary_iterator json::begin_dictionary(){
    return{pimpl->headD};
}

json::dictionary_iterator json::end_dictionary(){
    return{pimpl->tailD->next};
}

bool json::dictionary_iterator::operator==(dictionary_iterator const& rhs) const{
    return m_ptr == rhs.m_ptr;
}

bool json::dictionary_iterator::operator!=(dictionary_iterator const& rhs) const{
    return !(*this==rhs);
}

json::dictionary_iterator json::dictionary_iterator::operator+(int n){

    dictionary_iterator temp = *this;
    for(int i = 0; i<n; ++i){
        ++temp;
    }
    return temp;

}

json::dictionary_iterator& json::dictionary_iterator::operator++(){
    m_ptr = m_ptr->next;
    return *this;
}

json::dictionary_iterator json::dictionary_iterator::operator++(int /*dummy*/){
    dictionary_iterator tmp = *this;
    ++(*this);
    return tmp;
}

json::dictionary_iterator::reference json::dictionary_iterator::operator*() const {
    return m_ptr->val;    
}

json::dictionary_iterator::pointer json::dictionary_iterator::operator->() const{
    return &(m_ptr->val);
}


struct json::const_dictionary_iterator{

    using iterator_category=std::forward_iterator_tag;//!  per bidirectiornal ho bisogno di prev nella lista
    using value_type = const pair<string, json>; //!!!!!!!!!!!!
    using pointer = pair<string, json> const*; //prima json* //!!!!!!!!!!
    using reference = pair<string, json> const& ; //prima json& //!!!!!!!!!!!!

    const_dictionary_iterator(impl::PNodeD ptr) : m_ptr(ptr){}

    reference operator*() const;  
    pointer operator->() const; 
    const_dictionary_iterator operator+(int n);
    const_dictionary_iterator& operator++(); // prefix ++
    const_dictionary_iterator operator++(int /*dummy*/); // postfix ++
    bool operator==(const_dictionary_iterator const& rhs) const;
    bool operator!=(const_dictionary_iterator const& rhs) const;

    private :
        impl::PNodeD m_ptr; // valore di ritorno begin_list()

};

json::const_dictionary_iterator json::begin_dictionary() const{
    return{pimpl->headD};
}

json::const_dictionary_iterator json::end_dictionary() const{
    return{pimpl->tailD->next};
}

bool json::const_dictionary_iterator::operator==(const_dictionary_iterator const& rhs) const{
    return m_ptr == rhs.m_ptr;
}

bool json::const_dictionary_iterator::operator!=(const_dictionary_iterator const& rhs) const{
    return !(*this==rhs);
}

json::const_dictionary_iterator json::const_dictionary_iterator::operator+(int n){

    const_dictionary_iterator temp = *this;
    for(int i = 0; i<n; ++i){
        ++temp;
    }
    return temp;

}

json::const_dictionary_iterator& json::const_dictionary_iterator::operator++(){
    m_ptr = m_ptr->next;
    return *this;
}

json::const_dictionary_iterator json::const_dictionary_iterator::operator++(int /*dummy*/){
    const_dictionary_iterator tmp = *this;
    ++(*this);
    return tmp;
}

json::const_dictionary_iterator::reference json::const_dictionary_iterator::operator*() const {
    return m_ptr->val;    
}

json::const_dictionary_iterator::pointer json::const_dictionary_iterator::operator->() const{
    return &(m_ptr->val);
}

#include <fstream>

std::ostream& operator<<(std::ostream& lhs, json const& rhs){

    if(rhs.is_list()){
        lhs<<"[";

        for(auto it = rhs.begin_list(); it!=rhs.end_list(); ++it){
            if((*it).is_bool()){
                bool k = (*it).get_bool();
                string tmp = k ? "true" : "false";
                lhs << tmp;
            }else if((*it).is_number()){
                lhs<<(*it).get_number();
            }else if((*it).is_string()){
                lhs<<'"';
                lhs<<(*it).get_string();
                lhs<<'"';
            }else if((*it).is_null()){
                lhs<<"null";
            }else{
                lhs<<(*it);
            }
            auto tmp = it;
            if(++tmp!=rhs.end_list()) {
                lhs<<',';
            }
            
        }

        lhs<<"]";

    }else if(rhs.is_dictionary()) {
        lhs<<"{";

        for(auto it = rhs.begin_dictionary(); it!=rhs.end_dictionary(); ++it){

            lhs<<'"'<<(*it).first<<"\":";

            if((*it).second.is_bool()){
                bool k = (*it).second.get_bool();
                string tmp = k ? "true" : "false";
                lhs << tmp;
            }else if((*it).second.is_number()){
                lhs<<(*it).second.get_number();
            }else if((*it).second.is_string()){
                lhs<<'"';
                lhs<<(*it).second.get_string();
                lhs<<'"';
            }else if((*it).second.is_null()){
                lhs<<"null";
            }else{

                lhs<<(*it).second;

            }
            auto tmp = it;
            if(++tmp!=rhs.end_dictionary()) {
                lhs<<',';
            }
        }

        lhs<<"}";
    }

    return lhs;

}

int main(){
    json j;
    cin>>j;

    std::ofstream out("output.json");
    out<<j;

    // json& y = *(j.begin_list()+1);

    // json z;
    // // z.set_dictionary();
    // std::pair<std::string, json> l;
    // l.first = "a";
    // l.second.set_number(5);
    // z.insert(l);
    // l.first = "d";
    // l.second.set_number(6);
    // z.insert(l);
    // // cout<<"inserisci z \n";
    // // cin>>z;
    // // std::cout << y["quarta chiave"]["a"];

    // // y["prima chiave"] = z;
    // (*(++j.begin_list()))["prima chiave"] = z;
    
    cout<<"end of stream \n";
    return 0;
}