// multiply.cpp
#include "calculation.h"

class Multiplication : public Calculation {
public:
    double calculate(double a, double b) override {
        return a * b;
    }
};
