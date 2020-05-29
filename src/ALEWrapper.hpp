//
// Created by seb on 29/05/2020.
//

#ifndef MASTER_NEATER_ALEWRAPPER_HPP
#define MASTER_NEATER_ALEWRAPPER_HPP

#include <vector>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

class ALEWrapper {
public:
    ALEWrapper();

    std::vector<float> const &getData();
    void sendAction(std::vector<float>);

public:
    FILE* fin;
    FILE* fout;

    int width;
    int height;

    char buffer[204800];
    std::vector<float> out;
};


#endif //MASTER_NEATER_ALEWRAPPER_HPP
