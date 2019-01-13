#pragma once

#include "vec2D.h"

// Line Sweep Algorithm
template <typename T>
struct LineSegmentIntersection {
    struct LineSegment {
        Vec2D<T> a, b;
        int id;

        LineSegment(Vec2D<T> _a, Vec2D<T> _b) : a(_a), b(_b) {
            if (a > b)
                swap(a, b);
        }

        bool operator <(const LineSegment& rhs) const {
            if (a.x < rhs.a.x) {
                auto s = cross(a, b, rhs.a);
                return (s > 0 || s == 0 && a.y < rhs.a.y);
            } else {
                auto s = cross(rhs.a, rhs.b, a);
                return (s < 0 || s == 0 && a.y < rhs.a.y);
            }
            return a.y < rhs.a.y;
        }

        bool isIntersected(const LineSegment& rhs) const {
            T x1 = a.x, y1 = a.y, x2 = b.x, y2 = b.y;
            T x3 = rhs.a.x, y3 = rhs.a.y, x4 = rhs.b.x, y4 = rhs.b.y;
            if (max(x1, x2) < min(x3, x4) || max(x3, x4) < min(x1, x2) || max(y1, y2) < min(y3, y4) || max(y3, y4) < min(y1, y2))
                return false;

            T z1 = (x3 - x1) * (y2 - y1) - (y3 - y1) * (x2 - x1);
            T z2 = (x4 - x1) * (y2 - y1) - (y4 - y1) * (x2 - x1);
            if (z1 < 0 && z2 < 0 || z1 > 0 && z2 > 0)
                return false;

            T z3 = (x1 - x3) * (y4 - y3) - (y1 - y3) * (x4 - x3);
            T z4 = (x2 - x3) * (y4 - y3) - (y2 - y3) * (x4 - x3);
            if (z3 < 0 && z4 < 0 || z3 > 0 && z4 > 0)
                return false;

            return true;
        }
    };

    struct Event {
        Vec2D<T> p;
        int type;

        int id;

        Event(Vec2D<T> _p, int _id, int _type) : p(_p), id(_id), type(_type) {
        }

        bool operator <(const Event& rhs) const {
            if (p.x != rhs.p.x)
                return p.x < rhs.p.x;
            else if (type != rhs.type)
                return type < rhs.type;
            else
                return p.y < rhs.p.y;
        }
    };

    // Shamos-Hoey algorithm : O(NlogN)
    static pair<int,int> findIntersection(vector<LineSegment>& v) {
        int n = int(v.size());
        vector<Event> e;
        e.reserve(n * 2);
        for (int i = 0; i < n; i++) {
            e.emplace_back(v[i].a, i, 0);
            e.emplace_back(v[i].b, i, 1);
            v[i].id = i;
        }
        sort(e.begin(), e.end());

        set<LineSegment> S;
        for (int i = 0; i < n * 2; i++) {
            int id = e[i].id;
            if (e[i].type == 0) {
                set<LineSegment>::iterator it = S.lower_bound(v[id]);
                if (it != S.end()) {
                    if (it->isIntersected(v[id]))
                        return make_pair(it->id, v[id].id);
                }
                if (it != S.begin()) {
                    --it;
                    if (it->isIntersected(v[id]))
                        return make_pair(it->id, v[id].id);
                }
                S.insert(v[id]);
            } else {
                set<LineSegment>::iterator it = S.lower_bound(v[id]);
                auto next = it;
                auto prev = it;
                if (it != S.begin() && it != --S.end()) {
                    ++next, --prev;
                    if (next->isIntersected(*prev))
                        return make_pair(next->id, prev->id);
                }

                S.erase(it);
            }
        }
        return make_pair(-1, -1);
    }
};
