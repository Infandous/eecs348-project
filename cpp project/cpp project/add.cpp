// add.cpp
#include "calculation.h"

class Addition : public Calculation {
public:
    double calculate(double a, double b) override {
        return a + b;
    }
};
