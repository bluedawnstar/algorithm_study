#pragma once

template <typename T, typename U>
struct ConvexFunctions {
    //--- Point to Line

    // >  0 : distance from point (x, y) to line (p1, p2), left side of vector p2 - p1
    // == 0 : on line (p1, p2)
    // <  0 : -distance from point (x, y) to line (p1, p2), right side of vector p2 - p1
    static double distancePointToLine(const pair<T,T>& p1, const pair<T,T>& p2, U x, U y) {
        return ((y - p1.second) * (p2.first - p1.first) - (x - p1.first) * (p2.second - p1.second))
               / hypot(p2.first - p1.first, p2.second - p1.second);
    }


    //--- Point to Polygons

    // - max distance from point to each polygon edges, convex function with global minimum
    // - return value:
    //     <  0 : inside a polygon
    //     == 0 : boundary of a polygon
    //     >  0 : outside a polygon
    static double evalPointToPolygonMin(const vector<pair<T,T>>& poly, U x, U y) {
        double res = -distancePointToLine(poly.back(), poly[0], x, y);
        for (int i = 1; i < int(poly.size()); i++)
            res = max(res, -distancePointToLine(poly[i - 1], poly[i], x, y));
        return res;
    }

    // - max distance from point to each polygon edges, convex function with global maximum
    // - return value:
    //     >  0 : inside a polygon
    //     == 0 : boundary of a polygon
    //     <  0 : outside a polygon
    static double evalPointToPolygonMax(const vector<pair<T,T>>& poly, U x, U y) {
        double res = distancePointToLine(poly.back(), poly[0], x, y);
        for (int i = 1; i < int(poly.size()); i++)
            res = min(res, distancePointToLine(poly[i - 1], poly[i], x, y));
        return res;
    }


    // - max distance from point to each polygon edges, convex function with global minimum
    // - return value:
    //     <= 0 : inside intersection of polygons
    //     >  0 : outside intersection of polygons
    static double evalPointToPolygonsMin(const vector<vector<pair<T,T>>>& polyList, U x, U y) {
        double res = evalPointToPolygonMin(polyList[0], x, y);
        for (int i = 1; i < int(polyList.size()); i++)
            res = max(res, evalPointToPolygonMin(polyList[i], x, y));
        return res;
    }

    // - max distance from point to each polygon edges, convex function with global maximum
    // - return value:
    //     >= 0 : inside intersection of polygons
    //     <  0 : outside intersection of polygons
    static double evalPointToPolygonsMax(const vector<vector<pair<T,T>>>& polyList, U x, U y) {
        double res = evalPointToPolygonMax(polyList[0], x, y);
        for (int i = 1; i < int(polyList.size()); i++)
            res = min(res, evalPointToPolygonMax(polyList[i], x, y));
        return res;
    }


    //--- Ellipses

    // - convex function with global minimum
    // - return value:
    //     <  0 : inside an ellipse
    //     == 0 : boundary of an ellipse
    //     >  0 : outside an ellipse
    static double evalPointToEllipseMin(const pair<T,T>& focal1, const pair<T,T>& focal2, T semiMajorAxis, U x, U y) {
        return hypot(x - focal1.first, y - focal1.second) + hypot(x - focal2.first, y - focal2.second) - 2 * semiMajorAxis;
    }

    // - convex function with global maximum
    // - return value:
    //     >  0 : inside an ellipse
    //     == 0 : boundary of an ellipse
    //     <  0 : outside an ellipse
    static double evalPointToEllipseMax(const pair<T,T>& focal1, const pair<T,T>& focal2, T semiMajorAxis, U x, U y) {
        return 2 * semiMajorAxis - hypot(x - focal1.first, y - focal1.second) - hypot(x - focal2.first, y - focal2.second);
    }


    // - convex function with global minimum
    // - return value:
    //     <= 0 : inside intersection of ellipses
    //     >  0 : outside intersection of ellipses
    static double evalPointToEllipsesMin(const vector<tuple<pair<T,T>, pair<T,T>, T>>& ellipses, U x, U y) {
        double res = evalPointToEllipseMin(get<0>(ellipses[0]), get<1>(ellipses[0]), get<2>(ellipses[0]), x, y);
        for (int i = 1; i < int(ellipses.size()); i++)
            res = max(res, evalPointToEllipseMin(get<0>(ellipses[i]), get<1>(ellipses[i]), get<2>(ellipses[i]), x, y));
        return res;
    }

    // - convex function with global maximum
    // - return value:
    //     >= 0 : inside intersection of ellipses
    //     <  0 : outside intersection of ellipses
    static double evalPointToEllipsesMax(const vector<tuple<pair<T,T>, pair<T,T>, T>>& ellipses, U x, U y) {
        double res = evalPointToEllipseMax(get<0>(ellipses[0]), get<1>(ellipses[0]), get<2>(ellipses[0]), x, y);
        for (int i = 1; i < int(ellipses.size()); i++)
            res = min(res, evalPointToEllipseMax(get<0>(ellipses[i]), get<1>(ellipses[i]), get<2>(ellipses[i]), x, y));
        return res;
    }
};
