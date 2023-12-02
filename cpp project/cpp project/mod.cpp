// mod.cpp
#include "calculation.h"

class Modulus : public Calculation {
public:
    double calculate(double a, double b) override {
        if (b != 0) {
            return static_cast<int>(a) % static_cast<int>(b);
        } else {
            // Handle modulus by zero
            return 0;
        }
    }
};
