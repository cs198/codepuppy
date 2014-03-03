-- Clear all rows from all tables.
-- WARNING: THIS LITERALLY DESTROYS ALL THE DATA IN ALL TABLES.
-- NEVER RUN ONCE THE SITE IS LIVE.

DELETE FROM comments;
DELETE FROM submissions;
DELETE FROM assignments;

DELETE FROM sections_people;
DELETE FROM sections;

DELETE FROM courses_people;
DELETE FROM courses;
DELETE FROM people;

ALTER SEQUENCE comments_id_seq RESTART;
ALTER SEQUENCE submissions_id_seq RESTART;
ALTER SEQUENCE assignments_id_seq RESTART;

ALTER SEQUENCE sections_people_id_seq RESTART;
ALTER SEQUENCE sections_id_seq RESTART;

ALTER SEQUENCE courses_people_id_seq RESTART;
ALTER SEQUENCE courses_id_seq RESTART;
ALTER SEQUENCE people_id_seq RESTART;

-- Create fake data.

INSERT INTO people (user_system_id, given_name, family_name) VALUES
('amainero', 'Anthony', 'Mainero'),
('maesenc', 'Maesen', 'Churchill'),
('odiab', 'Omar Sebastian', 'Diab'),
('rawatson', 'Reid', 'Watson');

INSERT INTO courses (course_dept, course_code, course_name, term_name, period, active) VALUES
('CS', '106A', 'Programming Methodology', 'Fall', '13-14', False),
('CS', '106B', 'Programming Abstractions', 'Fall', '13-14', False),
('CS', '106A', 'Programming Methodology', 'Winter', '13-14', True),
('CS', '106B', 'Programming Abstractions', 'Winter', '13-14', True);

-- For CS106A Winter 2014, Reid is admin, Maesen leader, Omar and Anthony students.

INSERT INTO courses_people (course_id, person_id, role) VALUES
(3, 4, 'admin'),
(3, 2, 'leader'),
(3, 1, 'student'),
(3, 3, 'student');

-- Create a new section for CS106A Winter 2014.

INSERT INTO sections (course_id) VALUES
(3);

-- Maesen teaches a section consisting of Omar and Anthony.

INSERT INTO sections_people (section_id, person_id, role) VALUES
(1, 2, 'leader'),
(1, 1, 'student'),
(1, 3, 'student');

-- Create a new assignment for CS106A Winter 2014.

INSERT INTO assignments (course_id, number, date_assigned, date_due) VALUES
(3, 1, '2014-02-06', '2014-03-14 11:59:59');

-- Create new submissions for this assignment.

INSERT INTO submissions (student_id, assignment_id, location,
                        date_submitted, feedback_released) VALUES
(1, 1, 'anthony_fake_submission', '2014-02-05 11:00:00', False),
(3, 1, 'omar_fake_submission', '2014-02-05 11:30:00', True);

INSERT INTO submission_files (submission_id, path, data) VALUES
(1, 'lambda.cpp', '/**
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
}'),
(1, 'cpp/specialforms.cpp', '/**
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
