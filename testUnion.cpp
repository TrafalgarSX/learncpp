#include <iostream>

class Token{
    friend std::ostream& operator<<(std::ostream& os, const Token& t);
public:
    Token() : tok(INT), ival{0} {}
    ~Token() { if(tok == STR) sval.~basic_string(); }

    Token(const Token &t) : tok(t.tok) { copyUnion(t); }
    Token& operator=(const Token &t);

    Token(Token &&t) noexcept;
    Token& operator=(Token &&t) noexcept;

    Token& operator=(int i); 
    Token& operator=(char c); 
    Token& operator=(double d);
    Token& operator=(const std::string &s);
    Token& operator=(std::string&& s);
private:
    enum {INT, CHAR, DBL, STR} tok;
    union{
        int ival;
        char cval;
        double dval;
        std::string sval;
    };

    // 检查判别式，然后酌情拷贝 union 成员
    void copyUnion(const Token &t);
    void moveUnion(Token &&t);

};

Token &Token::operator=(int i){
    if(tok == STR) sval.~basic_string();
    ival = i;
    tok = INT;
    return *this;
}

Token &Token::operator=(char c){
    if(tok == STR) sval.~basic_string();
    cval = c;
    tok = CHAR;
    return *this;
}

Token &Token::operator=(double d){
    if(tok == STR) sval.~basic_string();
    dval = d;
    tok = DBL;
    return *this;
}

Token &Token::operator=(const std::string &s){
    if(tok == STR){
        sval = s;
    }
    else{
        new(&sval) std::string(s);
        tok = STR;
    }
    return *this;
}

Token &Token::operator=(const Token &t){
    if(tok == STR && t.tok != STR) sval.~basic_string();
    if(tok == STR && t.tok == STR)
        sval = t.sval;
    else
        copyUnion(t);
    tok = t.tok;
    return *this;
}

void Token::copyUnion(const Token &t){
    switch(t.tok){
    case Token::INT:
        ival = t.ival;
        break;
    case Token::CHAR:
        cval = t.cval;
        break;
    case Token::DBL:
        dval = t.dval;
        break;
    case Token::STR:
        new(&sval) std::string(t.sval);
        break;
    }
}

void Token::moveUnion(Token &&t){
    switch(t.tok){
    case Token::INT:
        ival = t.ival;
        break;
    case Token::CHAR:
        cval = t.cval;
        break;
    case Token::DBL:
        dval = t.dval;
        break;
    case Token::STR:
        new(&sval) std::string(std::move(t.sval));
        break;
    }
}

Token::Token(Token &&t) noexcept : tok(t.tok){
    moveUnion(std::move(t));

}

Token& Token::operator=(Token &&t) noexcept{
    if(this == &t) return *this;
    if(tok == STR && t.tok != STR) sval.~basic_string();
    if(tok == STR && t.tok == STR)
        sval = std::move(t.sval);
    else
        moveUnion(std::move(t));
    return *this;
}


std::ostream& operator<<(std::ostream& os, const Token& t){
    switch(t.tok){
    case Token::INT:
        os << t.ival;
        break;
    case Token::CHAR:
        os << t.cval;
        break;
    case Token::DBL:
        os << t.dval;
        break;
    case Token::STR:
        os << t.sval;
        break;
    }
    return os;
}


Token& Token::operator=(std::string&& s){
    if(tok == STR){
        sval = std::move(s);
    }
    else{
        new(&sval) std::string(std::move(s));
        tok = STR;
    }
    return *this;
}

int main(int argc, const char** argv) {

    Token t;
    t = 'a';
    std::cout << t << std::endl;

    t = 10;
    std::cout << t << std::endl;

    t = 3.14;
    std::cout << t << std::endl;

    t = "hello";
    std::cout << t << std::endl;

    Token t1 = t;
    std::cout << t1 << std::endl;

    Token t2 = std::move(t);
    std::cout << t1 << std::endl;

    Token t3;
    t3 = t2;

    Token t4;
    t4 = std::move(t2);
    return 0;
}