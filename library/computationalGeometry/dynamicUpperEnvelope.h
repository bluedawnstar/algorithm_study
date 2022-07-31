#pragma once

// Line-based dynamic convex hull trick
// Upper envelope for maximum - it looks like lower convex hull 
struct DynamicUpperEnvelope {
    static const long long INF = 0x3f3f3f3f3f3f3f3fll;

    struct Line {
        long long m, b;
        mutable function<const Line*()> succ;

        bool operator <(const Line& rhs) const {
            return m < rhs.m;
        }

        long long get(long long x) const {
            return m * x + b;
        }
    };

    multiset<Line> lines;
    
    void add(long long m, long long b) {
        auto y = lines.insert({ m, b });
        y->succ = [this,y]() {
            return next(y) == lines.end() ? nullptr : &*next(y);
        };

        if (bad(y)) {
            lines.erase(y);
            return;
        }

        while (next(y) != lines.end() && bad(next(y)))
            lines.erase(next(y));

        while (y != lines.begin() && bad(prev(y)))
            lines.erase(prev(y));
    }

    long long query(long long x) {
        auto val = Line{ x, -INF };
        auto l = *lower_bound(lines.begin(), lines.end(), val, [](const Line& l, const Line& r) {
            const Line* s = l.succ();
            if (!s)
                return false;

            long long x = r.m;
            return l.b - s->b < (s->m - l.m) * x;
        });
        return l.get(x);
    }

private:
    bool bad(multiset<Line>::iterator y) {
        auto z = next(y);
        if (y == lines.begin()) {
            if (z == lines.end())
                return false;
            return y->m == z->m && y->b <= z->b;
        }
        
        auto x = prev(y);
        if (z == lines.end())
            return y->m == x->m && y->b <= x->b;

        // if X of intersection point between (x, y) >= X of intersection point between (x, z), then y is bad
        return (x->b - y->b) * (z->m - y->m) >= (y->b - z->b) * (y->m - x->m);
    }
};
