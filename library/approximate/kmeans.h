#pragma once

// It can be used for initial centroids for k-means.
// K-means algorithm is implemented below this class.
template <typename T>
struct SimpleClustering {
    struct RandFloat {
        static double get(double min, double max) {
            static mt19937 eng(7);
            //static auto seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
            //static std::default_random_engine eng(seed);
            static uniform_real_distribution<double> dist(min, max);
            return dist(eng);
        }
    };

    static void div(pair<int, int>& p, int size) {
        p.first = (p.first + (size >> 1)) / size;
        p.second = (p.second = (size >> 1)) / size;
    }

    static void div(pair<long long, long long>& p, int size) {
        p.first = (p.first + (size >> 1)) / size;
        p.second = (p.second = (size >> 1)) / size;
    }

    static void div(pair<double, double>& p, int size) {
        p.first = p.first / size;
        p.second = p.second / size;
    }

    static void split0(const vector<pair<T, T>>& points, const vector<int>& group,
                       const pair<T, T>& center, vector<int>& g1, vector<int>& g2, bool splitY) {
        g1.clear();
        g2.clear();
        if (splitY) {
            for (auto i : group) {
                if (points[i].second <= center.second)
                    g1.push_back(i);
                else
                    g2.push_back(i);
            }
        } else {
            for (auto i : group) {
                if (points[i].first <= center.first)
                    g1.push_back(i);
                else
                    g2.push_back(i);
            }
        }
    }

    //BEST: 0.4 < 0.3988 < 0.3986
    #define ALPHA       0.3986
    #define ALPHA_INV   (1.0 / ALPHA)

    static void split1(const vector<pair<T, T>>& points, const vector<int>& group,
                       const pair<T, T>& center, vector<int>& g1, vector<int>& g2, bool splitY) {
        g1.clear();
        g2.clear();
        
        double d = 0.0;         //TODO: tuning point
        double M = ALPHA;       //TODO: tuning point
        //double d = RandFloat::get(0.0, 4.0) - 2.0;
        //double M = ALPHA + RandFloat::get(0.0, 1.0) - 0.5; 

        if (splitY) {
            for (auto i : group) {
                if (points[i].second <= (points[i].first - center.first) * M + center.second + d)
                    g1.push_back(i);
                else
                    g2.push_back(i);
            }
        } else {
            M = 1.0 / M;
            for (auto i : group) {
                if (points[i].second <= (center.first - points[i].first) * M + center.first + d)
                    g1.push_back(i);
                else
                    g2.push_back(i);
            }
        }
    }

    //---

    //double calcCost(double distSum, int groupCount);
    static double doClustering(bool splitY, const vector<pair<T,T>>& points, int maxGroupSize, int maxGroupCount,
                               vector<pair<T,T>>& centers, vector<vector<int>>& groups,
                               const function<double(double,int)>& calcCost) {
        int N = int(points.size());

        //--- step #1
        priority_queue<tuple<int, bool, shared_ptr<vector<int>>>> Q;
        Q.emplace(N, splitY, make_shared<vector<int>>(N));
        iota(get<2>(Q.top())->begin(), get<2>(Q.top())->end(), 0);

        while (get<0>(Q.top()) > maxGroupSize) {
            bool splitY = get<1>(Q.top());
            auto c = calcCentroid(points, *get<2>(Q.top()));

            auto g1 = make_shared<vector<int>>();
            auto g2 = make_shared<vector<int>>();
            split1(points, *get<2>(Q.top()), c, *g1, *g2, splitY);
            Q.pop();

            if (!g1->empty())
                Q.emplace(int(g1->size()), !splitY, g1);
            if (!g2->empty())
                Q.emplace(int(g2->size()), !splitY, g2);
        }

        //--- step #2
        priority_queue<tuple<double, bool, shared_ptr<vector<int>>>> Q2;
        double distSum = 0;
        while (!Q.empty()) {
            auto c = calcCentroid(points, *get<2>(Q.top()));
            double d = sumDistance(points, *get<2>(Q.top()), c);
            Q2.emplace(d, get<1>(Q.top()), get<2>(Q.top()));
            Q.pop();

            distSum += d;
        }

        double minCost = calcCost(distSum, int(Q2.size()));
        while ((int)Q2.size() < maxGroupCount && get<0>(Q2.top()) > 0) {
            bool splitY = get<1>(Q2.top());
            auto c = calcCentroid(points, *get<2>(Q2.top()));

            auto g1 = make_shared<vector<int>>();
            auto g2 = make_shared<vector<int>>();
            split1(points, *get<2>(Q2.top()), c, *g1, *g2, splitY);

            double newDistSum = distSum - get<0>(Q2.top());
            int newCnt = int(Q2.size()) - 1;

            double g1D = 0.0;
            if (!g1->empty()) {
                auto g1C = calcCentroid(points, *g1);
                g1D = sumDistance(points, *g1, g1C);
                newDistSum += g1D;
                newCnt++;
            }

            double g2D = 0.0;
            if (!g2->empty()) {
                auto g2C = calcCentroid(points, *g2);
                g2D = sumDistance(points, *g2, g2C);
                newDistSum += g2D;
                newCnt++;
            }

            double cost = calcCost(newDistSum, newCnt);
            if (cost > minCost) {
                double d = get<0>(Q2.top());
                auto g = get<2>(Q2.top());
                Q2.pop();
                Q2.emplace(-d, splitY, g);
                continue;
            }

            distSum = newDistSum;
            minCost = cost;

            Q2.pop();
            if (!g1->empty())
                Q2.emplace(g1D, !splitY, g1);
            if (!g2->empty())
                Q2.emplace(g2D, !splitY, g2);
        }

        //---

        int M = int(Q2.size());
        centers.resize(M);
        groups.reserve(M);
        while (!Q2.empty()) {
            groups.push_back(*get<2>(Q2.top()));
            Q2.pop();
        }

        for (int i = 0; i < M; i++)
            centers[i] = calcCentroid(points, groups[i]);

        return minCost;
    }

//private:
    static pair<T, T> calcCentroid(const vector<pair<T, T>>& points, const vector<int>& group) {
        if (group.size() == 1)
            return points[group[0]];

        int N = int(group.size());
        pair<T, T> res;
        for (auto i : group) {
            res.first += points[i].first;
            res.second += points[i].second;
        }

        div(res, N);

        return res;
    }

    static double calcDist(const pair<T, T>& p1, const pair<T, T>& p2) {
        return sqrt(T(p1.first - p2.first) * T(p1.first - p2.first)
                  + T(p1.second - p2.second) * T(p1.second - p2.second));
    }

    static double sumDistance(const vector<pair<T,T>>& points, const vector<int>& group, const pair<T, T>& center) {
        double res = 0.0;
        for (auto i : group)
            res += calcDist(points[i], center);
        return res;
    }

};

template <typename T>
struct KMeans {
    struct RandInt32 {
        static int get() {
            static mt19937 eng(7);
            //static auto seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
            //static std::default_random_engine eng(seed);
            static uniform_int_distribution<int> dist(0, numeric_limits<int>::max());
            return dist(eng);
        }
    };

    static void div(pair<int, int>& p, int size) {
        p.first = (p.first + (size >> 1)) / size;
        p.second = (p.second = (size >> 1)) / size;
    }

    static void div(pair<long long, long long>& p, int size) {
        p.first = (p.first + (size >> 1)) / size;
        p.second = (p.second = (size >> 1)) / size;
    }

    static void div(pair<double, double>& p, int size) {
        p.first = p.first / size;
        p.second = p.second / size;
    }

    static const int MAX_ITERATION = 2000;
    static const int NN_SIZE = 10;

    int maxIteration = MAX_ITERATION;
    int nnSize = NN_SIZE;

    vector<pair<T, T>> centers;
    vector<double> distances;
    vector<int> assignments;

    function<bool()> isTimeout;

    //---

    explicit KMeans(const function<bool()>& isTimeout) : isTimeout(isTimeout) {
    }

    void clear() {
        centers.clear();
        distances.clear();
        assignments.clear();
    }


    template <typename U>
    double doKMeans(const vector<pair<U, U>>& points, const vector<pair<U, U>>& centers,
                    int maxIter = MAX_ITERATION, int nnSize = NN_SIZE) {
        int N = int(points.size());
        int M = int(centers.size());

        this->maxIteration = maxIter;
        this->nnSize = nnSize;
        this->assignments.assign(N, -1);
        this->distances.assign(N, 0.0);

        this->centers.resize(M);
        for (int i = 0; i < M; i++) {
            this->centers[i].first = centers[i].first;
            this->centers[i].second = centers[i].second;
        }
        quantize(points);

        double c = refineCenters(points);
        quantizeFast(points);

        return c;
    }

    //---

    void quantize(const vector<pair<T, T>>& points) {
        int N = int(points.size());
        int M = int(centers.size());
        for (int i = 0; i < N; i++) {
            int k = 0;
            auto minDist = numeric_limits<double>::max();

            for (int j = 0; j < M; j++) {
                auto d = calcDist(points[i], centers[j]);
                if (d < minDist) {
                    minDist = d;
                    k = j;
                }
            }

            assignments[i] = k;
            distances[i] = minDist;
        }
    }

    void quantizeFast(const vector<pair<T, T>>& points) {
        int N = int(points.size());
        int M = int(centers.size());

        vector<double> s(M);
        vector<vector<pair<double, int>>> nnCenters(M);
        for (size_t c = 0; c < M; c++)
            nnCenters[c].reserve(nnSize + 1);

        for (int c1 = 0; c1 < M; c1++) {
            nnCenters[c1].clear();

            s[c1] = numeric_limits<double>::max();
            for (int c2 = 0; c2 < M; c2++) {
                if (c2 == c1)
                    continue;

                double d2 = calcDist(centers[c1], centers[c2]);
                if (d2 < s[c1])
                    s[c1] = d2;

                nnCenters[c1].emplace_back(d2, c2);
                push_heap(nnCenters[c1].begin(), nnCenters[c1].end());
                if (nnCenters[c1].size() > nnSize) {
                    pop_heap(nnCenters[c1].begin(), nnCenters[c1].end());
                    nnCenters[c1].pop_back();
                }
            }
            s[c1] = s[c1] / 2;
        }

        for (int i = 0; i < N; i++) {
            auto closest = assignments[i];

            auto u2 = calcDist(points[i], centers[closest]);
            distances[i] = u2;

            auto l2 = numeric_limits<double>::max();
            for (auto& it : nnCenters[closest]) {
                int j = it.second;

                auto dist2 = calcDist(points[i], centers[j]);
                if (dist2 < u2) {
                    l2 = u2;
                    u2 = dist2;
                    closest = j;
                } else if (dist2 < l2) {
                    l2 = dist2;
                }
            }

            if (assignments[i] != closest)
                assignments[i] = closest;
            distances[i] = u2;
        }
    }

    double refineCenters(const vector<pair<T, T>>& points) {
        int N = int(points.size());
        int M = int(centers.size());

        vector<double> s(M);
        vector<double> upper(N, numeric_limits<double>::max());
        vector<double> lower(N);

        vector<double> centerMovement(M);
        vector<int> clusterSize(M);
        vector<pair<T, T>> tempCenters(M);

        vector<vector<pair<double, int>>> nnCenters(M);
        for (size_t c = 0; c < M; c++)
            nnCenters[c].reserve(nnSize + 1);

        quantizeFast(points);

        for (int iterationCount = 0; iterationCount < maxIteration; iterationCount++) {
            if (isTimeout())
                break;

            mergeToCenters(clusterSize, tempCenters, points);

            for (int c = 0; c < M; c++) {
                auto& cpt = tempCenters[c];
                if (clusterSize[c] > 0) {
                    div(cpt, clusterSize[c]);
                    centerMovement[c] = calcDist(cpt, centers[c]);
                } else {
                    int x = RandInt32::get() % N;
                    cpt.first = points[x].first;
                    cpt.second = points[x].second;
                    centerMovement[c] = 0.0;
                }
            }

            int furthestMovingCenter = 0;
            double longest = 0.0;
            double secondLongest = 0.0;
            for (int c = 0; c < M; c++) {
                if (longest < centerMovement[c]) {
                    secondLongest = longest;
                    longest = centerMovement[c];
                    furthestMovingCenter = c;
                } else if (secondLongest < centerMovement[c]) {
                    secondLongest = centerMovement[c];
                }
            }

            centers.swap(tempCenters);
            if (centerMovement[furthestMovingCenter] < 1e-9)
                break;

            for (int c1 = 0; c1 < M; c1++) {
                if ((c1 & 0xff) == 0) {
                    if (isTimeout())
                        goto lbExit;
                }
                nnCenters[c1].clear();

                s[c1] = numeric_limits<double>::max();
                for (int c2 = 0; c2 < M; c2++) {
                    if (c2 == c1)
                        continue;

                    double d2 = calcDist(centers[c1], centers[c2]);
                    if (d2 < s[c1])
                        s[c1] = d2;

                    nnCenters[c1].emplace_back(d2, c2);
                    push_heap(nnCenters[c1].begin(), nnCenters[c1].end());
                    if (nnCenters[c1].size() > nnSize) {
                        pop_heap(nnCenters[c1].begin(), nnCenters[c1].end());
                        nnCenters[c1].pop_back();
                    }
                }
                s[c1] = s[c1] / 2;
            }

            for (int i = 0; i < N; i++) {
                if ((i & 0xff) == 0) {
                    if (isTimeout())
                        goto lbExit;
                }

                if (iterationCount > 0) {
                    upper[i] += centerMovement[assignments[i]];
                    lower[i] -= (assignments[i] == furthestMovingCenter) ? secondLongest : longest;
                }

                auto closest = assignments[i];

                auto upperComparisonBound = max(s[closest], lower[i]);
                if (upper[i] <= upperComparisonBound)
                    continue;

                auto u2 = calcDist(points[i], centers[closest]);
                distances[i] = u2;
                upper[i] = u2;
                if (upper[i] <= upperComparisonBound)
                    continue;

                auto l2 = numeric_limits<double>::max();
                for (auto& it : nnCenters[closest]) {
                    int j = it.second;

                    auto dist2 = calcDist(points[i], centers[j]);
                    if (dist2 < u2) {
                        l2 = u2;
                        u2 = dist2;
                        closest = j;
                    } else if (dist2 < l2) {
                        l2 = dist2;
                    }
                }

                lower[i] = l2;
                if (assignments[i] != closest) {
                    upper[i] = u2;
                    assignments[i] = closest;
                }
                distances[i] = u2;
            }
        }
    lbExit:

        return calcEnergy(distances);
    }


//private:
    static double calcDist(const pair<T, T>& p1, const pair<T, T>& p2) {
        return sqrt(T(p1.first - p2.first) * T(p1.first - p2.first)
                  + T(p1.second - p2.second) * T(p1.second - p2.second));
    }

    static double calcEnergy(const vector<double>& distances) {
        return accumulate(distances.begin(), distances.end(), 0.0);
    }

    void mergeToCenters(vector<int>& clusterSize, vector<pair<T, T>>& centers, const vector<pair<T, T>>& points) {
        int N = int(points.size());
        int M = int(centers.size());

        clusterSize.assign(M, 0);
        centers.assign(M, pair<T, T>(0, 0));

        for (int i = 0; i < N; i++) {
            auto c = assignments[i];

            ++clusterSize[c];
            centers[c].first += points[i].first;
            centers[c].second += points[i].second;
        }
    }
};

