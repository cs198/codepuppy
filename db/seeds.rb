# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rake db:seed (or created alongside the db with db:setup).
#
# Examples:
#
#   cities = City.create([{ name: 'Chicago' }, { name: 'Copenhagen' }])
#   Mayor.create(name: 'Emanuel', city: cities.first)


Course.delete_all
Course.create(course_dept:'CS', course_code:'106A',
              course_name: 'Programming Methodology',
              term_name:'Fall', period:'13-14', active:false)
Course.create(course_dept:'CS', course_code:'106B',
              course_name: 'Programming Abstractions',
              term_name:'Fall', period:'13-14', active:false)

cs106a = Course.create(course_dept:'CS', course_code:'106A',
              course_name: 'Programming Methodology',
              term_name:'Winter', period:'13-14', active:true)
cs106b = Course.create(course_dept:'CS', course_code:'106B',
              course_name: 'Programming Abstractions',
              term_name:'Winter', period:'13-14', active:true)


Person.delete_all
amainero = Person.create(user_system_id:'amainero', given_name:'Anthony', family_name:'Mainero')
maesenc = Person.create(user_system_id:'maesenc', given_name:'Maesen', family_name:'Churchill')
odiab = Person.create(user_system_id:'odiab', given_name:'Omar Sebastian', family_name:'Diab')
rawatson = Person.create(user_system_id:'rawatson', given_name:'Reid', family_name:'Watson')

CoursesPeople.delete_all
CoursesPeople.create(course:cs106b, person:rawatson, role:'admin')
CoursesPeople.create(course:cs106b, person:maesenc, role:'leader')
CoursesPeople.create(course:cs106b, person:odiab, role:'student')
CoursesPeople.create(course:cs106b, person:amainero, role:'student')

Section.delete_all
section = Section.create(course:cs106b)

SectionsPeople.delete_all
SectionsPeople.create(section:section, person:maesenc, role:'leader')
SectionsPeople.create(section:section, person:odiab, role:'student')
SectionsPeople.create(section:section, person:amainero, role:'student')

assignment_1 = Assignment.create(course:cs106b, number:1, name:'Game of Life', 
                                 date_assigned:'2014-01-10', date_due:'2014-01-17 14:00:00')
assignment_2 = Assignment.create(course:cs106b, number:2, name:'Word Ladder / NGrams', 
                                 date_assigned:'2014-01-17', date_due:'2014-01-27 14:00:00')
assignment_3 = Assignment.create(course:cs106b, number:3, name:'Recursion Problems',
                                 date_assigned:'2014-01-27', date_due:'2014-02-05 14:00:00')
assignment_4 = Assignment.create(course:cs106b, number:4, name:'Boggle',
                                 date_assigned:'2014-02-05', date_due:'2014-02-14 14:00:00')
assignment_5 = Assignment.create(course:cs106b, number:5, name:'Priority Queue',
                                 date_assigned:'2014-02-14', date_due:'2014-02-24 14:00:00')
assignment_6 = Assignment.create(course:cs106b, number:6, name:'Huffman',
                                 date_assigned:'2014-02-24', date_due:'2014-03-05 14:00:00')
assignment_7 = Assignment.create(course:cs106b, number:7, name:'Huffman',
                                 date_assigned:'2014-03-05', date_due:'2014-03-17 14:00:00')

anthony_submission = Submission.create(person:amainero, assignment:assignment_1, 
                                       date_submitted: '2014-01-12', feedback_released:false)
omar_submission = Submission.create(person:odiab, assignment:assignment_1, 
                                    date_submitted: '2014-01-13', feedback_released:true)


SubmissionFile.create(submission:anthony_submission, original_filename:'lambda.cpp', data:'/**
 * File: lambda.cpp
 * ----------------
 */

#include "genlib.h"
#include "lambda.h"
#include "symbol.h"
#include "strutils.h"

LambdaExpression::LambdaExpression(Vector<SymbolExpression *>& parameters,
                                   Expression *parametrizedExpression,
                                   EvalState& stateAtEvalTime) {
    param = parameters;
    funct = parametrizedExpression;
    initState = new EvalState(stateAtEvalTime);
}

/**
 * Rather than return the full stringification of the full lambda,
 * we elect to return an abbreviated string that just makes it clear
 * the expression is some sort of a lambda.
 */

Expression *LambdaExpression::call(Vector<Expression *>& arguments,
                                   EvalState& state) {
    if(arguments.size() != param.size()){
        Error("Incorrect number of arguments in the lambda expression.");
    }

    EvalState localState(state);

    for(int i = 0; i < arguments.size(); i++){
        Expression *evalArg = arguments[i]->eval(localState);
        localState.setExpression(param[i]->toString(), evalArg);
    }

    return funct->eval(localState);
}') 

SubmissionFile.create(submission:anthony_submission, original_filename:'specialforms.cpp', data:'/**
 * File: specialforms.cpp
 * ----------------------
 */

#include "specialforms.h"
#include "expression.h"
#include "lst.h"
#include "symbol.h"
#include "boolean.h"
#include "set.h"
#include "lambda.h"
#include "rational.h"
#include "str.h"
#include "scanner.h"
#include <iostream>

Expression *DefineSpecialForm::call(Vector<Expression *>& arguments,
                                    EvalState& state) {
    if (arguments.size() != 2) {
        Error("define expects exactly two arguments");
    }

    SymbolExpression *symbol = dynamic_cast<SymbolExpression *>(arguments[0]);
    if (symbol == NULL) {
        Error("The first argument to define by be a symbol");
    }
    Expression *value = arguments[1];

    state.setExpression(symbol->toString(),
                        value->eval(state));
    return symbol;
}

Expression *QuitSpecialForm::call(Vector<Expression *>& arguments,
                                  EvalState& state) {
    cout << endl;
    cout << "[Press <return> to close]";
    string dummy;
    getline(cin, dummy);
    exit(0);
    return this; // hush the compiler
}

Expression *CarSpecialForm::call(Vector<Expression *>& arguments,
                                 EvalState& state) {
    if (arguments.size() != 1) {
        Error("car expects exactly one argument");
    }

    evaluateAllArguments(arguments, state);
    ListExpression *listArgument =
        dynamic_cast<ListExpression *>(arguments[0]);
    if (listArgument == NULL) {
        Error("car expects its one argument to be a list");
    }

    return listArgument->car();
}

Expression *CdrSpecialForm::call(Vector<Expression *>& arguments,
                                 EvalState& state) {
    if (arguments.size() != 1) {
        Error("cdr expects exactly one argument.");
    }

    evaluateAllArguments(arguments, state);
    ListExpression *listArgument =
        dynamic_cast<ListExpression *>(arguments[0]);
    if (listArgument == NULL) {
        Error("cdr expects its one argument to be a list.");
    }

    return listArgument->cdr();
}

Expression *ConsSpecialForm::call(Vector<Expression *>& arguments,
                                  EvalState& state) {
    if (arguments.size() != 2) {
        Error("cons expects precisely two arguments.");
    }

    evaluateAllArguments(arguments, state);
    Expression *newCar = arguments[0];
    ListExpression *newCdr = dynamic_cast<ListExpression *>(arguments[1]);
    if (newCdr == NULL) {
        Error("Second argument passed to cons must be a list.");
    }

    return new ListExpression(newCar, newCdr);
}

Expression *ListSpecialForm::call(Vector<Expression *>& arguments,
                                  EvalState& state) {
    evaluateAllArguments(arguments, state);
    return new ListExpression(arguments);
}

Expression *IfSpecialForm::call(Vector<Expression *>& arguments,
                                EvalState& state) {
    if (arguments.size() != 3) {
        Error("if expressions expect precisely three arguments.");
    }

    if (arguments[0]->eval(state)->isTrue())
        return arguments[1]->eval(state);
    else
        return arguments[2]->eval(state);
}

Expression *NullSpecialForm::call(Vector<Expression *>& arguments,
                                  EvalState& state)
{
    if (arguments.size() != 1) {
        Error("null expressions take just one argument.");
    }

    evaluateAllArguments(arguments, state);
    ListExpression *list =
        dynamic_cast<ListExpression *>(arguments[0]);
    if (list == NULL) {
        Error("null expects a list and only a list");
    }

    return new BooleanExpression(list->isEmpty());
}

Expression *AddSpecialForm::call(Vector<Expression *>& arguments,
                                EvalState& state)
{
    evaluateAllArguments(arguments, state);
    RationalExpression *sum = new RationalExpression(0);
    for (int i = 0; i < arguments.size(); i++) {
        RationalExpression *addend =
            dynamic_cast<RationalExpression *>(arguments[i]);
        if (addend == NULL) {
            Error("add expects all arguments to be rational numbers.");
        }
        sum = sum->add(addend);
    }
    return sum;
}

Expression *SubtractSpecialForm::call(Vector<Expression *>& arguments,
                                      EvalState& state)
{
    if (arguments.size() != 2) {
        Error("subtract takes precisely two arguments.");
    }
    evaluateAllArguments(arguments, state);
    RationalExpression *minuend =
        dynamic_cast<RationalExpression *>(arguments[0]);
    RationalExpression *subtrahend =
        dynamic_cast<RationalExpression *>(arguments[1]);
    if (minuend == NULL || subtrahend == NULL) {
        Error("subtract only takes rational numbers as arguments.");
    }

    return minuend->subtract(subtrahend);
}

Expression *MultiplySpecialForm::call(Vector<Expression *>& arguments,
                                      EvalState& state)
{
    evaluateAllArguments(arguments, state);
    RationalExpression *product = new RationalExpression(1);
    for (int i = 0; i < arguments.size(); i++) {
        RationalExpression *factor =
            dynamic_cast<RationalExpression *>(arguments[i]);
        if (factor == NULL) {
            Error("multiply expects all arguments to be rational numbers.");
        }
        product = product->multiply(factor);
    }
    return product;
} // enough code for now!
  ')
