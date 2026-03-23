// This header includes functions and datatypes required for
// parsing and evaluation of an expression.
#pragma once

#include <string>
#include <vector>

// Definition of the our Tokens, either a number, operator,identifier,
// left/right parentheses, or a constant.
struct Token {

    enum Type { Number, Operator, LParen, RParen, Function, Constant };

    Type type;

    // Six token types are there, but these actual primitive data types
    // of a token.
    double token_value;
    char token_character;

    enum Func_Type { SIN, COS, TAN, LOG, LN, ASIN, ACOS, ATAN, EXP, POW10, SQRT };

    enum Const_Type { PI, E };

    Func_Type func_type;

    Const_Type const_type;

    // Constructors for each data type.
    Token(Token::Type type, double token_value) : type(type), token_value(token_value) {}

    Token(Token::Type type, char token_character) : type(type), token_character(token_character) {}

    Token(Token::Type type) : type(type) {};
};

// Return a tokenized vector of a given expression string.
std::vector<Token> Tokenize_Expression(std::string expression);

// Convert a vector of tokens in postfix order.
std::vector<Token> To_Postfix(std::vector<Token> &tokenized_exp);

// Evaluate an expression using its tokenized vector form.
double Eval_Expression(std::vector<Token> &postfix_exp);

// Helper function to return precedence of an operator.
int precedence(char c);

// Factorial function.
double factorial(double n);
