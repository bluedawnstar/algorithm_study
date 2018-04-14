#pragma once

// Heron's formula : https://en.wikipedia.org/wiki/Heron%27s_formula
double calcArea(double a, double b, double c) {
    double s = (a + b + c) / 2;
    return sqrt(s*(s - a)*(s - b)*(s - c));
}
