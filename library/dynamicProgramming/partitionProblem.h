#pragma once

// https://en.wikipedia.org/wiki/Partition_problem

// the task of deciding whether a given multiset S of positive integers can be partitioned into two subsets S1 and S2
//   such that the sum of the numbers in S1 equals the sum of the numbers in S

struct PartitionProblem {
    // O(sum * n)
    static bool existPartition(const int value[], int n) {
        int sum = 0;
        for (int i = 0; i < n; i++)
            sum += value[i];

        if (sum & 1)
            return false;

        int halfSum = sum >> 1;
        vector<vector<bool>> part(halfSum + 1, vector<bool>(n + 1));

        for (int i = 0; i <= n; i++)
            part[0][i] = true;
        for (int i = 1; i <= halfSum; i++)
            part[i][0] = false;

        for (int i = 1; i <= sum / 2; i++) {
            for (int j = 1; j <= n; j++) {
                part[i][j] = part[i][j - 1];
                if (i >= value[j - 1])
                    part[i][j] = part[i][j] || part[i - value[j - 1]][j - 1];
            }
        }

        return part[sum / 2][n];
    }

    // O(sum * n)
    static bool existPartition(const vector<int>& value) {
        return existPartition(value.data(), int(value.size()));
    }
};

struct PartitionProblemEx {
    int N;
    vector<int> value;

    int sum;
    vector<vector<pair<int, int>>> part;    // (exclude current value, include current value)

    PartitionProblemEx() {
    }

    PartitionProblemEx(const int value[], int n) {
        solve(value, n);
    }

    PartitionProblemEx(const vector<int>& value) {
        solve(value);
    }

    // O(sum * n)
    int solve(const int value[], int n) {
        this->N = n;
        this->value.assign(value, value + n);

        sum = 0;
        for (int i = 0; i < n; i++)
            sum += value[i];

        if (sum & 1)
            return 0;

        int halfSum = sum >> 1;
        part = vector<vector<pair<int, int>>>(halfSum + 1, vector<pair<int, int>>(n + 1));

        for (int i = 0; i <= n; i++) {
            part[0][i].first = 1;
            part[0][i].second = 0;
        }

        for (int i = 1; i <= sum / 2; i++) {
            for (int j = 1; j <= n; j++) {
                part[i][j].first = part[i][j - 1].first
                                 + part[i][j - 1].second;
                if (i >= value[j - 1])
                    part[i][j].second = part[i - value[j - 1]][j - 1].first
                                      + part[i - value[j - 1]][j - 1].second;
            }
        }

        return part[halfSum][n].first + part[halfSum][n].second;
    }

    // O(sum * n)
    int solve(const vector<int>& value) {
        return solve(value.data(), int(value.size()));
    }


    bool exist() const {
        return (sum & 1) == 0 && (part[sum >> 1][N].first + part[sum >> 1][N].second > 0);
    }

    int getPartitionCount() const {
        if (sum & 1)
            return 0;
        return (part[sum >> 1][N].first + part[sum >> 1][N].second) / 2;
    }

    pair<vector<int>, vector<int>> getAnyPartition() const {
        pair<vector<int>, vector<int>> res;
        if (exist()) {
            int currSum = sum >> 1;
            int j = N;
            while (j > 0) {
                if (part[currSum][j--].first > 0) {
                    res.second.push_back(value[j]);
                } else {
                    res.first.push_back(value[j]);
                    currSum -= value[j];
                }
            }
            reverse(res.first.begin(), res.first.end());
            reverse(res.second.begin(), res.second.end());
        }
        return res;
    }
};
