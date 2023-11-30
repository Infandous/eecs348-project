// exp.cpp
#include "calculation.h"
#include <math.h>

class Exp : public Calculation {
public:
    double calculate(double a, double b) override {
        return pow(a,b);
    }
};