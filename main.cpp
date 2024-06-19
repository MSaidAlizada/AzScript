#include <iostream>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
using namespace std;

//LEXER
enum Token {
  tok_eof = -1,
  tok_def = -2,
  tok_extern = -3,
  tok_identifier = -4,
  tok_number = -5,
};
static std::string IdentifierStr;
static double NumVal;

static int gettok() {
  static int  LastChar = ' ';
  //skips all whitespace till gets a character
  while (isspace(LastChar))
    LastChar = getchar();
  //first we check if it is in [a-z]
  if(isalpha(LastChar)){
    IdentifierStr = LastChar;
    //checks if the next chars are alphanumeric and adds it to the identifier name
    while(isalnum((LastChar=getchar()))){
      IdentifierStr += LastChar;
    }
    //checks if the identifier name is teyin or xarici
    if(IdentifierStr == "teyin"){
      return tok_def;
    }
    if(IdentifierStr == "xarici"){
      return tok_extern;
    }
    return tok_identifier;
  }
  //next we check if it is numVal
  while(isdigit(LastChar) || LastChar == '.'){
    std::string NumStr;
    //Since you can't have multiple decimal points in the we will have a flag to check if it has been added already
    bool DecimalPoint = false;
    if(LastChar == '.'){
      DecimalPoint = true;
    }
    do{
      NumStr += LastChar;
      LastChar = getchar();
      if(LastChar == '.'){
        DecimalPoint = true;
      }
    } while(isdigit(LastChar) || (LastChar == '.' && DecimalPoint == false));
    //we then convert the NumStr to a double
    NumVal = strtod(NumStr.c_str(),0);
    return tok_number;
  }
  //Next we check if it is a comment
  if (LastChar == '>'){
    //Clear the line by getting the characters till EOF, new line or return line
    do{
      LastChar = getchar();
    }while(LastChar != EOF && LastChar != '\n' && LastChar != '\r');
  }
  //Then we check if it is EOF so we can return that token
  if(LastChar == EOF){
    return tok_eof;
  }
  //If none of the above then it is an unknown character so we just return it's ASCII value
  int ThisChar = LastChar;
  LastChar = getchar();
  return ThisChar;
}

int main() {
  while (true){
    int tok = gettok();
    cout << "got token " << tok << endl;
  }
  return 0;
};

//ABSTRACT SYNTAX TREE
//The types of nodes in the AST is split into 3 sections Expressions, Prototypes and Functions

//Expressions
//For all expressions they will be derived from a base class called ExprAST
class ExprAST{
  public: virtual ~ExprAST() = default;
};
//The expressions we will have nodes for numeric literals, referencing variables, binary operators and function calls
class NumberExprAST : public ExprAST{
  double Val;

  public:
    NumberExprAST(double Val) : Val(Val) {}
};

class VariableExprAST : public ExprAST{
  string Name;
  public:
    VariableExprAST(const string &Name) : Name(Name) {}
};

class BinaryExprAST : public ExprAST{
  char Op;
  unique_ptr<ExprAST> LHS, RHS;
  public:
    BinaryExprAST(char Op, unique_ptr<ExprAST> LHS, unique_ptr<ExprAST>  RHS) : Op(Op), LHS(move(LHS)), RHS(move(RHS)){}
};

class CallExprAST : public ExprAST{
  string Callee;
  vector<unique_ptr<ExprAST>> Args;
  public:
    CallExprAST(string Callee, vector<unique_ptr<ExprAST>> Args) : Callee(Callee), Args(move(Args)) {}
};

//Prototype
//Used to get the name of function and its arguements
class PrototypeAST{
  string Name;
  vector<string> Args;
  public:
    PrototypeAST(const string &Name, vector<string> Args) : Name(Name), Args(move(Args)) {}
};

//Function
//Represents functions deifintion itself which is a combination of Prototype and Body
class FunctionAST{
  unique_ptr<PrototypeAST> Proto;
  unique_ptr<ExprAST> Body;
  public:
    FunctionAST(unique_ptr<PrototypeAST> Proto, unique_ptr<ExprAST> Body) : Proto(move(Proto)), Body(move(Body)) {}
};