#pragma once

// covered range : x1 <= x < x2  (exclude x2)
struct LineSegmentSet1D {
    set<tuple<int, int, int>> lines;    // (x2, x1, delta), no overlap
    int covered;                        // covered length

    LineSegmentSet1D() : covered(0) {
    }

    int getCovered() const {
        return covered;
    }

    int add(int x1, int x2) {
        return add(x1, x2, 1);
    }

    int remove(int x1, int x2) {
        return add(x1, x2, -1);
    }

    // x1 <= x < x2, d is -1 or 1
    // worst case O(NlogN), but normally O(logN)
    int add(int x1, int x2, int d) {
        auto it = lines.lower_bound(make_tuple(x1, x1, 0));
        while (it != lines.end()) {
            int y2, y1, yd;
            tie(y2, y1, yd) = *it;

            if (x2 <= y1)
                break;

            auto curr = it++;
            lines.erase(curr);
            covered -= y2 - y1;

            // ****----+
            //     +-------+
            if (x1 != y1) {
                int l = min(x1, y1);
                int r = max(x1, y1);
                lines.emplace(r, l, x1 < y1 ? d : yd);
                covered += r - l;
                x1 = y1 = r;
            }

            // +---*****
            //     *****---+
            if (yd + d > 0) {
                lines.emplace(min(x2, y2), x1, yd + d);
                covered += min(x2, y2) - x1;
            }
            x1 = y1 = min(x2, y2);

            // +-------+
            //     +----****
            if (y1 < y2) {
                lines.emplace(y2, y1, yd);
                covered += y2 - y1;
                break;
            }
        }
        // +-------+
        //     +----****
        if (x1 < x2) {
            lines.emplace(x2, x1, d);
            covered += x2 - x1;
        }

        return covered;
    }
};
