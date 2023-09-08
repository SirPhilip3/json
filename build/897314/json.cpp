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


json::json(json const& j){ //! può essere fatto con operator=

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


json::json(json&& j) : pimpl(j.pimpl) { 
    j.pimpl = nullptr;
}

json& json::operator=(json&& j){
    if(this!=&j){ 
        this->~json();
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

        pimpl->tipo = j.pimpl->tipo;//!!!!
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
        throw json_exception {"the json is not a number , invalid operation"};
    }
}

double const& json::get_number() const{
    if (is_number()){
        return pimpl->number_val;
    }else {
        throw json_exception {"the json is not a number , invalid operation"};
    }
}

bool& json::get_bool(){
    if (is_bool()){
        return pimpl->bool_val;
    }else {
        throw json_exception {"the json is not a boolean , invalid operation"};
    }
}

bool const& json::get_bool() const{
    if (is_bool()){
        return pimpl->bool_val;
    }else {
        throw json_exception {"the json is not a boolean , invalid operation"};
    }
}

string& json::get_string(){
    if(is_string()){
        return pimpl->string_val;
    }else {
        throw json_exception {"the json is not a string , invalid operation"};
    }
}

string const& json::get_string() const{
    if(is_string()){
        return pimpl->string_val;
    }else {
        throw json_exception {"the json is not a string , invalid operation"};
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

        throw json_exception {"The json with corresponding key : "+ s +" does not exist in the dictionary"};

    }else{
        throw json_exception {"Can't perform this action, the json object isn't a dictionary"};
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
        throw json_exception {"Can't perform this action, the json object isn't a dictionary"};
    }

}


void json::set_list(){

    this->~json();
    // delete this->pimpl;

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::List;
}

void json::set_dictionary(){

    this->~json();
    // delete this->pimpl;
    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Dictionary;
}

void json::set_string(string const& x){

    this->~json();
    // delete this->pimpl;

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::String;
    pimpl->string_val = x;
}

void json::set_bool(bool x){
    
    this->~json();
    // delete this->pimpl;

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Bool;
    pimpl->bool_val = x;
}

void json::set_number(double x){
    
    this->~json();
    // delete this->pimpl;

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Number;
    pimpl->number_val = x;
}

void json::set_null(){
 
    this->~json();
    // delete this->pimpl;

    pimpl = new impl;
    pimpl->tipo = impl::Tipo::Null;
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
        throw json_exception {"push_front() can only be called on a list"};
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
        throw json_exception {"push_back() can only be called on a list"};
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
        throw json_exception {"insert() can only be called on a dictionary"};
    }
}


//? forward declaration
json E(std::istream& is,short int from_dic_or_list); // true from dic false from list 

//! controlla la presenza corretta delle virgole 
bool check_commas(std::istream& is , short int from_dic_or_list, short int from_fun){

    // if(from_dic_or_list==3) return true;

    char n = (is>>std::ws).peek();

    if (n!=','&&n!=']'&&n!='}'&&n!=EOF){
        json_exception invalid_json;
        if(from_fun==0) invalid_json.msg = "Invalid number inserted";
        else if(from_fun==1) invalid_json.msg = "Invalid character after a string inserted";
        else if(from_fun==2) invalid_json.msg = "Invalid character after null";
        else if(from_fun==3) invalid_json.msg = "Invalid character after boolean"; 
        throw invalid_json;
    }

    if(from_dic_or_list==1){
        //from dic
        if(n=='}') return true;
        else if(n==']'){
            throw json_exception {"MissMatch closing dictionary with ]"};
        }else if(n==','){
            is>>n;
            n=(is>>std::ws).peek();
            if(n==']'||n=='}'||n==','){   
                throw json_exception {"Illegal comma before end of dictionary"};
            }
        }
    }else if(from_dic_or_list==0){
        //from list
        if(n==']') return true;
        else if(n=='}'){
            throw json_exception {"MissMatch closing list with }"};
        }else if(n==','){
            is>>n;
            n=(is>>std::ws).peek();
            if(n==']'||n=='}'||n==','){   
                throw json_exception {"Illegal comma before end of list"};
            }
        }
    }else if(from_dic_or_list==3){
        if(n!=EOF) throw json_exception {"extra data"};
    }

}

json get_lista(std::istream& is){
    json j;
    j.set_list();
    is>>std::skipws;

    // char p=(is>>std::ws).peek();
    char p;
    is>>p;

    if(p=='}'){
        throw json_exception {"MissMatched a '[' with a '}'"};
    }else if (p==','){
        throw json_exception {"missing value"};
    }else if (p!=']') is.putback(p);

    while(p!=']'){

        // if(p!=','){
            j.push_back(E(is,0));//! e ritorna el dopo virgola
        // }else is>>p;

        // p=(is>>std::ws).peek();

        /*
        [{}]
        is>>p ritorna } qua quindi faila perchè dovrebbe esserci una chiusa al massimo dopo
        
        
        */


        if(is){
            is>>p;
            // p=(is>>std::ws).peek();
            if(p!=','&&p!=']') throw json_exception {"invalid list"};
        }
        else {
            throw json_exception {"The list is incorrectly formatted, no ']' detected"};
        }
    }
    // p=(is>>std::ws).peek();
    return j;
}


string dic_str(istream& is){
    std::string s;
    char p=0;
    bool is_acceptable=false;
    is>>p;

    if(p=='"'){
        p=0;
    }else{
        throw json_exception {"There needs to be a key, missing '\"'"};
    }

    is>>std::noskipws;

    while(p!='"'||is_acceptable){
        if(is.peek()==EOF){
            throw json_exception {"no closing \" dic"};
        }
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

    if(is) p=(is>>std::ws).peek();
    else{
        throw json_exception {"The dictionary is incorrectly formatted, no value detected after \"" + s +'\"'};
    }
    if(p!=':') {
        throw json_exception {"There is no ':' after key value \"" + s +'\"'};
    }
    
    is.ignore(std::numeric_limits<std::streamsize>::max(), ':');
    return s;
}

json get_dictionary(istream& is){

    json d; 
    d.set_dictionary();
    std::pair<string , json> El_dict;
    is>>std::skipws;

    // char p=(is>>std::ws).peek();
    char p;
    is>>p;

    if(p==']'){
        throw json_exception {"MissMatched a '{' with a ']'"};
    }else if (p==','){
        throw json_exception {"missing value"};
    }else if (p!='}') is.putback(p);

    while(p!='}'){
        // if(p!=','){
            El_dict.first = dic_str(is);
            El_dict.second = move(E(is,1)); 

            d.insert(El_dict);

            El_dict.second.~json(); 

        // }else is>>p;

        if(is) {
            is>>p;
            // p=(is>>std::ws).peek();
            if(p!=','&&p!='}') throw json_exception {"invalid dictionary"};
        }else{
            throw json_exception {"The dictionary is incorrectly formatted, no '}' detected"};
        }
        // if(p=='}') return d;
    }
    // p=(is>>std::ws).peek();

    // p sempre }
    return d;
}


json number(istream& is, short int from_dic_or_list){

    json j;
    double num = 0;
    is>>std::skipws;

    is>>num;
    j.set_number(num);

    return j;
    // if(check_commas(is,from_dic_or_list,0)) return j;

}

json strings(istream& is,short int from_dic_or_list){
    std::string s;
    json j;
    char p=0;
    bool is_acceptable=false;

    is>>std::noskipws;

    while(p!='"'||is_acceptable){
        if(is.peek()==EOF){
            throw json_exception {"no closing \" "};
        }
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
    // p=is.peek();
    return j;
    // if(check_commas(is,from_dic_or_list,1)) return j;
}

// function to ceck if written correctly true, false or null 

bool ceck_bool_correctness(short int b_Type, istream& is, short int from_dic_or_list){

    short int letters;
    string lettersC;

    if(b_Type==0){ //for null
        letters = 3;
        lettersC = "ull";
    }else if(b_Type==1){ //for true
        letters  = 3;
        lettersC = "rue";
    }else if(b_Type==2){ //for false
        letters = 4;
        lettersC = "alse";
    }

    bool valid = true;
    char l;
    for(short int i = 0; i<letters&&valid; i++){
        is>>l;
        if(lettersC.empty()||l!=lettersC.at(0)) valid=false;
        else lettersC.erase(0,1);
    }
    
    l=(is>>std::ws).peek();
    if(from_dic_or_list!=3) if (l!=','&&l!=']'&&l!='}') valid=false;

    return valid;
}

json null(istream& is, short int from_dic_or_list){

    json j;
    is>>std::skipws;

    if(ceck_bool_correctness(0,is,from_dic_or_list)) j.set_null();
    else {
        throw json_exception {"The data is in an incorrect format, from null branch"};
    }

    return j;

    // if(check_commas(is,from_dic_or_list,2)) return j;

}

json boolean(istream& is, short int from_dic_or_list){
    char p=0;
    json j;
    is>>std::skipws;
    is>>p;

    if(p=='t'){
        if(ceck_bool_correctness(1,is,from_dic_or_list)) j.set_bool(true);
        else {
            throw json_exception {"The data is in an incorrect format, from true branch"};
        }
    }else if(p=='f'){
        if(ceck_bool_correctness(2,is,from_dic_or_list)) j.set_bool(false);
        else {
            throw json_exception {"The data is in an incorrect format, from false branch"};
        }
    }

    return j;

    // if(check_commas(is,from_dic_or_list,3)) return j;

}


json E(istream& is, short int from_dic_or_list){

    char p=0;
    json j;
    is>>std::skipws;

    is>>p;

    if((p>='0' && p<='9')||p=='-'||p=='.'){
        is.putback(p);
        j=move(number(is,from_dic_or_list));
    }else if(p=='['){
        j=move(get_lista(is));
        // is>>p;
    }else if(p=='{'){
        j=move(get_dictionary(is));
        // is>>p;
    }else if(p!='"'){
        if (p=='n'){
            j=move(null(is,from_dic_or_list));
        } 
        else if(p=='t'||p=='f'){
            is.putback(p);
            j=move(boolean(is,from_dic_or_list));
        }else {
            json_exception invalid_json;
            if(from_dic_or_list) invalid_json.msg = "unexpected character as value of a dictionary, the character was : \'"+ string(1,p)+ '\'';
            else invalid_json.msg = "The data in a list is in an incorrect format, the invalid character detected was : \'" + string(1,p)+ '\'';// chiamata quando mi aspetto qualcosaltro oltre es ["c",[33]
            throw invalid_json;
        }
    }else{
        j=move(strings(is,from_dic_or_list));
    }

    return j;
}


istream& operator>>(istream& is, json& j){
    char p = 0;
    is>>std::skipws;
    is >> p;

    j.set_null();
    is.putback(p);
    j=move(E(is,3));
    is>>p;
    if(is>>p) {
        j.set_null();
        throw json_exception {"extra data"};
    }

    // if(p=='['){
    //     j.set_list();
    //     j=move(get_lista(is));
    //     is>>p;
    //     if(is>>p) {
    //         j.set_null();
    //         throw json_exception {"extra data"};
    //     }
    // }else if(p=='{'){
    //     j.set_dictionary();
    //     j=move(get_dictionary(is));
    //     is>>p;
    //     if(is>>p) {
    //         j.set_null();
    //         throw json_exception {"extra data"};
    //     }
    // }else{
    //     j.set_null();
    //     is.putback(p);
    //     j=move(E(is,3));
    //     // throw json_exception {"The data is in an incorrect format"};
    // }

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
        throw json_exception {"begin_list() can not be called on a non-list json object"};
    }
}

json::list_iterator json::end_list(){
    if(is_list()){
        return{nullptr};
    }else{
        throw json_exception {"end_list() can not be called on a non-list json object"};
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
        throw json_exception {"begin_list() can not be called on a non-list json object"};
    }
}

json::const_list_iterator json::end_list() const{
    if(is_list()){
        return{nullptr};
    }else{
        throw json_exception {"end_list() can not be called on a non-list json object"};
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
        throw json_exception {"begin_dictionary() can not be called on a non-dictionary json object"};
    }
}

json::dictionary_iterator json::end_dictionary(){
    if(is_dictionary()){
        return{nullptr};
    }else {
        throw json_exception {"end_dictionary() can not be called on a non-dictionary json object"};
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
        throw json_exception {"begin_dictionary() can not be called on a non-dictionary json object"};
    }
}

json::const_dictionary_iterator json::end_dictionary() const{
    if(is_dictionary()){
        return{nullptr};
    }else {
        throw json_exception {"end_dictionary() can not be called on a non-dictionary json object"};
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
                // printf("%.*g", std::numeric_limits<double>::max_digits10, (*it).get_number());
                // lhs<<std::fixed;
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
                // printf("%.*g", std::numeric_limits<double>::max_digits10, (*it).second.get_number());
                // lhs<<std::fixed;
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
            // printf("%.*g", std::numeric_limits<double>::max_digits10, rhs.get_number());
            // lhs<<std::fixed;
            lhs<<rhs.get_number();
        }else if(rhs.is_string()){
            lhs<<'"';
            lhs<<rhs.get_string();
            lhs<<'"';
        }else if(rhs.is_null()){
            lhs<<"null";
        }else {
            throw json_exception {"The json is invalid for printing"};
        }
    }

    return lhs;
}