#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>
#include <sstream>
#define unt unsigned int
using namespace std;

class Node{
    string data;
    Node* left;
    Node* right;

public:
    explicit Node(string &data) : data(data), left(nullptr), right(nullptr) {}

    friend class SolveTree;

    void killSelf() {
        if(right!=nullptr) right->killSelf();
        if(left!=nullptr) left->killSelf();
        delete this;
    }
    virtual ~Node() = default;
};

class SolveTree{
private:
    Node* root;

    bool ree = false;
    string signos[500];
    unt SignosCont = 0;
    vector<char> numeros = {'1','2','3','4','5','6','7','8','9','0'};
    vector<char> operadores = {'(',')','^','*','/','+','-'};
    stack<string> ordenS;
    vector<string> ordenV;
    vector<Node*> nodos;
    stack<Node*> nodosS;

    unt operatorSize(char x){
        if(x=='^') return 3;
        else if(x=='*') return 2;
        else if(x=='/') return 2;
        else if(x=='+') return 1;
        else if(x=='-') return 1;
        else if(x=='(') return 0;
        return 0;
    }

    void checkStack(char x){
        string y;
        y.push_back(x);
        while(operatorSize(x)<=operatorSize(ordenS.top()[0])){
            ordenV.push_back(ordenS.top());
            ordenS.pop();
            if(ordenS.empty()) break;
        }
        ordenS.push(y);
    }

    bool MoreThanTop(char x){
        if(x==')') return false;
        else if(x=='(') return true;
        if(!ordenS.empty()) return operatorSize(x) > operatorSize(ordenS.top()[0]);
        return true;
    }

    void GetInStack(string x){
        if(x[0] == ')'){
            while(ordenS.top()[0] != '('){
                ordenV.push_back(ordenS.top());
                ordenS.pop();
                if(ordenS.empty()) break;
            }
            if(!ordenS.empty()) ordenS.pop();
            return;
        }
        if(MoreThanTop(x[0])){
            ordenS.push(x);
            return;
        }
        else{
            checkStack(x[0]);
            return;
        }
    }

    bool CheckForNum(char x){
        for(auto c : numeros){
            if( x == c){
                signos[SignosCont].push_back(x);
                ree = true;
                return true;
            }
        }
        if (ree) ++SignosCont;
        return false;
    }

    bool esOperador(string x){
        for(auto d : operadores){
            if(x[0] == d){
                return true;
            }
        }
        return false;
    }
    bool esNumero(string x){
        for(auto d : numeros){
            if(x[0] == d or x[1]==d){
                return true;
            }
        }
        return false;
    }

    float Num(string const& numbero){
        if(esNumero(numbero)){
            float y;
            stringstream yes(numbero);
            yes >> y;
            return y;
        }
        else{
            float x;
            cout<<"Ingrese valor para la variable \""<<numbero<<"\": ";
            cin>>x;
            return x;
        }
    }

    string toString(float numbero){
        stringstream ss;
        ss << numbero;
        string str = ss.str();
        return str;
    }

    string operate(Node* operador){
        switch(operador->data[0]){
            case '+':
                return toString((Num(operador->left->data)) + (Num(operador->right->data)));
            case '-':
                return toString((Num(operador->left->data)) - (Num(operador->right->data)));
            case '*':
                return toString((Num(operador->left->data)) * (Num(operador->right->data)));
            case '/':
                return toString((Num(operador->left->data)) / (Num(operador->right->data)));
            case '^':
                return toString(pow(Num(operador->left->data),Num(operador->right->data)));
        }
        return(":3");
    }

    void solve(Node* temp){
        if(esOperador(temp->left->data)) solve(temp->left);
        if(esOperador(temp->right->data)) solve(temp->right);
        temp->data = operate(temp);
    }

public:
    explicit SolveTree() : root(nullptr) {}

    void solveFormula(){
        string formula;
        getline(cin, formula);
        formula.erase(remove(formula.begin(), formula.end(),' '), formula.end());
        bool numero;
        for(auto c : formula){
            numero = CheckForNum(c);
            if(!numero){
                signos[SignosCont].push_back(c);
                ree = false;
                ++SignosCont;
            }
        }
        for(unt i = 0; i<=SignosCont; i++){
            numero = false;
            for(auto j : operadores){
                if(signos[i][0] == j){
                    GetInStack(signos[i]);
                    numero = true;
                }
            }
            if(!numero){
                ordenV.push_back(signos[i]);
            }
        }

        while(!ordenS.empty()){
            ordenV.push_back(ordenS.top());
            ordenS.pop();
        }

        for(auto c : ordenV){
            auto temp = new Node(c);
            nodos.push_back(temp);
        }

        for(auto c : nodos){
            if(esOperador(c->data)){
                if(!nodosS.empty()){
                    c->right = nodosS.top();
                    nodosS.pop();
                    c->left = nodosS.top();
                    nodosS.pop();
                    nodosS.push(c);
                }
            }
            else if(c->data != " " and !c->data.empty()){
                nodosS.push(c);
            }
        }
        root = nodosS.top();
        nodosS.pop();

        solve(root);

        cout<<root->data<<'\n';
    }

    virtual ~SolveTree() {
        root->killSelf();
    }
};

int main() {
    auto tree = new SolveTree();
    tree->solveFormula();
    return EXIT_SUCCESS;
}
