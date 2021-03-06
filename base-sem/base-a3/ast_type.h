/*
 * In our AST, Type nodes are used to represent and store type
 * information. The base Type class is used for built-in types, the
 * NamedType for classes and interfaces, and the ArrayType for arrays
 * of other types.
 *
 * Assignment 3: You will need to extend the Type classes to implement
 * the type system and rules for type equivalency and compatibility.
 */

#ifndef _ast_type_h
#define _ast_type_h

#include "ast.h"
#include "list.h"
#include <iostream>
using namespace std;

class Type : public Node
{
  protected:
    char *typeName;

  public :
    static Type *intType, *doubleType, *boolType, *voidType,
                *nullType, *stringType, *errorType;

    Type(yyltype loc) : Node(loc) {}
    Type(const char *str);

    virtual bool IsEquivalentTo(Type *other) { return this == other; }

    const char *GetPrintNameForNode() { return "Type"; }
    void PrintChildren(int indentLevel);
    virtual const char* GetTypeName() { return typeName; }
    virtual void PrintToStream(ostream& out) { out << typeName; }
    friend ostream& operator<<(ostream& out, Type *t) { t->PrintToStream(out); return out; }
};

class NamedType : public Type
{
  protected:
    Identifier *id;

  public:
    NamedType(Identifier *i);

    const char *GetPrintNameForNode() { return "NamedType"; }
    const char* GetTypeName() { return id->GetName(); }
    virtual Identifier* GetID() { return id; }
    void PrintChildren(int indentLevel);
    void PrintToStream(ostream& out) { out << id; }
};

class ArrayType : public Type
{
  protected:
    Type *elemType;

  public:
    ArrayType(yyltype loc, Type *elemType);

    const char *GetPrintNameForNode() { return "ArrayType"; }
    void PrintChildren(int indentLevel);
    const char* GetTypeName() { return elemType->GetTypeName(); }
    void PrintToStream(ostream& out) { out << elemType << "[]"; }
};


#endif
