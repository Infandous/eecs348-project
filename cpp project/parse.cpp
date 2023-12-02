#include <stack>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdbool.h>

class Parser{
private:
    struct ASTNode{ //basic node for AST
        std::string value; //holds token
        ASTNode* left;
        ASTNode* right;
        
        ASTNode(std::string val) : value(val), left(nullptr), right(nullptr) {}
    };
    bool isNum(const std::string s){ //helper function used to determine if a string is a valid number
        char *end;
        strtod(s.c_str(),&end);
        if(end==s.c_str() || *end!='\0')
            return false; //return false if not a number
        return true; //otherwise return true
    }
    bool isOperator(const std::string s){ //helper function to determine if a string is an operator
        switch (s){ //returns true if equivalent operator found, default returns false
            case ("+"):
                return true;
            case ("-"):
                return false;
            case ("*"):
                return true;
            case ("/"):
                return true;
            case ("%"):
                return true;
            case ("^"):
                return true;
            default:
                return false;
        }
    }
    int getPrecedence(const std::string& s){ //helper function to get the precedence of an operator as a numerical value
        if (s=="^"){
            return 3;
        }else if(s=="*"||s=="/"||s=="%"){
            return 2;
        }else if (s=="+"||s=="-"){
            return 1;
        }else{
            return 0;
        }
    }
    bool hasPrecedence(const std::string& s1, const std::string& s2){ //helper function to determine if s1 has precedence over s2
        //get precedence of s1 and s2
        int s1Prec = getPrecedence(s1);
        int s2Prec = getPrecedence(s2);

        return s1Prec>s2Prec; //return whether s1 has higher precedence than s2
    }
public:
    //parse expression using shunting yard algorithm
    //return root node
    ASTNode * parseExpression(const std::queue<std::string> tokens){
        std::stack<std::string> op_stack; //holds operators
        std::stack<ASTNode*> node_stack; //holds ASTNode's

        while(!tokens.empty()){ //while there are still tokens to parse
            std::string token = tokens.front(); //get front of queue
            
            if (isNum(token)){ //if the token is a number
                //create new node and push to node_stack
                ASTNode *n = new ASTNode(token); 
                node_stack.push(node);
            }else if(isOperator(token)){ //of token is an operator
                //while op_stack is not empty, the top is not an open paren and the top of the stack has greater precedence than the token
                while (!op_stack.empty()&&op_stack.top()!="("&&hasPrecedence(op_stack.top(),token)){
                    std::string op = op_stack.top(); //get value from the top of the stack
                    op_stack.pop();//pop value from the top of the stack

                    //get the right hand operands
                    //store them in ASTNode's
                    ASTNode* r_op = node_stack.top();
                    node_stack.pop();
                    ASTNode* l_op = node_stack.top();
                    node_stack.pop();

                    //create new ASTNode for the operator
                    //add r_op and l_op as children
                    ASTNode *op_node = new ASTNode(op);
                    op_node->right=r_op;
                    op_node->left=l_op;

                    node_stack.push(op_node); //push new node to the node_stack
                }
                op_stack.push(token); //push operator token to the op_stack
            }else if(token=="("){

            }else if (token==")"){

            }
            


            tokens.pop(); //pop token from queue
        }
    }
    
    
}
