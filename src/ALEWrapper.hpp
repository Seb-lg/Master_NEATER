//
// Created by seb on 29/05/2020.
//

#ifndef MASTER_NEATER_ALEWRAPPER_HPP
#define MASTER_NEATER_ALEWRAPPER_HPP

#include <vector>
#include <sstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>

class ALEWrapper {
public:
    explicit ALEWrapper(bool graphical = false);
    ~ALEWrapper();

    std::vector<float> const &getData();
    void sendAction(std::vector<float>);
    bool isTerminal();

    static void killExcess();

public:
    int fin[2]{};
    int fout[2]{};

    int width{};
    int height{};

    char buffer[204800]{};
    std::vector<float> out;
    int id;
    bool terminal;
    int fitness;
};


#endif //MASTER_NEATER_ALEWRAPPER_HPP
