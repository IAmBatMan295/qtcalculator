#include "math_expression.hpp"
#include <stdexcept>
#include <vector>

int precedence(char c) {
    switch (c) {
    case '+':
    case '-':
        return 1;

    case '*':
    case '/':
    case '%':
        return 2;

    case '^':
        return 3;

    default:
        return 0;
    }
}

std::vector<Token> To_Postfix(std::vector<Token> &tokenized_exp) {

    // Handle the implicit multiplication, we create an expanded token vector
    // where we add * explicitly to make the format correct for the parser.
    std::vector<Token> expanded_tokenized_exp;
    int token_iter = 0;
    int no_of_tokens = tokenized_exp.size();

    // We are checking in paris current and current+1 so we expclude the last index
    // as it can't generate further pairs.
    while (token_iter <= no_of_tokens - 2) {

        // First push the curent token to expanded token vector.
        expanded_tokenized_exp.push_back(tokenized_exp[token_iter]);

        // This is the main logic.
        // Only the token combinations from (Number,Constant,RParen,!) -> (Number, Constant, LParen, Function) can imply a hidden multiplication.
        // So we define two boolean variables for better readablitly to check the types of former and later tokens.
        // Though our lexer can never generate two tokens of type Number side by side thats simply impossible.
        bool left_ok = (tokenized_exp[token_iter].type == Token::Number || tokenized_exp[token_iter].type == Token::Constant || tokenized_exp[token_iter].type == Token::RParen ||
                        (tokenized_exp[token_iter].type == Token::Operator && tokenized_exp[token_iter].token_character == '!'));

        bool right_ok = (tokenized_exp[token_iter + 1].type == Token::Number || tokenized_exp[token_iter + 1].type == Token::Constant ||
                         tokenized_exp[token_iter + 1].type == Token::LParen || tokenized_exp[token_iter + 1].type == Token::Function);

        // If both sides are ok then we push a '*' token explicitly to expanded token vector.
        if (left_ok && right_ok) {
            expanded_tokenized_exp.push_back(Token(Token::Operator, '*'));
        }
        token_iter++;
    }
    // Since we only push current tokens, the last token will be left out so push it to expanded token vector too.
    expanded_tokenized_exp.push_back(tokenized_exp[token_iter]);

    // Reset the iterators and limit for expanded token vector.
    token_iter = 0;
    no_of_tokens = expanded_tokenized_exp.size();

    // Stack to keep track of operators.
    std::vector<Token> op_stack;

    // Stack to generate the resultant vector of tokens in postfix order.
    std::vector<Token> postfix_exp;

    while (token_iter < no_of_tokens) {

        // If its a number...
        if (expanded_tokenized_exp[token_iter].type == Token::Number) {

            // Directly push to the resultant vector.
            postfix_exp.push_back(expanded_tokenized_exp[token_iter]);
            token_iter++;
        }

        // If its an operator then we have several cases.
        else if (expanded_tokenized_exp[token_iter].type == Token::Operator) {

            switch (expanded_tokenized_exp[token_iter].token_character) {

            // Well use seprate case for '-' to handle its unary behaviour.
            case '-': {

                // If the its at first index or there's a left parantheses on left, then also treat it as unary.
                //  So we add an extra 0 to resultant vector then push the '-' into the op_stack.
                if (token_iter == 0 || expanded_tokenized_exp[token_iter - 1].type == Token::LParen) {
                    postfix_exp.push_back(Token(Token::Number, 0.0));
                    op_stack.push_back(expanded_tokenized_exp[token_iter]);
                    break;

                }

                // If there's any operator on its left other than ! then also treat is as unary.
                else if (expanded_tokenized_exp[token_iter - 1].type == Token::Operator && expanded_tokenized_exp[token_iter - 1].token_character != '!') {
                    postfix_exp.push_back(Token(Token::Number, 0.0));
                    op_stack.push_back(expanded_tokenized_exp[token_iter]);
                    break;
                }

                // Otherwise treat is a binary.
                else {

                    // If the op_stack is empty or the top of the op_stack is a left parentheses,
                    // push the '-' to op_stack;
                    if (op_stack.empty() || (!op_stack.empty() && op_stack.back().type == Token::LParen))
                        op_stack.push_back(expanded_tokenized_exp[token_iter]);

                    // Otherwise we dont have to worry about precedence as it has the lowest precedence
                    // and we can just pop everything from the op_stack while unitl not empty or we encounter a left parentheses.parentheses.
                    // This can also cover the case where the op_stack is empty and we just push '-' to the op_stack but its good practice to cover
                    // that case manually as we did just above.
                    else {
                        while (!op_stack.empty() && op_stack.back().type != Token::LParen) {

                            postfix_exp.push_back(op_stack.back());
                            op_stack.pop_back();
                        }
                        op_stack.push_back(expanded_tokenized_exp[token_iter]);
                    }
                    break;
                }
            }
            case '+':
            case '*':
            case '/':
            case '%': {

                // Again, its good practice to cover the empty op_stack case here instead of letting it
                // get covered later in the 'else' block implicitly.
                if (op_stack.empty()) {
                    op_stack.push_back(expanded_tokenized_exp[token_iter]);
                    break;
                }

                // If top of op_stack is a left parentheses or precedence of the current operator is higher
                // than the one in top of op_stack then push it to op_stack.
                else if (op_stack.back().type == Token::LParen || precedence(expanded_tokenized_exp[token_iter].token_character) > precedence(op_stack.back().token_character))
                    op_stack.push_back(expanded_tokenized_exp[token_iter]);

                // Otherwise pop everything form op_stack as long as its an operator and not a left parantheses and push it to resultant vector.
                else {
                    while (!op_stack.empty() && op_stack.back().type != Token::LParen &&
                           precedence(op_stack.back().token_character) >= precedence(expanded_tokenized_exp[token_iter].token_character)) {

                        postfix_exp.push_back(op_stack.back());
                        op_stack.pop_back();
                    }
                    op_stack.push_back(expanded_tokenized_exp[token_iter]);
                }

                break;
            }

            // It is right associative and has the second highest priority among all operators. but since '!' is directly pushed
            // to the resultant vector, we push '^' directly to op_stack every time.
            case '^': {
                op_stack.push_back(expanded_tokenized_exp[token_iter]);
                break;
            }

            // Its a postfix unary operator with highest priority so just
            // directly push it to the resultant vector.
            case '!': {
                postfix_exp.push_back(expanded_tokenized_exp[token_iter]);
                break;
            }

            default:
                break;
            }
            // We checking for each token type in its own block so  we increament the iterator after every check instead
            // of increamenting it at the end just before while loop ends for better readability.
            token_iter++;
        }
        // If its a left parentheses, push it to op_stack.
        else if (expanded_tokenized_exp[token_iter].type == Token::LParen) {
            op_stack.push_back(expanded_tokenized_exp[token_iter]);
            token_iter++;
        }

        // If its a right parentheses...
        else if (expanded_tokenized_exp[token_iter].type == Token::RParen) {

            // Pop everything from op_stack until a left parentheses is encountered.
            while (!op_stack.empty() && op_stack.back().type != Token::LParen) {
                postfix_exp.push_back(op_stack.back());
                op_stack.pop_back();
            }
            // If the op_stack is empty, then we didn't have the pairity parantheses.
            // So we throw a format error.
            // Only handles ')' non-pairity.
            if (op_stack.empty())
                throw std::runtime_error("Format Error!");
            op_stack.pop_back();

            // Covers the case is the most recent pair of parentheses was a control block for an function input.
            if (!op_stack.empty() && op_stack.back().type == Token::Function) {
                postfix_exp.push_back(op_stack.back());
                op_stack.pop_back();
            }
            token_iter++;
        }

        // If its an function push it to the op_stack.
        else if (expanded_tokenized_exp[token_iter].type == Token::Function) {
            op_stack.push_back(expanded_tokenized_exp[token_iter]);
            token_iter++;
        }

        // Finally it has to be a constant. so push it directly to resultant stack.
        else {
            postfix_exp.push_back(expanded_tokenized_exp[token_iter]);
            token_iter++;
        }
    }

    // Handles operator noise such as ++,((,)), () etc where there are no numbers.
    if (postfix_exp.empty())
        throw std::runtime_error("Format Error!");

    // At last, if op_stack has any tokens left, pop them and push them into resultant vector.
    while (!op_stack.empty()) {

        // If there was still an LParen left in operator stack then that means there was an extra '('.
        if (op_stack.back().type == Token::LParen)
            throw std::runtime_error("Format Error!");

        postfix_exp.push_back(op_stack.back());
        op_stack.pop_back();
    }

    return postfix_exp;
}
