#include <string>
#include <vector>
#include <utility>
#include <memory>

enum Token{
    tok_eof = -1,

    tok_def = -2,
    tok_extern = -3,

    tok_identifier = -4,
    tok_number = -5
};

static std::string IdentifierStr;
static double NumVal;

static int gettok(){
    static int LastChar = ' ';

    while(isspace(LastChar)){
        LastChar = getchar();
    }

    if(isalpha(LastChar)){
        IdentifierStr = LastChar;
        while(isalnum(LastChar = getchar())){
            IdentifierStr += LastChar;
        }

        if(IdentifierStr == "def"){
            return tok_def;
        }
        if(IdentifierStr == "extern"){
            return tok_extern;
        }
        return tok_identifier;
    }

    if(isdigit(LastChar) || LastChar == '.'){
        std::string NumStr;

        do{
            NumStr += LastChar;
            LastChar = getchar();
        }while(isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), 0);
        return tok_number;
    }

    if(LastChar == '#'){
        do{
            LastChar = getchar();
        }while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if(LastChar != EOF) return gettok();
    }

    if(LastChar == EOF) return tok_eof;

    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}


class ExprAST{
public:
    virtual ~ExprAST() {}
};

class NumberExprAST : public ExprAST{
    double Val;

public:
    NumberExprAST(double Val): Val(Val) {}
};

class VariableExprAST : public ExprAST{
    std::string Name;

public:
    VariableExprAST(const std::string &Name):Name(Name) {}
};

class BinaryExprAST : public ExprAST{
    char Op;
    std::unique_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(char op, std::unique_ptr<ExprAST> LHS, 
                  std::unique_ptr<ExprAST> RHS):Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

class CallExprAST : public ExprAST{
    std::string Callee;
    std::vector< std::unique_ptr<ExprAST> > Args;

public:
    CallExprAST(const std::string &Callee,
                std::vector< std::unique_ptr<ExprAST> > Args):Callee(Callee), Args(std::move(Args)) {}
};

/// PrototypeAST - This class represents the "prototype" for a function,
/// which captures its name, and its argument names (thus implicitly the number
/// of arguments the function takes).
class PrototypeAST{
    std::string Name;
    std::vector<std::string> Args;

public:
    PrototypeAST(const std::string &name, std::vector<std::string> Args)
                :Name(name), Args(std::move(Args)) {}

    const std::string &getName() const { return Name; }
};

/// FunctionAST - This class represents a function definition itself.
class FunctionAST {
    std::unique_ptr<PrototypeAST> Proto;
    std::unique_ptr<ExprAST> Body;

public:
    FunctionAST(std::unique_ptr<PrototypeAST> Proto,
                std::unique_ptr<ExprAST> Body)
                : Proto(std::move(Proto)), Body(std::move(Body)) {}
};

static int CurTok;
static int getNextToken() {
    return CurTok = gettok();
}

static std::unique_ptr<ExprAST> ParseNumberExpr() {
    auto Result = std::make_unique<NumberExprAST>(NumVal);
    getNextToken();
    return std::move(Result);
}

static std::unique_ptr<ExprAST> ParseParenExpr() {
    getNextToken();
    auto V = ParseExPression();

    if(!V)
        return nullptr;

    if(CurTok != ')')
        return LogError("expected ')'");
    getNextToken();
    return V;
}