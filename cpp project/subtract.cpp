// subtract.cpp
#include "calculation.h"

class Subtraction : public Calculation {
public:
    double calculate(double a, double b) override {
        return a - b;
    }
};
