/**
 * @file    evalstate.h
 * @brief   this file is responsible for evalstate at runtime
 *
 * @author  simple_wind
 * @version 1.0
 * @date    2025-11-27
 * */

#pragma once

#include<QObject>
/*
 * Class EvalState
 * ---------------------------------------------------
 * Stores all runtime variable bindings.
 * Supports get/set operations.
 */
class EvalState {
public:
    EvalState();
    ~EvalState();

    // Set the value of a variable
    void setValue(const std::string &var, int value);

    // Get the value of a variable
    int getValue(const std::string &var) const;

    // Check whether variable exists
    bool isDefined(const std::string &var) const;

    // Clear all variables
    void clear();

private:
    std::map<std::string, int> symbolTable;   // variable → value
};
