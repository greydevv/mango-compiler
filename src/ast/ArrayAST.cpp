#include <vector>
#include "ArrayAST.h"
#include "llvm/IR/Value.h"

ArrayAST::ArrayAST(Type type, std::vector<std::unique_ptr<AST>> elements)
    : type(type), elements(std::move(elements)) {}

ArrayAST::ArrayAST(Type type)
    : type(type), elements() {}

ArrayAST::ArrayAST(const ArrayAST& other)
    : type(other.type)
{
    for (auto& ele : other.elements)
    {
        addElement(std::unique_ptr<AST>(ele->clone()));
    }
}

void ArrayAST::addElement(std::unique_ptr<AST> ele)
{
    elements.push_back(std::move(ele));
}

llvm::Value* ArrayAST::accept(ASTCodegenner& cg) 
{
    return cg.codegen(this);
}

std::string ArrayAST::accept(ASTStringifier& sf, int tabs) 
{
    return sf.toString(this, tabs);
}

ArrayAST* ArrayAST::cloneImpl()
{
    return new ArrayAST(*this);
}
