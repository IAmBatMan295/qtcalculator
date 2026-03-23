#include "math_expression.hpp"
#include <cmath>
#include <stdexcept>
#include <vector>

constexpr double PI = 3.14159265358979323846;
constexpr double E = 2.71828182845904523536;

// A factorial function that takes and return a double for convinience
// but internally it checks for domain erros and works only if the given double
// is not a fraction or negative number.
double factorial(double n) {

    if (n < 0)
        throw std::runtime_error("Factorial domain error.");
    if (std::floor(n) != n)
        throw std::runtime_error("Factorial domain error.");

    int x = static_cast<int>(n);

    // Factorial of 170 is the upper limit of what double can represent
    // as a number without having overflow.
    if (x > 170)
        throw std::runtime_error("Factorial overflow.");
    double result = 1.0;

    for (int i = 2; i <= x; i++)
        result *= i;

    return result;
}

double Eval_Expression(std::vector<Token> &postfix_exp) {

    // Keep track of oparands.
    // We push them in order and pop out when we encounter a function or an operator.
    std::vector<Token> oparand_stack;
    int token_iter = 0;
    int no_of_tokens = postfix_exp.size();

    while (token_iter < no_of_tokens) {

        // If its a number push to oparand stack.
        if (postfix_exp[token_iter].type == Token::Number)
            oparand_stack.push_back(postfix_exp[token_iter]);

        // If its a constant the push a token of type number with value of that constant.
        else if (postfix_exp[token_iter].type == Token::Constant) {
            switch (postfix_exp[token_iter].const_type) {
            case Token::PI: {
                oparand_stack.push_back(Token(Token::Number, PI));
                break;
            }
            case Token::E: {
                oparand_stack.push_back(Token(Token::Number, E));
                break;
            }
            }
        }

        // If its a function then we only require one oparand and we generate
        // a result based on the type of the fucntion.
        // Then we push the result to the oparand stack as a number.
        else if (postfix_exp[token_iter].type == Token::Function) {
            double oparand_one;
            double result;

            if (oparand_stack.empty())
                throw std::runtime_error("Stack Underflow");

            oparand_one = oparand_stack.back().token_value;
            oparand_stack.pop_back();

            switch (postfix_exp[token_iter].func_type) {
            case Token::LOG: {

                if (oparand_one <= 0)
                    throw std::runtime_error("Logarithmic Domain Error.");

                result = std::log10(oparand_one);
                break;
            }

            case Token::LN: {

                if (oparand_one <= 0)
                    throw std::runtime_error("Logarithmic Domain Error.");
                result = std::log(oparand_one);
                break;
            }

            case Token::SQRT: {
                if (oparand_one < 0)
                    throw std::runtime_error("Square Root Domain Error.");

                result = std::sqrt(oparand_one);
                break;
            }

            case Token::SIN: {
                result = std::sin(oparand_one);
                break;
            }

            case Token::COS: {
                result = std::cos(oparand_one);
                break;
            }

            case Token::TAN: {
                result = std::tan(oparand_one);
                break;
            }

            case Token::ASIN: {
                if (oparand_one < -1 || oparand_one > 1)
                    throw std::runtime_error("Trigonomatric Domain Error.");

                result = std::asin(oparand_one);
                break;
            }

            case Token::ACOS: {
                if (oparand_one < -1 || oparand_one > 1)
                    throw std::runtime_error("Trigonomatric Domain Error.");
                result = std::acos(oparand_one);
                break;
            }

            case Token::ATAN: {
                result = std::atan(oparand_one);
                break;
            }

            case Token::EXP: {
                result = std::exp(oparand_one);
                break;
            }

            case Token::POW10: {
                result = std::pow(10.0, oparand_one);
                break;
            }
            default:
                break;
            }
            oparand_stack.push_back(Token(Token::Number, result));
        }

        // If its an operator we do the same thing we calculate the result then
        // push it to the oparand stack.
        else if (postfix_exp[token_iter].type == Token::Operator) {

            if (oparand_stack.empty())
                throw std::runtime_error("Stack Underflow!");

            double oparand_one;
            double result;

            // We scan for factorial seperately. because it doesnt require
            // more than one oparand in stack to be calculated.
            if (postfix_exp[token_iter].token_character == '!') {
                oparand_one = oparand_stack.back().token_value;
                oparand_stack.pop_back();
                result = factorial(oparand_one);
            }

            else {

                // We require atleast 2 oparands for an operator.
                if (oparand_stack.size() < 2)
                    throw std::runtime_error("Format Error!");

                double oparand_two;

                oparand_two = oparand_stack.back().token_value;
                oparand_stack.pop_back();
                oparand_one = oparand_stack.back().token_value;
                oparand_stack.pop_back();

                switch (postfix_exp[token_iter].token_character) {
                case '-': {
                    result = oparand_one - oparand_two;
                    break;
                }
                case '+': {
                    result = oparand_one + oparand_two;
                    break;
                }
                case '*': {
                    result = oparand_one * oparand_two;
                    break;
                }
                case '/': {
                    if (oparand_two == 0)
                        throw std::runtime_error("Cannot divide by 0");

                    result = oparand_one / oparand_two;
                    break;
                }
                case '%': {
                    if (oparand_two == 0)
                        throw std::runtime_error("Cannot divide by 0");

                    result = std::fmod(oparand_one, oparand_two);
                    break;
                }
                case '^': {
                    if (oparand_one == 0 && oparand_two == 0)
                        throw std::runtime_error("0^0 is undefined.");

                    result = pow(oparand_one, oparand_two);
                } break;
                }
            }
            oparand_stack.push_back(Token(Token::Number, result));
        }
        token_iter++;
    }

    // handles infinity and undefined results.
    if (!std::isfinite(oparand_stack.back().token_value))
        throw std::runtime_error("Undefined.");

    // The final result can be signed -0 or +0 in IEEE-754.
    // So returning literal 0 fixes that.
    else if (oparand_stack.back().token_value == 0)
        return 0;

    // Final number can be a meant to integer with som floating point noise.
    // So This block handles 2 of such cases that can occur.
    else {

        // Round it to nearest integer.
        double nearest = std::round(oparand_stack.back().token_value);

        // If the difference is less than 10^-12 then
        // it was just noise so return that integer instead.
        if (std::fabs(oparand_stack.back().token_value - nearest) < 1e-12)
            return nearest;
        else {

            // If its not that near to an integer then this case covers it by first
            // multiplying with 10^12 then rounding it then dividing it agian.
            double rounded_result = oparand_stack.back().token_value * 1e12;
            rounded_result = std::round(rounded_result);
            rounded_result = rounded_result / 1e12;

            if (std::fabs(oparand_stack.back().token_value - rounded_result) < 1e-12)
                return rounded_result;
        }
    }

    return oparand_stack.back().token_value;
}
