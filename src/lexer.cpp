#include "math_expression.hpp"
#include <stdexcept>
#include <string>
#include <vector>
std::vector<Token> Tokenize_Expression(std::string expression) {

    // Handle Edge cases like when user input is only .,(,),+,! etc...
    if (expression.length() == 1 && !(expression[0] <= '9' && expression[0] >= '0'))
        throw std::runtime_error("Format Error!");

    std::vector<Token> tokenized_exp;
    int exp_len = expression.length();
    int exp_iter = 0;

    // A string of operators for checking if the character is
    // an operator or not.
    std::string all_opr = "+-*/%^!";

    // Iterate through the whole string.
    while (exp_iter < exp_len) {

        // If its a number(can start with '.' too) then store its integer and decimal parts in seperate varables.
        if ((expression[exp_iter] <= '9' && expression[exp_iter] >= '0') || expression[exp_iter] == '.') {
            double value = 0.0;

            // For later use, required to calculate decimal part.
            double division = 10.0;

            while (exp_iter < exp_len && expression[exp_iter] <= '9' && expression[exp_iter] >= '0') {
                value = (value * 10.0) + expression[exp_iter] - '0';
                exp_iter++;
            }
            // If there's a decimal symbol, skip it.
            if (exp_iter < exp_len && expression[exp_iter] == '.')
                exp_iter++;

            // If its a number after the decimal then keep adding it to the integer part as a decimal.
            while (exp_iter < exp_len && expression[exp_iter] <= '9' && expression[exp_iter] >= '0') {

                // Add it after converting it to the correct fraction by dividing
                // it by 10^n where the current digit being added as a fraction is at n-th position
                // from the decimal.
                value += (expression[exp_iter] - '0') / division;
                division *= 10.0;
                exp_iter++;
            }

            // Cover the malformed numbers like 1.1.1, 5.6.7 etc...
            if (exp_iter < exp_len && expression[exp_iter] == '.')
                throw std::runtime_error("Format Error!");

            tokenized_exp.push_back(Token(Token::Number, value));
        }

        // If its a lowercase then we expect an function.
        else if (expression[exp_iter] >= 'a' && expression[exp_iter] <= 'z') {

            // Gneerate a string of that funcion.
            std::string function;

            Token temp = Token(Token::Function);

            // Determine the type of that fucntion by  using the string compare with if-else if ladder.
            // And then push it into the token vector.
            while (exp_iter < exp_len && expression[exp_iter] >= 'a' && expression[exp_iter] <= 'z') {
                function.push_back(expression[exp_iter]);
                exp_iter++;
            }

            if (function == "log") {
                temp.func_type = Token::LOG;
            } else if (function == "ln") {
                temp.func_type = Token::LN;
            } else if (function == "sqrt") {
                temp.func_type = Token::SQRT;
            } else if (function == "exp") {
                temp.func_type = Token::EXP;
            } else if (function == "sin") {
                temp.func_type = Token::SIN;
            } else if (function == "cos") {
                temp.func_type = Token::COS;
            } else if (function == "tan") {
                temp.func_type = Token::TAN;
            } else if (function == "asin") {
                temp.func_type = Token::ASIN;
            } else if (function == "acos") {
                temp.func_type = Token::ACOS;
            } else if (function == "atan") {
                temp.func_type = Token::ATAN;
            } else if (function == "powt") {
                temp.func_type = Token::POW10;
            }

            tokenized_exp.push_back(temp);
        }

        // If its an uppercase letter then we expect a constant.
        else if (expression[exp_iter] >= 'A' && expression[exp_iter] <= 'Z') {

            // We do the same procedure here we did with function type.
            std::string constant;
            Token temp = Token(Token::Constant);

            while (exp_iter < exp_len && expression[exp_iter] >= 'A' && expression[exp_iter] <= 'Z') {

                constant.push_back(expression[exp_iter]);
                if (constant == "PI") {
                    temp.const_type = Token::PI;
                    exp_iter++;
                    break;
                } else if (constant == "E") {
                    temp.const_type = Token::E;
                    exp_iter++;
                    break;
                }
                exp_iter++;
            }

            tokenized_exp.push_back(temp);
        }

        // If its a character from the string all_opr which we defined earlier then
        // its an operator.
        else if (all_opr.find(expression[exp_iter]) != std::string::npos) {
            tokenized_exp.push_back(Token(Token::Operator, expression[exp_iter]));
            exp_iter++;
        }
        // If its a left parentheses.
        else if (expression[exp_iter] == '(') {
            tokenized_exp.push_back(Token(Token::LParen, '('));
            exp_iter++;
        }
        // If its a right parentheses.
        else if (expression[exp_iter] == ')') {
            tokenized_exp.push_back(Token(Token::RParen, ')'));
            exp_iter++;
        }
        // If its anything else other than the above mentioned cases,then throw an error.
        // we'll have controlled input but still keep it as a safety gaurd.
        else
            throw std::runtime_error("Unsupported input.");
    }

    return tokenized_exp;
}
