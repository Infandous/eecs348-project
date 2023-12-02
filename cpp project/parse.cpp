#include <string>
#include <map>
#include <queue>
#include <cctype>
#include <iostream>
#include <ctype.h>
#include "calculation.h"

class Parser : public Calculation{
public:
    std::queue<std::string> parse(std::string input, map<std::string, double> variables){ //function to parse user input
        std::queue<std::string> q; //queue to hold tokens
        std::string num_token=""; //stores individual tokens
        map<std::string,double>::iterator it; //used to iterate over map
        int paren_count=0; //stores number of read open parenthesis vs number of closed parenthesis

        for (char c : input){ //for every character
            if (isdigit(c)){ //if c is a digit
                num_token+=c; //add to num_tokens
            }else{
                if (!num_token.empty()){ //if num_token contains data
                    q.push(num_token); //push to queue
                }
                it = variables.find(c); //see if char is a variable
                if (it==variables.end()){ //if iterator is not in the map
                    if (isalpha(c)){ //if c is a variable and not in the map
                        std::cerr<<"[*]Error parsing input. Invalid variable[*]"<<std::endl;
                        break;
                    }
                }else{ //variable is in map
                    q.push(std::to_string(it->second)); //convert double to string and push to queue
                    continue; //continue to next char
                }
                switch (c){ //check if parenthesis or operator
                    case '(':
                        q.push(c);
                        break;
                        paren_count++; //increment paren_count
                    case ')':
                        q.push(c);
                        break;
                        paren_count--; //decrement paren_count
                    case '+':
                        q.push(c);
                        break;
                    case '-':
                        q.push(c);
                        break;
                    case '*':
                        q.push(c);
                        break;
                    case '/':
                        q.push(c);
                        break;
                    case '%':
                        q.push(c);
                        break;
                    case '^':
                        q.push(c);
                        break;
                    default:
                        std:cerr<<"[*]Error parsing input. Invalid character[*]"<<std::endl; 
                }

            }
        }
        if (paren_count!=0){ //if there are not an equal number of opening and closing parenthesis
            std::cerr<<"[*]Error, not an equal number of opening and closing parenthesis[*]"<std::cerr;
        }
        return q;
    }
};
