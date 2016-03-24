/*
 * Implementation of statement node classes.
 */
#include "ast_stmt.h"
#include "ast_type.h"
#include "ast_decl.h"
#include "ast_expr.h"


SymbolTable::SymbolTable(){
    this->symbolTable = new Hashtable<Node*>;
}

Hashtable<Node*>* SymbolTable::getHashTablePointer(){
    return this->symbolTable;
}

void SymbolTable::AddDecl(Decl* newEntry, bool overwrite){
    symbolTable->Enter(newEntry->GetDeclName(), newEntry, overwrite);
}

Program::Program(List<Decl*> *d) {
    Assert(d != NULL);
    (decls=d)->SetParentAll(this);
}

void Program::Check() {
    /* Assignment 3
     * ============
     * Here is where the semantic analyzer is kicked off.  The general
     * idea is to perform a tree traversal of the entire program,
     * examining all constructs for compliance with the semantic
     * rules.  Each node can have its own way of checking itself,
     * which makes for a great use of inheritance and polymorphism in
     * the node classes.
     */
     SymbolTable* symbolT = new SymbolTable();
     if(needsSymbolTable() == true){
        this->globalSymbolTable = symbolT;
     }
     //Build the symbol table
     BuildScope();
     //Check everything
     for (int i = 0; i < decls->NumElements(); i++){
        decls->Nth(i)->Check();
     }
}

void Program::BuildScope(){
    this->globalSymbolTable->SetParentTable(NULL);
    for (int i = 0; i < decls->NumElements(); i++){
        Node* n = this->globalSymbolTable->getHashTablePointer()->Lookup( decls->Nth(i)->GetDeclName() );
        bool overwrite = false;
        if(n != NULL){
            //Throw error and return
            return;
        }
        this->globalSymbolTable->AddDecl(decls->Nth(i), overwrite);
    }

    for (int i = 0; i < decls->NumElements(); i++){
        decls->Nth(i)->BuildScope(this->globalSymbolTable);
    }
}

void Program::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    printf("\n");
}

StmtBlock::StmtBlock(List<VarDecl*> *d, List<Stmt*> *s) {
    Assert(d != NULL && s != NULL);
    (decls=d)->SetParentAll(this);
    (stmts=s)->SetParentAll(this);
}

void StmtBlock::PrintChildren(int indentLevel) {
    decls->PrintAll(indentLevel+1);
    stmts->PrintAll(indentLevel+1);
}

ConditionalStmt::ConditionalStmt(Expr *t, Stmt *b) {
    Assert(t != NULL && b != NULL);
    (test=t)->SetParent(this);
    (body=b)->SetParent(this);
}

ForStmt::ForStmt(Expr *i, Expr *t, Expr *s, Stmt *b): LoopStmt(t, b) {
    Assert(i != NULL && t != NULL && s != NULL && b != NULL);
    (init=i)->SetParent(this);
    (step=s)->SetParent(this);
}

void ForStmt::PrintChildren(int indentLevel) {
    init->Print(indentLevel+1, "(init) ");
    test->Print(indentLevel+1, "(test) ");
    step->Print(indentLevel+1, "(step) ");
    body->Print(indentLevel+1, "(body) ");
}

void WhileStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(body) ");
}

IfStmt::IfStmt(Expr *t, Stmt *tb, Stmt *eb): ConditionalStmt(t, tb) {
    Assert(t != NULL && tb != NULL); // else can be NULL
    elseBody = eb;
    if (elseBody) elseBody->SetParent(this);
}

void IfStmt::PrintChildren(int indentLevel) {
    test->Print(indentLevel+1, "(test) ");
    body->Print(indentLevel+1, "(then) ");
    if (elseBody) elseBody->Print(indentLevel+1, "(else) ");
}


ReturnStmt::ReturnStmt(yyltype loc, Expr *e) : Stmt(loc) {
    Assert(e != NULL);
    (expr=e)->SetParent(this);
}

void ReturnStmt::PrintChildren(int indentLevel) {
    expr->Print(indentLevel+1);
}

PrintStmt::PrintStmt(List<Expr*> *a) {
    Assert(a != NULL);
    (args=a)->SetParentAll(this);
}

void PrintStmt::PrintChildren(int indentLevel) {
    args->PrintAll(indentLevel+1, "(args) ");
}

SwitchLabel::SwitchLabel(IntConstant *l, List<Stmt*> *s) {
    Assert(l != NULL && s != NULL);
    (label=l)->SetParent(this);
    (stmts=s)->SetParentAll(this);
}

SwitchLabel::SwitchLabel(List<Stmt*> *s) {
    Assert(s != NULL);
    label = NULL;
    (stmts=s)->SetParentAll(this);
}

void SwitchLabel::PrintChildren(int indentLevel) {
    if (label) label->Print(indentLevel+1);
    stmts->PrintAll(indentLevel+1);
}

SwitchStmt::SwitchStmt(Expr *e, List<Case*> *c, Default *d) {
    Assert(e != NULL && c != NULL && c->NumElements() != 0 );
    (expr=e)->SetParent(this);
    (cases=c)->SetParentAll(this);
    def = d;
    if (def) def->SetParent(this);
}

void SwitchStmt::PrintChildren(int indentLevel) {
    expr->Print(indentLevel+1);
    cases->PrintAll(indentLevel+1);
    if (def) def->Print(indentLevel+1);
}