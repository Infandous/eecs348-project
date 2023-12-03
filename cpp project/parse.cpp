#include <stack>
#include <queue>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <stdbool.h>
#include <math.h>

class Parser{
private:
    /*struct ASTNode{ //basic node for AST
        std::string value; //holds token
        ASTNode* left;
        ASTNode* right;
        
        ASTNode(std::string val) : value(val), left(nullptr), right(nullptr) {}
    };*/
    bool isNum(const std::string s){ //helper function used to determine if a string is a valid number
        char *end;
        strtod(s.c_str(),&end);
        if(end==s.c_str() || *end!='\0')
            return false; //return false if not a number
        return true; //otherwise return true
    }
    bool isOperator(const std::string s){ //helper function to determine if a string is an operator
        if (s=="+" || s=="-" || s=="*" || s=="/" || s=="%" || s=="^"){ //if s is an operator
            return true; //return true
        }
        return false; //otherwise return false
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
    struct ASTNode{ //basic node for AST
        std::string value; //holds token
        ASTNode* left;
        ASTNode* right;

        ASTNode(std::string val) : value(val), left(nullptr), right(nullptr) {}
    };

    void deleteTree(ASTNode* node){//helper function to delete tree at end of main
        if (node==nullptr){
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
    //parse expression using shunting yard algorithm
    //return root node
    ASTNode * parseExpression(std::queue<std::string> tokens){
        std::stack<std::string> op_stack; //holds operators
        std::stack<ASTNode*> node_stack; //holds ASTNode's

        while(!tokens.empty()){ //while there are still tokens to parse
            std::string token = tokens.front(); //get front of queue
            
            if (isNum(token)){ //if the token is a number
                //create new node and push to node_stack
                ASTNode *n = new ASTNode(token); 
                node_stack.push(n);
            }else if(isOperator(token)){ //of token is an operator
                //while op_stack is not empty, the top is not an open paren and the top of the stack has greater precedence than the token
                while (!op_stack.empty()&&op_stack.top()!="("&&hasPrecedence(op_stack.top(),token)){
                    std::string op;
                    if (!op_stack.empty()){
                        op = op_stack.top(); //get value from the top of the stack
                        op_stack.pop();//pop value from the top of the stack
                    }

                    //create new ASTNode for the operator
                    ASTNode *op_node = new ASTNode(op);
                    //get the right hand operands
                    //store them in ASTNode's
                    ASTNode* r_op;
                    ASTNode* l_op;
                    if (!node_stack.empty()){
                        r_op = node_stack.top();
                        node_stack.pop();
                        op_node->right=r_op; 
                    }
                    if(!node_stack.empty()){
                        l_op = node_stack.top();
                        node_stack.pop();
                        op_node->left=l_op; 
                    }

                    //create new ASTNode for the operator
                    //add r_op and l_op as children
                    //ASTNode *op_node = new ASTNode(op);
                    //op_node->right=r_op;
                    //op_node->left=l_op;

                    node_stack.push(op_node); //push new node to the node_stack
                }
                op_stack.push(token); //push operator token to the op_stack
            }else if(token=="("){ //if opening parenthesis
                op_stack.push(token); //push token to op_stack
            }else if (token==")"){ //if closing parenthesis
                //find the equivalent opening parenthesis from op_stack
                while (!op_stack.empty() && op_stack.top()!="("){ //loop while "(" not found and the stack isn't empty
                    std::string op;
                    if(!op_stack.empty()){
                        op = op_stack.top(); //get the operator
                        op_stack.pop(); //pop from stack
                    }


                    //create new ASTNode for the operator
                    ASTNode *n = new ASTNode(op);
                    //get the right hand operands
                    //store them in ASTNode's
                    ASTNode* r_op;
                    ASTNode* l_op;
                    if (!node_stack.empty()){
                        r_op = node_stack.top();
                        node_stack.pop();
                        n->right=r_op; 
                    }
                    if(!node_stack.empty()){
                        l_op = node_stack.top();
                        node_stack.pop();
                        n->left=l_op; 
                    }

                    //get nodesfrom node_stack for left and right
                    //children of new node
                    /*ASTNode* r_op;
                    ASTNode *l_op;
                    if (!node_stack.empty()){
                        r_op = node_stack.top();
                        node_stack.pop(); 
                    }
                    if (!node_stack.empty()){
                        l_op = node_stack.top();
                        node_stack.pop();
                    }
                    

                    ASTNode *n = new ASTNode(op); //new node to push on node_stack
                    n->left = l_op;
                    n->right = r_op;*/
                    node_stack.push(n); //push new node to node_stack
                }
                if (!op_stack.empty()){
                    op_stack.pop(); //pop the opening parenthesis 
                }
                
            }
            


            tokens.pop(); //pop token from queue
        }

        //make sure all operators are popped from op_stack
        while (!op_stack.empty()){
            std::string op = op_stack.top(); //get operator
            op_stack.pop(); //pop top

            //create new ASTNode for the operator
            ASTNode *n = new ASTNode(op);
            //get the right hand operands
            //store them in ASTNode's
            ASTNode* r_op;
            ASTNode* l_op;
            if (!node_stack.empty()){
                r_op = node_stack.top();
                node_stack.pop();
                n->right=r_op; 
            }
            if(!node_stack.empty()){
                l_op = node_stack.top();
                node_stack.pop();
                n->left=l_op; 
            }

            //get left and right children from node_stack
            //for new node
            /*ASTNode* r_op;
            ASTNode* l_op;
            if (!node_stack.empty()){
                r_op = node_stack.top();
                node_stack.pop(); 
            }
            if(!node_stack.empty()){
                l_op = node_stack.top();
                node_stack.pop();
            }
            

            ASTNode *n = new ASTNode(op); //create new node for new operator
            n->left = l_op;
            n->right = r_op;*/

            node_stack.push(n); //push new node onto node_stack
        }

        if (!node_stack.empty()){ //there should only be one item left on the stack, more means theres an error
            ASTNode *root = node_stack.top(); //get root node from stack
            node_stack.pop();
            return root; //return root node
        }

        return nullptr; //returns nullptr as base case
    }

    //function to evaluate the expression
    double evaluate(ASTNode* node){
        if (node==nullptr){ //if the node is a nullptr, alert user and return 0
            std::cerr<<"Empty expression, returning 0"<<std::endl;
            return 0;
        }
        
        if (node->left==nullptr && node->right==nullptr){ //we're at the end
            return std::stod(node->value); //return the value at node
        }

        //use recursion to iterate over the tree
        double l_val = evaluate(node->left); 
        double r_val = evaluate(node->right);

        //check if node->value equals an operator and if so, evaluate
        if (node->value=="+"){
            return l_val+r_val;
        }else if(node->value=="-"){
            return l_val-r_val;
        }else if (node->value=="*"){
            return l_val*r_val;
        }else if (node->value=="/"){
            if (r_val==std::stod("0")){
                std::cerr<<"[*]Error, division by 0. Returning 0[*]"<<std::endl;
                return 0;
            }
            return l_val/r_val;
        }else if (node->value=="^"){
            return pow(l_val, r_val);
        }else if (node->value=="%"){
            return (int)l_val%(int)r_val;
        }

        return 0;

    }
    
};
