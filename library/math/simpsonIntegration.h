#pragma once

// Simpson's integration
struct SimpsonIntegration {
    // inclusive
    static double integrate(function<double(double)> f, double a, double b) {
        double EPS = 1e-10;
        double m = (a + b) / 2;
        double am = simpsonIntegration(f, a, m);
        double mb = simpsonIntegration(f, m, b);
        double ab = simpsonIntegration(f, a, b);
        if (abs(am + mb - ab) < EPS)
            return ab;
        return integrate(f, a, m) + integrate(f, m, b);
    }
    
private:
    static double simpsonIntegration(function<double(double)> f, double a, double b) {
        return (f(a) + 4 * f((a + b) / 2) + f(b)) * (b - a) / 6.0;
    }
};