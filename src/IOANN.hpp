//
// Created by seb on 28/05/2020.
//

#ifndef MASTER_NEATER_IOANN_HPP
#define MASTER_NEATER_IOANN_HPP

struct Input {
    Input()= default;
    Input(int input, float closeness): input(input), closeness(closeness) {}
    int input;
    float closeness;
};

struct Output {
    Output()= default;
    Output(int input, float closeness): input(input), closeness(closeness) {}
    int input;
    float closeness;
};

#endif //MASTER_NEATER_IOANN_HPP
