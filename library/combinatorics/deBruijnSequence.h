#pragma once

// https://en.wikipedia.org/wiki/De_Bruijn_sequence
struct DeBruijnSequence {
    // k = the number of digit, n = subsequence length
    static vector<int> deBruijnSequence(int k, int n) {
        vector<int> a(k * n);
        vector<int> sequence;

        sequence.reserve(int(pow(k, n)) + n - 1);

        function<void(int, int)> db;
        db = [&](int t, int p) {
            if (t > n) {
                if (n % p == 0) {
                    for (int i = 1; i <= p; i++)
                        sequence.push_back(a[i]);
                }
            } else {
                a[t] = a[t - p];
                db(t + 1, p);
                for (int i = a[t - p] + 1; i < k; i++) {
                    a[t] = i;
                    db(t + 1, t);
                }
            }
        };
        db(1, 1);

        return sequence;
    }

    // k = the number of alphabet, n = subsequence length
    static string deBruijnAlphabet(int k, int n, char base = '0') {
        auto sequence = deBruijnSequence(k, n);
        string res;
        res.reserve(sequence.size());
        for (auto x : sequence)
            res.push_back(base + x);
        return res;
    }
};

// with Hamiltonian Path
struct DeBruijnSequenceWithHamiltonianCircuit {
    // k = the number of digit, n = subsequence length
    // return reverse order, O(N*K^N)
    static string deBruijnAlphabet(int k, int n, char base = '0') {
        if (n == 1 && k == 1)
            return string(1, base);

        unordered_set<string> seen;
        string start(n - 1, base);
        string ans;

        dfs(seen, ans, start, k, base);
        ans += start;

        reverse(ans.begin(), ans.end());

        // for circular sequence
        ans.resize(ans.length() - n + 1);   //TODO: comment this line for non-circular sequence

        return ans;
    }

private:
    static void dfs(unordered_set<string>& seen, string& ans, const string& node, int k, char base = '0') {
        string nei = node;
        nei.push_back(base);

        for (int x = 0; x < k; ++x) {
            if (seen.find(nei) == seen.end()) {
                seen.insert(nei);
                dfs(seen, ans, nei.substr(1), k, base);
                ans.push_back(base + x);
            }
            ++nei.back();
        }
    }
};

// with Inverse Burrows-Wheeler Transform
struct DeBruijnSequenceWithIBWT {
    // k = the number of digit, n = subsequence length
    // return reverse order, O(K^N)
    static string deBruijnAlphabet(int k, int n, char base = '0') {
        int M = int(pow(k, n - 1));
        vector<int> P(M * k);

        for (int i = 0; i < k; i++) {
            for (int q = 0; q < M; q++)
                P[i * M + q] = q * k + i;
        }

        string ans;
        for (int i = 0; i < M * k; i++) {
            int j = i;
            while (P[j] >= 0) {
                ans.push_back(base + j / M);
                int v = P[j];
                P[j] = -1;
                j = v;
            }
        }

        // for non-circular sequence
        //for (int i = 0; i < n - 1; i++)   //TODO: comment this line for non-circular sequence
        //    ans.push_back(base);          //TODO: comment this line for non-circular sequence

        return ans;
    }
};
