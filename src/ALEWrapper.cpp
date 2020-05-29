//
// Created by seb on 29/05/2020.
//

#include <iostream>
#include <cstring>
#include "ALEWrapper.hpp"
#include "../include/Helper.hpp"

ALEWrapper::ALEWrapper() {
    fin = fopen(FIFO_IN, "r");
    fout = fopen(FIFO_OUT, "w");

    char oui[80];
    fgets(oui, 80, fin);

    std::istringstream data(oui);
    std::string test;
    char b;
    data >> height >> b >> width;
    out.resize(height * width, 0.0f);
    std::ostringstream dataout;
    dataout << true << ',' << false << ',' << false << ',' << true << "\n";
    std::string out = dataout.str();
    fputs(out.data(), fout);
    fflush(fout);
}

std::vector<float> const &ALEWrapper::getData() {
    memset(buffer, 0, sizeof(buffer));
    fgets(buffer, sizeof(buffer), fin);
    auto tmp = buffer;
    for (auto &elem : out) {
        tmp += 2;
        elem = (CtoI(tmp[0]) * 16.f + CtoI(tmp[1])) / 255.0f;
    }

    return out;
}

void ALEWrapper::sendAction(std::vector<float> actions) {
    int action = 0;
    float max = actions[0];
    for (int i = 0; i < actions.size(); ++i) {
        if (actions[i] > max) {
            max = actions[i];
            action = i;
        }
    }
    std::ostringstream outbuff;
    outbuff << action << ",18\n";
    fputs(outbuff.str().data(), fout);
    fflush(fout);
}
