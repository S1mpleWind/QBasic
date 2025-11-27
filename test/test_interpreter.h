#pragma once

#include"../interpreter/interpreter.h"
#include"../core/program.h"
#include<iostream>

void testInterpreter(){
    Interpreter itp;

    // 设置回调
    // itp.setOutputConsumer([](const QString &s) {
    //     std::cout << s.toStdString() << std::endl;
    // });

    std::cout<<"program loading"<<std::endl;

    Program p;

    /*
        "10 LET X = 1",
        "20 PRINT X",
        "30 LET X = X + 1",
        "40 IF X < 5 THEN 20",
        "50 END"*/

    // parse the prag by hand
    p.addSourceLine(10, "LET X = 1");
    p.addSourceLine(20, "PRINT X");
    p.addSourceLine(30, "END");
    // p.addSourceLine(30, "LET X = X + 1");
    // p.addSourceLine(40, "IF X < 5 THEN 20");
    // p.addSourceLine(50, "END");

    std::cout<<"successfully loaded"<<std::endl;

    //std::cout<<p.getDisplayText()<<std::endl;

    // 10 LET X = 1
    p.setParsedStatement(
        10,
        new LetStmt("X", new ConstantExp(1))
        );

    // 20 PRINT X
    p.setParsedStatement(
        20,
        new PrintStmt(new IdentifierExp("X"))
        );

    p.setParsedStatement(
        30,
        new EndStmt()
        );



    itp.run(p);

    std::cout<<"running";





}
