//
// Created by wdazhi on 2025/1/30.
//
#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include "httpserver.h"
int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "Usage server config " << std::endl;
        return 0;
    }
    httpserver httpserver(argv[1]);
    httpserver.start();
    return 0;
};

#endif