#include "json.hpp"

using namespace std;

struct json::impl{
    enum Tipo {Null, Bool, Number, String, List, Dictionary };
    Tipo tipo = Null;

    double number_val = 0;
	bool bool_val = false;
	std::string string_val = "";

    struct NodeL
    {
        json val;
        NodeL* next=nullptr;
    };

    typedef NodeL* PNodeL;

    PNodeL headL=nullptr;
    PNodeL tailL=nullptr;

    struct NodeD
    {
        std::pair<std::string, json> val;
        NodeD* next=nullptr;
    };

    typedef NodeD* PNodeD;

    PNodeD headD=nullptr;
    PNodeD tailD=nullptr;
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
        std::cout<<"";//!!!!
    }

}


json::json(json const& j){

    pimpl = new impl;//!
    pimpl->tipo = impl::Tipo::Null;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;

    if(j.is_list()){
        impl::PNodeL pc = j.pimpl->headL;
        set_list();
        while (pc!=nullptr){
            push_back(pc->val);
            pc=pc->next;
        }
    }else if(j.is_dictionary()) {
        impl::PNodeD pc = j.pimpl->headD;
        set_dictionary();
        while (pc!=nullptr){ 
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


json::json(json&& j) : pimpl(j.pimpl) { //! fixes uninitalized values problems
    j.pimpl = nullptr;
}

json& json::operator=(json&& j){
    if(this!=&j){ 
        delete pimpl;
        pimpl = j.pimpl;
        j.pimpl = nullptr;
    }
    return *this;
}


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
            set_list();
            while (pc!=nullptr){
                push_back(pc->val); 
                pc=pc->next;
            }
        }else if(j.is_dictionary()) {
            impl::PNodeD pc = j.pimpl->headD;
            set_dictionary();
            while (pc!=nullptr){ 
                insert(pc->val);
                pc=pc->next;
            }
        }
    }

    return *this;

}


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
    }else {
        json_exception not_a_number;
        not_a_number.msg = "the json is not a number , invalid operation";
        throw not_a_number;
    }
}

double const& json::get_number() const{
    if (is_number()){
        return pimpl->number_val;
    }else {
        json_exception not_a_number;
        not_a_number.msg = "the json is not a number , invalid operation";
        throw not_a_number;
    }
}

bool& json::get_bool(){
    if (is_bool()){
        return pimpl->bool_val;
    }else {
        json_exception not_a_bool;
        not_a_bool.msg = "the json is not a boolean , invalid operation";
        throw not_a_bool;
    }
}

bool const& json::get_bool() const{
    if (is_bool()){
        return pimpl->bool_val;
    }else {
        json_exception not_a_bool;
        not_a_bool.msg = "the json is not a boolean , invalid operation";
        throw not_a_bool;
    }
}

string& json::get_string(){
    if(is_string()){
        return pimpl->string_val;
    }else {
        json_exception not_a_string;
        not_a_string.msg = "the json is not a string , invalid operation";
        throw not_a_string;
    }
}

string const& json::get_string() const{
    if(is_string()){
        return pimpl->string_val;
    }else {
        json_exception not_a_string;
        not_a_string.msg = "the json is not a string , invalid operation";
        throw not_a_string;
    }
}


json const& json::operator[](std::string const& s) const{
    if(is_dictionary()){
        impl::PNodeD tmp = pimpl->headD;
        bool found = false;

        while (tmp != nullptr&&!found)
        {
            if(tmp->val.first==s) found=true;
            else tmp = tmp->next;   
        }

        if(found) return tmp->val.second;

        json_exception not_found;
        not_found.msg = "The json with corresponding key : "+ s +" does not exist in the dictionary";
        throw not_found;

    }else{
        json_exception not_a_dictionary;
        not_a_dictionary.msg = "Can't perform this action, the json object isn't a dictionary";
        throw not_a_dictionary;
    } 

}

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
        p.second.set_null(); 
        insert(p);
        return pimpl->tailD->val.second;

    }else{
        json_exception not_a_dictionary;
        not_a_dictionary.msg = "Can't perform this action, the json object isn't a dictionary";
        throw not_a_dictionary;
    }

}


void json::set_list(){

    this->~json();

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::List;
    pimpl->headL = nullptr;
    pimpl->tailL = nullptr;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;

}

void json::set_dictionary(){

    this->~json();

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Dictionary;
    pimpl->headD = nullptr;
    pimpl->tailD = nullptr;
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


void json::push_front(json const& j){ 
    if(is_list()){

        impl::PNodeL newNode = new impl::NodeL;
        newNode->val = j;
        newNode->next = pimpl->headL;

        if (pimpl->headL == nullptr) {
            pimpl->tailL = newNode;
        }
        pimpl->headL = newNode;
    }else{
        json_exception invalid_push_front;
        invalid_push_front.msg = "push_front() can only be called on a list";
        throw invalid_push_front;
    }


}


void json::push_back(json const& j){ 

    if(is_list()){
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
    }else{
        json_exception invalid_push_front;
        invalid_push_front.msg = "push_back() can only be called on a list";
        throw invalid_push_front;
    }

}


void json::insert(std::pair<std::string, json> const& d){

    if(is_dictionary()){
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
    }else{
        json_exception invalid_push_front;
        invalid_push_front.msg = "insert() can only be called on a dictionary";
        throw invalid_push_front;
    }
}


// //? forward declaration
json E(std::istream& is);

json get_lista(std::istream& is){
    json j;
    j.set_list();
    is>>std::skipws;

    char p=(is>>std::ws).peek();
    while(p!=']'){

        if(p=='\n') {
            is.ignore(1,'\n');
            if(is) p=(is>>std::ws).peek(); 
        }
        if(p==']'||p=='}') return j;

        if(p!=','){
            j.push_back(E(is));  
        }else is>>p;

        if(is) p=(is>>std::ws).peek(); 
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
    
    s.erase(s.length()-1);
    is>>std::skipws;
    is.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    return s;
}

json get_dictionary(istream& is){

    json d; 
    d.set_dictionary();
    std::pair<string , json> El_dict;
    is>>std::skipws;

    char p=(is>>std::ws).peek();

    while(p!='}'){

        if(p!=','){
            El_dict.first = dic_str(is);
            El_dict.second = move(E(is)); 

            d.insert(El_dict);

            El_dict.second.~json(); 

        }else is>>p;

        if(is) {
            p=(is>>std::ws).peek();
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

    is>>num;
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

    j.set_string(s); 
    char n = (is>>std::ws).peek();
    if(n=='\n') {is.ignore(1, '\n'); n=(is>>std::ws).peek();}
    if(n=='}'||n==']') return j;
    is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
    return j;
}

json null(istream& is){

    json j;
    is>>std::skipws;
    j.set_null();

    is.ignore(3);
    char n = (is>>std::ws).peek();
    if(n=='\n') {is.ignore(1, '\n'); n=(is>>std::ws).peek();}
    if(n=='}'||n==']') return j;
    is.ignore(std::numeric_limits<std::streamsize>::max(),','); 
    return j;
}

json boolean(istream& is){
    char p=0;
    json j;
    is>>std::skipws;
    is>>p;

    if(p=='t'){
        j.set_bool(true);
        is.ignore(3);
    } 
    else if(p=='f'){
        j.set_bool(false);
        is.ignore(4);
    }

    char n = (is>>std::ws).peek();
    if(n=='\n') {is.ignore(1, '\n'); n=(is>>std::ws).peek();}
    if(n=='}'||n==']') return j;
    is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
    return j;
}


json E(istream& is){

    char p=0;
    json j;
    is>>std::skipws;

    is>>p;
    if(p=='\n') {is>>p;}
    if((p>='0' && p<='9')||p=='-'){
        is.putback(p);
        j=move(number(is));
    }else if(p=='['){
        j=move(get_lista(is));
        is>>p;
    }else if(p=='{'){
        j=move(get_dictionary(is));
        is>>p;
    }else if(p!='"'){
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


istream& operator>>(istream& is, json& j){
    char p = 0;
    is>>std::skipws;

    is >> p;
    if(p=='['){
        j=move(get_lista(is));
    }else if(p=='{'){
        j=move(get_dictionary(is));
    }else {
        is.putback(p);
        j=move(E(is));
    }

    return is;
}

struct json::list_iterator{

    using iterator_category=std::forward_iterator_tag;
    using value_type = json;
    using pointer = json*; 
    using reference = json&; 

    list_iterator(impl::PNodeL ptr) : m_ptr(ptr){}

    reference operator*() const;  
    pointer operator->() const; 
    list_iterator operator+(int n);
    list_iterator& operator++(); 
    list_iterator operator++(int /*dummy*/); 
    bool operator==(list_iterator const& rhs) const;
    bool operator!=(list_iterator const& rhs) const;

    private :
        impl::PNodeL m_ptr; 

};

json::list_iterator json::begin_list(){
    if(is_list()){
        return{pimpl->headL};
    }else{
        json_exception invalid_list_iterator_usage;
        invalid_list_iterator_usage.msg = "begin_list() can not be called on a non-list json object";
        throw invalid_list_iterator_usage;
    }
}

json::list_iterator json::end_list(){
    if(is_list()){
        return{nullptr};
    }else{
        json_exception invalid_list_iterator_usage;
        invalid_list_iterator_usage.msg = "end_list() can not be called on a non-list json object";
        throw invalid_list_iterator_usage;
    }
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

    using iterator_category=std::forward_iterator_tag;  
    using value_type = const json;
    using pointer = json const*; 
    using reference = json const& ; 

    const_list_iterator(impl::PNodeL ptr) : m_ptr(ptr){}

    reference operator*() const;  
    pointer operator->() const; 
    const_list_iterator operator+(int n);
    const_list_iterator& operator++(); 
    const_list_iterator operator++(int /*dummy*/); 
    bool operator==(const_list_iterator const& rhs) const;
    bool operator!=(const_list_iterator const& rhs) const;

    private :
        impl::PNodeL m_ptr; 

};

json::const_list_iterator json::begin_list() const{
    if(is_list()){
        return{pimpl->headL};
    }else{
        json_exception invalid_list_iterator_usage;
        invalid_list_iterator_usage.msg = "begin_list() can not be called on a non-list json object";
        throw invalid_list_iterator_usage;
    }
}

json::const_list_iterator json::end_list() const{
    if(is_list()){
        return{nullptr};
    }else{
        json_exception invalid_list_iterator_usage;
        invalid_list_iterator_usage.msg = "end_list() can not be called on a non-list json object";
        throw invalid_list_iterator_usage;
    }
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

    using iterator_category=std::forward_iterator_tag;
    using value_type = pair<string, json>; 
    using pointer = pair<string, json>*; 
    using reference = pair<string, json>& ; 

    dictionary_iterator(impl::PNodeD ptr) : m_ptr(ptr){}

    reference operator*() const; 
    pointer operator->() const; 
    dictionary_iterator operator+(int n);
    dictionary_iterator& operator++(); 
    dictionary_iterator operator++(int /*dummy*/); 
    bool operator==(dictionary_iterator const& rhs) const;
    bool operator!=(dictionary_iterator const& rhs) const;

    private :
        impl::PNodeD m_ptr; 

};

json::dictionary_iterator json::begin_dictionary(){
    if(is_dictionary()){
        return{pimpl->headD};
    }else{
        json_exception invalid_dictionary_iterator_usage;
        invalid_dictionary_iterator_usage.msg = "begin_dictionary() can not be called on a non-dictionary json object";
        throw invalid_dictionary_iterator_usage;
    }
}

json::dictionary_iterator json::end_dictionary(){
    if(is_dictionary()){
        return{nullptr};
    }else {
        json_exception invalid_dictionary_iterator_usage;
        invalid_dictionary_iterator_usage.msg = "end_dictionary() can not be called on a non-dictionary json object";
        throw invalid_dictionary_iterator_usage;
    }   
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

    using iterator_category=std::forward_iterator_tag;
    using value_type = const pair<string, json>;
    using pointer = pair<string, json> const*; 
    using reference = pair<string, json> const& ; 

    const_dictionary_iterator(impl::PNodeD ptr) : m_ptr(ptr){}

    reference operator*() const;  
    pointer operator->() const; 
    const_dictionary_iterator operator+(int n);
    const_dictionary_iterator& operator++(); 
    const_dictionary_iterator operator++(int /*dummy*/); 
    bool operator==(const_dictionary_iterator const& rhs) const;
    bool operator!=(const_dictionary_iterator const& rhs) const;

    private :
        impl::PNodeD m_ptr; 

};

json::const_dictionary_iterator json::begin_dictionary() const{
    if(is_dictionary()){
        return{pimpl->headD};
    }else{
        json_exception invalid_dictionary_iterator_usage;
        invalid_dictionary_iterator_usage.msg = "begin_dictionary() can not be called on a non-dictionary json object";
        throw invalid_dictionary_iterator_usage;
    }
}

json::const_dictionary_iterator json::end_dictionary() const{
    if(is_dictionary()){
        return{nullptr};
    }else {
        json_exception invalid_dictionary_iterator_usage;
        invalid_dictionary_iterator_usage.msg = "end_dictionary() can not be called on a non-dictionary json object";
        throw invalid_dictionary_iterator_usage;
    }
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

std::ostream& operator<<(std::ostream& lhs, json const& rhs){

    if(rhs.is_list()){
        lhs<<"[";

        for(auto it = rhs.begin_list(); it!=rhs.end_list(); ++it){
            if((*it).is_bool()){
                string tmp = (*it).get_bool() ? "true" : "false";
                lhs << tmp;
            }else if((*it).is_number()){
                lhs<<std::fixed;
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
                string tmp = (*it).second.get_bool() ? "true" : "false";
                lhs << tmp;
            }else if((*it).second.is_number()){
                lhs<<std::fixed;
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
    }else{ 
        if(rhs.is_bool()){
            string tmp = rhs.get_bool() ? "true" : "false";
            lhs << tmp;
        }else if(rhs.is_number()){
            lhs<<rhs.get_number();
        }else if(rhs.is_string()){
            lhs<<rhs.get_string();
        }else if(rhs.is_null()){
            lhs<<"null";
        }
    }

    return lhs;
}