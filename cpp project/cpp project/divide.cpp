// divide.cpp
#include "calculation.h"

class Division : public Calculation {
public:
    double calculate(double a, double b) override {
        if (b != 0) {
            return a / b;
        } else {
            // Handle division by zero
            return 0;
        }
    }
};
