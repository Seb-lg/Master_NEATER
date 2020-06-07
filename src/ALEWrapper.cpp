//
// Created by seb on 29/05/2020.
//

#include <iostream>
#include <cstring>
#include <csignal>
#include "ALEWrapper.hpp"
#include "../include/Helper.hpp"

ALEWrapper::ALEWrapper(bool graphical): terminal(false) {
    pipe(fin);
    pipe(fout);
    id = fork();

    ///PRO-TIP: [1] writing end, [0] reading end
    if (id != 0) {
        /// PARENT
        close(fin[1]);
        close(fout[0]);

        char rawData[80];
        read(fin[0], rawData, 80);
        std::istringstream data(rawData);
        char b;
        data >> height >> b >> width;
        out.resize(height * width, 0.0f);

        std::ostringstream dataout;
        ///        screen         ram             useless         fitness
        dataout << true << ',' << false << ',' << false << ',' << true << "\n";
        std::string out = dataout.str();
        write(fout[1], out.c_str(), out.length());
    } else {
        /// CHILD
        dup2(fin[1], 1);
        dup2(fout[0], 0);
        close(fin[0]);
        close(fout[1]);

        execvp(ALE, graphical ? ALE_Graphic : ALE_NonGraphic);
        close(fin[1]);
        close(fout[0]);
        exit(EXIT_SUCCESS);
    }
}

ALEWrapper::~ALEWrapper() {
    close(fin[0]);
    close(fout[1]);
    kill(id, SIGKILL);
}

std::vector<float> const &ALEWrapper::getData() {
    memset(buffer, 0, sizeof(buffer));
    auto tmp = buffer;
    bool loop = true;
    while (loop) {
        int length = read(fin[0], tmp, sizeof(buffer));
        for (int i = 0; i < length; ++i, ++tmp)
            if (*tmp == '\n')
                loop = false;
    }
    std::stringstream ss(buffer);
    std::string screen;
    std::string end;
    std::string tmpfitness;
    std::getline(ss, screen, ':');
    std::getline(ss, end, ',');
    std::getline(ss, tmpfitness, ':');
    tmp = screen.data();
    for (auto &elem : out) {
        elem = (CtoI(tmp[0]) * 16.f + CtoI(tmp[1])) / 255.0f;
        tmp += 2;
	}
	if (end[0] == '1')
    	terminal = true;
    fitness = std::stoi(tmpfitness);

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
    write(fout[1], outbuff.str().data(), outbuff.str().length());
}

bool ALEWrapper::isTerminal() {
    return terminal;
}

void ALEWrapper::killExcess() {
	char * const params [] = {"killall", "-s", "SIGKILL", "ale"};
	execvp("killall", params);
}
