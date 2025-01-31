//
// Created by wdazhi on 2025/1/30.
//
#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include "httpserver.h"
int main()
{
    httpserver httpserver("127.0.0.1", 2001);
    httpserver.start();
    return 0;
};

#endif