#include <string>
#include <map>
#include <queue>
#include <cctype>
#include <iostream>
#include <ctype.h>
#include "calculation.h"

class Tokenizer{
public:
    std::queue<std::string> tokenize(std::string input, std::map<std::string, double> variables){ //function to tokenize user input
        std::queue<std::string> q; //queue to hold tokens
        std::string num_token=""; //stores individual tokens
        std::map<std::string,double>::iterator it; //used to iterate over map
        int paren_count=0; //stores number of read open parenthesis vs number of closed parenthesis

        for (char c : input){ //for every character
            if (isdigit(c)){ //if c is a digit
                num_token+=c; //add to num_tokens
            }else{
                if (!num_token.empty()){ //if num_token contains data
                    q.push(num_token); //push to queue
                    num_token = ""; //reset num_token
                }
                if (c==' '){ //if char is a space
                    continue; //continue
                }
                it = variables.find(std::string(1,c)); //see if char is a variable
                if (it==variables.end()){ //if iterator is not in the map
                    if (isalpha(c)){ //if c is a variable and not in the map
                        std::cerr<<"[*]Error tokenizing input. Invalid variable[*]"<<std::endl;
                        break;
                    }
                }else if (variables.count(std::string(1,c))){ //variable is in map
                    q.push(std::to_string(it->second)); //convert double to string and push to queue
                    continue; //continue to next char
                }
                switch (c){ //check if parenthesis or operator
                    case '(':
                        q.push(std::string(1,c));
                        paren_count++; //increment paren_count
                        break;
                    case ')':
                        if (paren_count<1){ //there is no opening parenthesis, give error
                            std::cerr<<"[*]Error, closing parenthesis without matching opening parenthesis found[*]"<<std::endl;
                        }
                        q.push(std::string(1,c));
                        paren_count--; //decrement paren_count
                        break;
                    case '+':
                        q.push(std::string(1,c));
                        break;
                    case '-':
                        q.push(std::string(1,c));
                        break;
                    case '*':
                        q.push(std::string(1,c));
                        break;
                    case '/':
                        q.push(std::string(1,c));
                        break;
                    case '%':
                        q.push(std::string(1,c));
                        break;
                    case '^':
                        q.push(std::string(1,c));
                        break;
                    default:
                        std::cerr<<"[*]Error tokenizing input. Invalid character[*]"<<std::endl; 
                }

            }
        }

        if (!num_token.empty()){ //if num_token is not empty after loop, push to queue
            q.push(num_token);
        }

        if (paren_count!=0){ //if there are not an equal number of opening and closing parenthesis
            std::cerr<<"[*]Error, not an equal number of opening and closing parenthesis[*]"<<std::endl;
        }
        return q;
    }
};
