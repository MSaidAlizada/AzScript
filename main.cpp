#include <iostream>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

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
    //Since you can't have multiple decimal points in the 
    do{
      NumStr += LastChar;
      LastChar = getchar();
    } while(isdigit(LastChar) || LastChar == '.');
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
  return 0;
};