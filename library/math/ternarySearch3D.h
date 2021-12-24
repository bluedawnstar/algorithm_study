#pragma once

// return (argmax_y F(x, y), max F(x, y)) in [(x, y1), (x, y2)], inclusive
// - O(log|Y|*c), c = time complexity of F()
inline pair<double, double> ternarySearchMax2D(double x, double y1, double y2,
        const function<double(double, double)>& F, double epsilon = 1e-9) {
    while (abs(y2 - y1) > epsilon) {
        double midY1 = (2 * y1 + y2) / 3;
        double midY2 = (y1 + 2 * y2) / 3;
        double z1 = F(x, midY1);
        double z2 = F(x, midY2);
        if (z1 > z2)
            y2 = midY2;
        else
            y1 = midY1;
    }
    return make_pair((y1 + y2) / 2, F(x, (y1 + y2) / 2));
}

// return (argmax F(x, y), max F(x, y)) in [(x1, y1), (x2, y2)], inclusive
// - O(log|X|*log|Y|*c), c = time complexity of F()
inline pair<pair<double,double>,double> ternarySearchMax3D(double x1, double y1, double x2, double y2,
        const function<double(double, double)>& F, double epsilon = 1e-9) {
    while (abs(x2 - x1) > epsilon) {
        double midX1 = (2 * x1 + x2) / 3;
        double midX2 = (x1 + 2 * x2) / 3;
        double z1 = ternarySearchMax2D(midX1, y1, y2, F, epsilon).second;
        double z2 = ternarySearchMax2D(midX2, y1, y2, F, epsilon).second;
        if (z1 > z2)
            x2 = midX2;
        else
            x1 = midX1;
    }
    auto res = ternarySearchMax2D((x1 + x2) / 2, y1, y2, F, epsilon);
    return make_pair(make_pair((x1 + x2) / 2, res.first), res.second);
}
