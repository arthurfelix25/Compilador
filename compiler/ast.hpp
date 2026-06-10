#pragma once
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// ============ BASE NODE CLASS ============
struct Node {
    virtual ~Node() = default;
    virtual std::string toString() const = 0;
    virtual void print(int indent = 0) const {
        std::cout << std::string(indent * 2, ' ') << toString() << std::endl;
    }
};

// ============ LITERALS ============
struct NumLiteral : Node {
    double value;
    NumLiteral(double v) : value(v) {}
    
    std::string toString() const override {
        std::ostringstream oss;
        oss << "NumLiteral(" << value << ")";
        return oss.str();
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "NumLiteral: " << value << std::endl;
    }
};

struct StringLiteral : Node {
    std::string value;
    StringLiteral(std::string v) : value(std::move(v)) {}
    
    std::string toString() const override {
        return "StringLiteral(\"" + value + "\")";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "StringLiteral: \"" << value << "\"" << std::endl;
    }
};

struct BoolLiteral : Node {
    bool value;
    BoolLiteral(bool v) : value(v) {}
    
    std::string toString() const override {
        return value ? "BoolLiteral(true)" : "BoolLiteral(false)";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "BoolLiteral: " << (value ? "true" : "false") << std::endl;
    }
};

// ============ VARIABLES & REFERENCES ============
struct VarRef : Node {
    std::string name;
    VarRef(std::string n) : name(std::move(n)) {}
    
    std::string toString() const override {
        return "VarRef(" + name + ")";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "VarRef: " << name << std::endl;
    }
};

// ============ OPERATIONS ============
struct UnaryOp : Node {
    std::string op;
    std::unique_ptr<Node> operand;
    
    UnaryOp(std::string o, std::unique_ptr<Node> operand)
        : op(std::move(o)), operand(std::move(operand)) {}
    
    std::string toString() const override {
        return "UnaryOp(" + op + ")";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "UnaryOp: " << op << std::endl;
        operand->print(indent + 1);
    }
};

struct BinOp : Node {
    std::string op;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    
    BinOp(std::string o, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
    
    std::string toString() const override {
        return "BinOp(" + op + ")";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "BinOp: " << op << std::endl;
        std::cout << std::string((indent + 1) * 2, ' ') << "├─ Left:" << std::endl;
        left->print(indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "└─ Right:" << std::endl;
        right->print(indent + 2);
    }
};

// ============ STATEMENTS ============
struct Decl : Node {
    std::string typeName;
    std::string varName;
    std::unique_ptr<Node> expr;
    
    Decl(std::string t, std::string n, std::unique_ptr<Node> e)
        : typeName(std::move(t)), varName(std::move(n)), expr(std::move(e)) {}
    
    std::string toString() const override {
        return "Decl(" + typeName + " " + varName + ")";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "Decl: " << typeName << " " << varName << std::endl;
        if (expr) {
            std::cout << std::string((indent + 1) * 2, ' ') << "= " << std::endl;
            expr->print(indent + 1);
        }
    }
};

struct AssignStmt : Node {
    std::string varName;
    std::unique_ptr<Node> expr;
    
    AssignStmt(std::string n, std::unique_ptr<Node> e)
        : varName(std::move(n)), expr(std::move(e)) {}
    
    std::string toString() const override {
        return "AssignStmt(" + varName + ")";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "AssignStmt: " << varName << " =" << std::endl;
        expr->print(indent + 1);
    }
};

struct PrintStmt : Node {
    std::unique_ptr<Node> expr;
    
    PrintStmt(std::unique_ptr<Node> e) : expr(std::move(e)) {}
    
    std::string toString() const override {
        return "PrintStmt";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "PrintStmt:" << std::endl;
        expr->print(indent + 1);
    }
};

struct InputStmt : Node {
    std::string varName;
    
    InputStmt(std::string v) : varName(std::move(v)) {}
    
    std::string toString() const override {
        return "InputStmt(" + varName + ")";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "InputStmt: " << varName << std::endl;
    }
};

struct ExprStmt : Node {
    std::unique_ptr<Node> expr;
    
    ExprStmt(std::unique_ptr<Node> e) : expr(std::move(e)) {}
    
    std::string toString() const override {
        return "ExprStmt";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "ExprStmt:" << std::endl;
        expr->print(indent + 1);
    }
};

// ============ LOOPS & CONDITIONALS ============
struct WhileStmt : Node {
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> body;
    
    WhileStmt(std::unique_ptr<Node> c, std::unique_ptr<Node> b)
        : condition(std::move(c)), body(std::move(b)) {}
    
    std::string toString() const override {
        return "WhileStmt";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "WhileStmt:" << std::endl;
        std::cout << std::string((indent + 1) * 2, ' ') << "Condition:" << std::endl;
        condition->print(indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "Body:" << std::endl;
        body->print(indent + 2);
    }
};

struct ForStmt : Node {
    std::unique_ptr<Node> init;
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> increment;
    std::unique_ptr<Node> body;
    
    ForStmt(std::unique_ptr<Node> i, std::unique_ptr<Node> c,
            std::unique_ptr<Node> inc, std::unique_ptr<Node> b)
        : init(std::move(i)), condition(std::move(c)),
          increment(std::move(inc)), body(std::move(b)) {}
    
    std::string toString() const override {
        return "ForStmt";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "ForStmt:" << std::endl;
        std::cout << std::string((indent + 1) * 2, ' ') << "Init:" << std::endl;
        if (init) init->print(indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "Condition:" << std::endl;
        if (condition) condition->print(indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "Increment:" << std::endl;
        if (increment) increment->print(indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "Body:" << std::endl;
        body->print(indent + 2);
    }
};

struct IfStmt : Node {
    std::unique_ptr<Node> condition;
    std::unique_ptr<Node> thenBranch;
    std::unique_ptr<Node> elseBranch; // nullptr se não tiver else
    
    IfStmt(std::unique_ptr<Node> c, std::unique_ptr<Node> t, std::unique_ptr<Node> e = nullptr)
        : condition(std::move(c)), thenBranch(std::move(t)), elseBranch(std::move(e)) {}
    
    std::string toString() const override {
        return "IfStmt";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "IfStmt:" << std::endl;
        std::cout << std::string((indent + 1) * 2, ' ') << "Condition:" << std::endl;
        condition->print(indent + 2);
        std::cout << std::string((indent + 1) * 2, ' ') << "Then:" << std::endl;
        thenBranch->print(indent + 2);
        if (elseBranch) {
            std::cout << std::string((indent + 1) * 2, ' ') << "Else:" << std::endl;
            elseBranch->print(indent + 2);
        }
    }
};

// ============ BLOCKS ============
struct BlockStmt : Node {
    std::vector<std::unique_ptr<Node>> statements;
    
    BlockStmt(std::vector<std::unique_ptr<Node>> stmts)
        : statements(std::move(stmts)) {}
    
    std::string toString() const override {
        return "BlockStmt";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "BlockStmt {" << std::endl;
        for (const auto& stmt : statements) {
            stmt->print(indent + 1);
        }
        std::cout << std::string(indent * 2, ' ') << "}" << std::endl;
    }
};

// ============ PROGRAM ============
struct Program : Node {
    std::vector<std::unique_ptr<Node>> statements;
    
    Program(std::vector<std::unique_ptr<Node>> stmts)
        : statements(std::move(stmts)) {}
    
    std::string toString() const override {
        return "Program";
    }
    
    void print(int indent = 0) const override {
        std::cout << std::string(indent * 2, ' ') << "Program {" << std::endl;
        for (const auto& stmt : statements) {
            stmt->print(indent + 1);
        }
        std::cout << std::string(indent * 2, ' ') << "}" << std::endl;
    }
};

// ============ UTILITY FUNCTIONS ============
inline void printAST(const Node* node) {
    if (node) {
        node->print();
    }
}

inline void printAST(const std::unique_ptr<Node>& node) {
    if (node) {
        node->print();
    }
}
