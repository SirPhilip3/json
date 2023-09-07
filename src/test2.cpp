#include "json.hpp"

enum Type {type_list, type_dictionary, type_string, type_number, type_boolean, type_null};

struct list_node
{
    json info;
    list_node* next;
};

struct dict_node
{
    std::pair<std::string, json> info;
    dict_node* next;
};

//$------------------------------------------- IMPLEMENTATION

struct json::impl
{
    impl();

    Type type;

    list_node* list_head;
    list_node* list_tail;

    dict_node* dict_head;
    dict_node* dict_tail;

    std::string str;
    double num;
    bool boolean;

    void reset();
    void destroy();
};

// COSTRUTTORE DI DEFAULT DI IMPL - SETTA JSON A NULL
json::impl::impl() : type(type_null), list_head(nullptr), list_tail(nullptr),
                     dict_head(nullptr), dict_tail(nullptr), str(""), num(0),
                     boolean(false) {}



//$------------------------------------------- ITERATORS DECLARATION

struct json::list_iterator
{
public:
    list_iterator(list_node*);

    json& operator*() const;
    json* operator->() const;
    list_iterator& operator++();
    list_iterator operator++(int);
    bool operator==(list_iterator const&) const;
    bool operator!=(list_iterator const&) const;

private:
    list_node* index;
};

struct json::const_list_iterator
{
public:
    const_list_iterator(list_node*);

    json& operator*() const;
    json* operator->() const;
    const_list_iterator& operator++();
    const_list_iterator operator++(int);
    bool operator==(const_list_iterator const&) const;
    bool operator!=(const_list_iterator const&) const;

private:
    list_node* index;
};

struct json::dictionary_iterator
{
public:
    dictionary_iterator(dict_node*);

    std::pair<std::string,json>& operator*() const;
    std::pair<std::string,json>* operator->() const;
    dictionary_iterator& operator++();
    dictionary_iterator operator++(int);
    bool operator==(dictionary_iterator const&) const;
    bool operator!=(dictionary_iterator const&) const;

private:
    dict_node* index;
};

struct json::const_dictionary_iterator
{
    public:
    const_dictionary_iterator(dict_node*);

    std::pair<std::string,json>& operator*() const;
    std::pair<std::string,json>* operator->() const;
    const_dictionary_iterator& operator++();
    const_dictionary_iterator operator++(int);
    bool operator==(const_dictionary_iterator const&) const;
    bool operator!=(const_dictionary_iterator const&) const;

private:
    dict_node* index;
};



//$------------------------------------------- ITERATORS FUNCTIONS

// COSTRUTTORE LIST ITERATOR
json::list_iterator::list_iterator(list_node* ptr) : index{ptr} {};

// COSTRUTTORE CONST LIST ITERATOR
json::const_list_iterator::const_list_iterator(list_node* ptr) : index{ptr} {};

// COSTRUTTORE DICTIONARY ITERATOR
json::dictionary_iterator::dictionary_iterator(dict_node* ptr) : index{ptr} {};

// COSTRUTTORE CONST DICTIONARY ITERATOR
json::const_dictionary_iterator::const_dictionary_iterator(dict_node* ptr) : index{ptr} {};

//*------------------------------------------- LIST ITERATOR

json& json::list_iterator::operator*() const { return index->info; }

json* json::list_iterator::operator->() const { return &(index->info); }

json::list_iterator& json::list_iterator::operator++() {
    index = index->next;
    return *this;
}

json::list_iterator json::list_iterator::operator++(int) {
    list_iterator prev_index {index};
    index = index->next;
    return prev_index;
}

bool json::list_iterator::operator==(list_iterator const& it) const { return index == it.index; }

bool json::list_iterator::operator!=(list_iterator const& it) const { return index != it.index; }

//*------------------------------------------- CONST LIST ITERATOR

json& json::const_list_iterator::operator*() const { return index->info; }

json* json::const_list_iterator::operator->() const { return &(index->info); }

json::const_list_iterator& json::const_list_iterator::operator++() {
    index = index->next;
    return *this;
}

json::const_list_iterator json::const_list_iterator::operator++(int) {
    const_list_iterator prev_index {index};
    index = index->next;
    return prev_index;
}

bool json::const_list_iterator::operator==(const_list_iterator const& it) const {
    return index == it.index;
}

bool json::const_list_iterator::operator!=(const_list_iterator const& it) const {
    return index != it.index;
}

//*------------------------------------------- DICTIONARY ITERATOR

std::pair<std::string,json>& json::dictionary_iterator::operator*() const { return index->info; }

std::pair<std::string,json>* json::dictionary_iterator::operator->() const { return &(index->info); }

json::dictionary_iterator& json::dictionary_iterator::operator++() {
    index = index->next;
    return *this;
}

json::dictionary_iterator json::dictionary_iterator::operator++(int) {
    dictionary_iterator prev_index {index};
    index = index->next;
    return prev_index;
}

bool json::dictionary_iterator::operator==(dictionary_iterator const& it) const {
    return index == it.index;
}

bool json::dictionary_iterator::operator!=(dictionary_iterator const& it) const {
    return index != it.index;
}

//*------------------------------------------- CONST DICTIONARY ITERATOR

std::pair<std::string,json>& json::const_dictionary_iterator::operator*() const {
    return index->info;
}

std::pair<std::string,json>* json::const_dictionary_iterator::operator->() const {
    return &(index->info);
}

json::const_dictionary_iterator& json::const_dictionary_iterator::operator++() {
    index = index->next;
    return *this;
}

json::const_dictionary_iterator json::const_dictionary_iterator::operator++(int) {
    const_dictionary_iterator prev_index {index};
    index = index->next;
    return prev_index;
}

bool json::const_dictionary_iterator::operator==(const_dictionary_iterator const& it) const {
    return index == it.index;
}

bool json::const_dictionary_iterator::operator!=(const_dictionary_iterator const& it) const {
    return index != it.index;
}

//$------------------------------------------- ITERATORS BEGIN & END METHODS

json::list_iterator json::begin_list()
{
    if (is_list())
        return {pimpl->list_head};
    else
        throw json_exception {"error calling begin_list(): element is not a list"};
}

json::list_iterator json::end_list()
{
    if (is_list())
        return {nullptr};
    else
        throw json_exception {"error calling end_list(): element is not a list"};
}

json::dictionary_iterator json::begin_dictionary()
{
    if (is_dictionary())
        return {pimpl->dict_head};
    else
        throw json_exception {"error calling begin_dictionary(): element is not a dictionary"};
}

json::dictionary_iterator json::end_dictionary()
{
    if (is_dictionary())
        return {nullptr};
    else
        throw json_exception {"error calling end_dictionary(): element is not a dictionary"};
}

//*------------------------------------------------------

json::const_list_iterator json::begin_list() const
{
    if (is_list())
        return {pimpl->list_head};
    else
        throw json_exception {"error calling begin_list(): element is not a list"};
}

json::const_list_iterator json::end_list() const
{
    if (is_list())
        return {nullptr};
    else
        throw json_exception {"error calling end_list(): element is not a list"};
}

json::const_dictionary_iterator json::begin_dictionary() const
{
    if (is_dictionary())
        return {pimpl->dict_head};
    else
        throw json_exception {"error calling begin_dictionary(): element is not a dictionary"};
}

json::const_dictionary_iterator json::end_dictionary() const
{
    if (is_dictionary())
        return {nullptr};
    else
        throw json_exception {"error calling end_dictionary(): element is not a dictionary"};
}



//$------------------------------------------- OPERATOR []

json& json::operator[](std::string const& key)
{
    if (!is_dictionary())
        throw json_exception {"error using operator[]: element is not a dictionary"};

    dictionary_iterator it {pimpl->dict_head};

    bool found {false};
    while (found == false  &&  it != end_dictionary())
    {
        if (key == it->first)
            found = true;
        else
            ++it;
    }

    if (found == true)      // verifico se ho trovato la chiave, se non c'è aggiungo
        return it->second;
    else
    {
        json value {};
        std::pair<std::string,json> keyvalue {key, value};

        insert(keyvalue);

        return pimpl->dict_tail->info.second;
    }
}

json const& json::operator[](std::string const& key) const
{
    if (!is_dictionary())
        throw json_exception {"error using operator[]: element is not a dictionary"};
    
    const_dictionary_iterator it {pimpl->dict_head};

    bool found {false};
    while (found == false  &&  it != end_dictionary())
    {
        if (key == it->first)
            found = true;
        else
            ++it;
    }

    if (found == true)
        return it->second;
    else
        throw json_exception {"error using operator[] const: key not found"};
}



//$------------------------------------------- CONSTRUCTORS & DESTRUCTORS

// COSTRUTTORE DI DEFAULT DI JSON
json::json() : pimpl{ new impl{} } {}   //? faccio in modo che nel momento in cui creo un json questo assuma il valore null

// DISTRUTTORE DI JSON
json::~json() {
    if (pimpl != nullptr) {
        pimpl->destroy();
        delete pimpl;
    }
}

// COSTRUTTORE DI COPIA DI JSON
json::json(json const& jsn) : pimpl{nullptr} { *this = jsn; }

// MOVE CONSTRUCTOR DI JSON
json::json(json&& jsn) : pimpl{nullptr} { *this = std::move(jsn); }



//$------------------------------------------- OPERATOR =

json& json::operator=(json const& jsn)
{
    if (this == &jsn) return *this;

    if (pimpl != nullptr)
    {
        pimpl->reset();
        delete pimpl;
        pimpl = nullptr;
    }

    switch (jsn.pimpl->type)
    {
        case type_null: {
            set_null();
            break;
        }
        case type_boolean: {
            set_bool(jsn.pimpl->boolean);
            break;
        }
        case type_number: {
            set_number(jsn.pimpl->num);
            break;
        }
        case type_string: {
            set_string(jsn.pimpl->str);
            break;
        }
        case type_list:
        {
            set_list();

            list_node* index = jsn.pimpl->list_head;
            while (index != nullptr) {
                push_back(index->info);
                index = index->next;
            }

            break;
        }
        case type_dictionary:
        {
            set_dictionary();

            dict_node* index = jsn.pimpl->dict_head;
            while (index != nullptr) {
                insert(index->info);
                index = index->next;
            }
        }
    }

    return *this;
}

json& json::operator=(json&& jsn)
{
    if (this == &jsn) return *this;

    if (pimpl != nullptr)
    {
        pimpl->reset();
        delete pimpl;
        pimpl = nullptr;
    }

    pimpl = jsn.pimpl;
    jsn.pimpl = nullptr;

    return *this;
}



//$------------------------------------------- DESTROY FUNCTIONS

void json::impl::reset()
{    
    destroy();

    type = type_null;
    str = "";
    num = 0;
    boolean = false;
}

void json::impl::destroy()
{
    while (list_head != nullptr)
    {
        list_node* tmp = list_head;
        list_head = list_head->next;
        delete tmp;
    }
    list_tail = nullptr;

    while (dict_head != nullptr)
    {
        dict_node* tmp = dict_head;
        dict_head = dict_head->next;
        delete tmp;
    }
    dict_tail = nullptr;
}



//$------------------------------------------- GET FUNCTIONS

double& json::get_number()
{
    if (is_number())
        return pimpl->num;
    else
        throw json_exception {"error calling function get_number(): element is not a number"};
}
double const& json::get_number() const
{
    if (is_number())
        return pimpl->num;
    else
        throw json_exception {"error calling function get_number(): element is not a number"};
}

bool& json::get_bool()
{
    if (is_bool())
        return pimpl->boolean;
    else
        throw json_exception {"error calling function get_bool(): element is not a bool"};
}
bool const& json::get_bool() const
{
    if (is_bool())
        return pimpl->boolean;
    else
        throw json_exception {"error calling function get_bool(): element is not a bool"};
}

std::string& json::get_string()
{
    if (is_string())
        return pimpl->str;
    else
        throw json_exception {"error calling function get_string(): element is not a string"};
}
std::string const& json::get_string() const
{
    if (is_string())
        return pimpl->str;
    else
        throw json_exception {"error calling function get_string(): element is not a string"};
}



//$------------------------------------------- SET FUNCTIONS

void json::set_string(std::string const& s)
{
    if (pimpl == nullptr)
        pimpl = new impl {};
    else
        pimpl->reset();

    pimpl->str = s;
    pimpl->type = type_string;
}

void json::set_bool(bool b)
{
    if (pimpl == nullptr)
        pimpl = new impl {};
    else
        pimpl->reset();

    pimpl->boolean = b;
    pimpl->type = type_boolean;
}

void json::set_number(double n)
{
    if (pimpl == nullptr)
        pimpl = new impl {};
    else
        pimpl->reset();

    pimpl->num = n;
    pimpl->type = type_number;
}

void json::set_null()
{
    if (pimpl == nullptr)
        pimpl = new impl {};  // il json è null di default
    else
        pimpl->reset();
}

void json::set_list()
{
    if (pimpl == nullptr)
        pimpl = new impl {};
    else
        pimpl->reset();

    pimpl->type = type_list;
}

void json::set_dictionary()
{
    if (pimpl == nullptr)
        pimpl = new impl {};
    else
        pimpl->reset();

    pimpl->type = type_dictionary;
}



//$------------------------------------------- PUSH FUNCTIONS

void json::push_back(json const& jsn)
{
    if (!is_list())
        throw json_exception {"error calling function push_back(): element is not a list"};
    
    list_node* newNode = new list_node {jsn, nullptr};

    if (pimpl->list_head == nullptr)                // lista vuota questo è il primo nodo
        pimpl->list_head = newNode;
    else
        pimpl->list_tail->next = newNode;

    pimpl->list_tail = newNode;
}

void json::push_front(json const& jsn)
{
    if (!is_list())
        throw json_exception {"error calling function push_front(): element is not a list"};

    list_node* newNode = new list_node {jsn};

    if (pimpl->list_head == nullptr)                // lista vuota questo è il primo nodo
    {
        newNode->next = nullptr;
        pimpl->list_head = pimpl->list_tail = newNode;
    }
    else
    {
        newNode->next = pimpl->list_head;
        pimpl->list_head = newNode;
    }
}

void json::insert(std::pair<std::string, json> const& keyvalue)
{
    if (!is_dictionary())
        throw json_exception {"error calling function insert(): element is not a dictionary"};

    dict_node* newNode = new dict_node {keyvalue, nullptr};

    if (pimpl->dict_head == nullptr)                // dizionario vuoto questo è il primo nodo
        pimpl->dict_head = newNode;
    else
        pimpl->dict_tail->next = newNode;

    pimpl->dict_tail = newNode;
}



//$------------------------------------------- CHECK TYPE FUNCTIONS

bool json::is_list()       const   { return pimpl->type == type_list; }
bool json::is_dictionary() const   { return pimpl->type == type_dictionary; }
bool json::is_string()     const   { return pimpl->type == type_string; }
bool json::is_number()     const   { return pimpl->type == type_number; }
bool json::is_bool()       const   { return pimpl->type == type_boolean; }
bool json::is_null()       const   { return pimpl->type == type_null; }



//$------------------------------------------- HELPER FUNCTIONS for PARSER

bool stringToBool(std::string const& s)
{
    if (s == "false")
        return false;
    else
        return true;
}

// Questa funzione estrae finché non trova un carattere che non sia uno spazio o un newline
void skip_spaces(std::istream& is)
{
    char ch {0};
    do {
        is >> ch;
    } while (ch == ' ' || ch == '\n');

    is.putback(ch);     // poi ributta dentro il carattere serio
}

void check(std::string tag, Type type)  // check per il tipo
{
    switch (type)
    {
        case type_null: {
            if (tag != "null")
                throw json_exception {"error reading null"};
            break;
        }
        case type_boolean: {
            if (tag != "true" && tag != "false")
                throw json_exception {"error reading boolean"};
        }
    }
}

void check(char read, char expected) {   // check per l'uguaglianza tra char
    if (read != expected)
        throw json_exception {"error reading key"};
}


//$------------------------------------------- PARSING FUNCTIONS

void         parse_null             (std::istream&, json&);
void         parse_boolean          (std::istream&, json&, char);
void         parse_number           (std::istream&, json&);
void         parse_string           (std::istream&, json&);
std::string  parse_key              (std::istream&);
void         parse_list_node        (std::istream&, json&);
void         parse_dictionary_node  (std::istream&, json&);


std::istream& operator>>(std::istream& lhs, json& rhs)
{
    char ch {0};
    lhs >> ch;

    if (lhs.eof())
        return lhs;

    switch (ch)
    {
        case 'n': {
            lhs.putback(ch);
            parse_null(lhs, rhs);
            break;
        }
        case 'f': case 't': {
            lhs.putback(ch);
            parse_boolean(lhs, rhs, ch);
            break;
        }
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9': {
            lhs.putback(ch);
            parse_number(lhs, rhs);
            break;
        }
        case '"': {
            parse_string(lhs, rhs);           // non ributta dentro le virgolette
            break;
        }
        case '[': {
            rhs.set_list();
            skip_spaces(lhs);
            if (lhs.peek() != ']')               //* se non c'è parentesi chiusa leggo normalmente
                parse_list_node(lhs, rhs);        // non ributta dentro la parentesi [
            else
                lhs >> ch;                       //* se c'è parentesi chiusa è json vuoto quindi mangia ']' e non fa niente
            break;
        }
        case '{': {
            rhs.set_dictionary();
            skip_spaces(lhs);
            if (lhs.peek() != '}')               //* se non c'è parentesi chiusa leggo normalmente
                parse_dictionary_node(lhs, rhs);  // non ributta dentro la parentesi {
            else
                lhs >> ch;                       //* se c'è parentesi chiusa è json vuoto quindi mangia '}' e non fa niente
            break;
        }
        default:
            throw json_exception {"error reading json"};
    }

    return lhs;
}


void parse_null(std::istream& is, json& jsn)
{
    char buf [4];
    is.read(buf, 4);
    std::string tag {buf};

    check(tag, type_null);

    jsn.set_null();
}
//* finita questa funzione lo istream è appena dopo la 'l' di null


void parse_boolean(std::istream& is, json& jsn, char b)
{
    char buf_f [5] {};     // false
    char buf_t [4] {};     // true
    std::string tag {};

    if (b == 'f') {
        is.read(buf_f, 5);
        tag = buf_f;
    }
    else {
        is.read(buf_t, 4);
        tag = buf_t;
    }

    check(tag, type_boolean);

    jsn.set_bool(stringToBool(tag));
}
//* finita questa funzione lo stream è appena dopo l'ultima lettera di true o false


void parse_number(std::istream& is, json& jsn)
{
    double tag {};

    is >> tag;

    jsn.set_number(tag);
}
//* finita questa funzione lo stream è appena dopo l'ultima cifra


void parse_string(std::istream& is, json& jsn)
{
    std::string tag {};
    char ch {0};

    bool found {false};
    while (found == false)
    {
        is.get(ch);             // estraggo il primo carattere della stringa
        if (ch == '\\')         // se è \ aggiungo lui e quello dopo
        {
            tag += ch;
            is.get(ch);
            tag += ch;
        }
        else if (ch == '"')     // se sono virgolette ho finito, non aggiungo e termino
            found = true;
        else                    // se è un qualsiasi altro carattere lo aggiungo
            tag += ch;
    }

    jsn.set_string(tag);
}
//* finita questa funzione lo istream è appena dopo "

std::string parse_key(std::istream& is)
{
    std::string tag {""};
    char ch {0};

    is >> ch;                   // estraggo le virgolette iniziali
    check(ch, '"');             // controllo che sia l'inizio di una chiave

    bool found {false};
    while (found == false)
    {
        is.get(ch);             // estraggo il primo carattere della stringa
        if (ch == '\\')         // se è \ aggiungo lui e quello dopo
        {
            tag += ch;
            is.get(ch);
            tag += ch;
        }
        else if (ch == '"')     // se sono virgolette ho finito, non aggiungo e termino
            found = true;
        else                    // se è un qualsiasi altro carattere lo aggiungo
            tag += ch;
    }

    return tag;
}

void parse_list_node(std::istream& is, json& jsn)
{
    skip_spaces(is);       // estraggo fino a un carattere serio

    json newJson {};
    is >> newJson;

    jsn.push_back(newJson);

    skip_spaces(is);
    char ch {0};

    is >> ch;
    if (ch == ',')
        parse_list_node(is, jsn);
    else if (ch == ']')
        skip_spaces(is);
    else
        throw json_exception {"error reading list"};
}


void parse_dictionary_node(std::istream& is, json& jsn)
{
    skip_spaces(is);       // estraggo fino a un carattere serio

    std::pair<std::string,json> keyvalue {};
    
    keyvalue.first = parse_key(is);

    char ch {0};
    skip_spaces(is);
    is >> ch;
    if (ch == ':')
        skip_spaces(is);
    else
        throw json_exception {"error reading dictionary"};

    is >> keyvalue.second;

    jsn.insert(keyvalue);

    skip_spaces(is);
    is >> ch;
    if (ch == ',')
        parse_dictionary_node(is, jsn);
    else if (ch == '}')
        skip_spaces(is);
    else
        throw json_exception {"error reading dictionary"};
}



//$------------------------------------------- PRINT FUNCTIONS

void print_list       (std::ostream&, json const&);
void print_dictionary (std::ostream&, json const&);
void print_bool       (std::ostream&, bool const&);


std::ostream& operator<<(std::ostream& lhs, json const& rhs)
{
    if (rhs.is_null()) {
        lhs << "null";
    }
    else if (rhs.is_bool()) {
        print_bool(lhs, rhs.get_bool());
    }
    else if (rhs.is_number()) {
        lhs << rhs.get_number();
    }
    else if (rhs.is_string()) {
        lhs << '"' << rhs.get_string() << '"';
    }
    else if (rhs.is_list()) {
        print_list(lhs, rhs);
    }
    else if (rhs.is_dictionary()) {
        print_dictionary(lhs, rhs);
    }
    else {
        throw json_exception {"error printing json"};
    }

    return lhs;
}

void print_bool(std::ostream& os, bool const& b)
{
    if (b == true)
        std::cout << "true";
    else
        std::cout << "false";
}

void print_list(std::ostream& os, json const& jsn)
{
    os << std::endl << '[' << std::endl;

    json::const_list_iterator it = jsn.begin_list();
    while (it != jsn.end_list())
    {
        os << *it;

        if (++it != jsn.end_list())
            os << ", ";
    }

    os << std::endl << ']';
}

void print_dictionary(std::ostream& os, json const& jsn)
{
    os << std::endl << '{' << std::endl;

    json::const_dictionary_iterator it = jsn.begin_dictionary();
    while (it != jsn.end_dictionary())
    {
        os << '"' << it->first << "\" : " << it->second;

        if (++it != jsn.end_dictionary())
            os << ',' << std::endl;
    }

    os << std::endl << '}';
}