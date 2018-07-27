#include <vector>
#include <string>
#include <algorithm>

using namespace std;

namespace NotUsed {
//--- O(N (logN)^2) method ---

template <typename T>
vector<int> makeSuffixArray(T s, int n) {
    vector<int> group(n + 1);
    for (int i = 0; i < n; i++)
        group[i] = s[i];
    group[n] = -1;

    vector<int> perm(n);
    for (int i = 0; i < n; i++)
        perm[i] = i;

    int t = 1;
    while (t < n) {
        auto op = [t, &group](int a, int b) {
            if (group[a] != group[b])
                return group[a] < group[b];
            return group[a + t] < group[b + t];
        };
        sort(perm.begin(), perm.end(), op);

        t *= 2;
        if (t >= n) break;

        vector<int> g2(n + 1);
        g2[n] = -1;
        g2[perm[0]] = 0;
        for (int i = 1; i < int(perm.size()); i++) {
            if (op(perm[i - 1], perm[i]))
                g2[perm[i]] = g2[perm[i - 1]] + 1;
            else
                g2[perm[i]] = g2[perm[i - 1]];
        }
        group = g2;
    }

    return perm;
}

//--- Skew Method O(N) ---

// stably sort a[0..n-1] to b[0..n-1] with keys in 0..K from r
static void radixPass(vector<int>& a, vector<int>& b, vector<int>& r, int rOff, int n, int K) { // count occurrences
    vector<int> c(K + 1);                   // counter array
    for (int i = 0; i <= K; i++)
        c[i] = 0;                           // reset counters
    for (int i = 0; i < n; i++)
        c[r[a[i] + rOff]]++;                // count occurences
    for (int i = 0, sum = 0; i <= K; i++) { // exclusive prefix sums
        int t = c[i]; c[i] = sum; sum += t;
    }
    for (int i = 0; i < n; i++)
        b[c[r[a[i] + rOff]]++] = a[i];      // sort
}

static inline bool leq(int a1, int a2, int b1, int b2) { // lexic. order for pairs
    return(a1 < b1 || a1 == b1 && a2 <= b2);
}                                                   // and triples

static inline bool leq(int a1, int a2, int a3, int b1, int b2, int b3) {
    return(a1 < b1 || a1 == b1 && leq(a2, a3, b2, b3));
}

// building suffix array O(N)
// find the suffix array SA of s[0..n-1] in {1..K}^n
// require s[n]=s[n+1]=s[n+2]=0, n>=2
static void buildSuffixArrayFastest(vector<int>& s, vector<int>& SA, int n, int K) {
    int n0 = (n + 2) / 3;
    int n1 = (n + 1) / 3;
    int n2 = n / 3;
    int n02 = n0 + n2;
    vector<int> s12(n02 + 3);
    vector<int> SA12(n02 + 3);
    vector<int> s0(n0);
    vector<int> SA0(n0);

    s12[n02] = s12[n02 + 1] = s12[n02 + 2] = 0;
    SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;

    // generate positions of mod 1 and mod  2 suffixes
    // the "+(n0-n1)" adds a dummy mod 1 suffix if n%3 == 1
    for (int i = 0, j = 0; i < n + (n0 - n1); i++)
        if (i % 3 != 0)
            s12[j++] = i;

    // lsb radix sort the mod 1 and mod 2 triples
    radixPass(s12, SA12, s, 2, n02, K);
    radixPass(SA12, s12, s, 1, n02, K);
    radixPass(s12, SA12, s, 0, n02, K);

    // find lexicographic names of triples
    int name = 0, c0 = -1, c1 = -1, c2 = -1;
    for (int i = 0; i < n02; i++) {
        if (s[SA12[i]] != c0 || s[SA12[i] + 1] != c1 || s[SA12[i] + 2] != c2) {
            name++;
            c0 = s[SA12[i]];
            c1 = s[SA12[i] + 1];
            c2 = s[SA12[i] + 2];
        }
        if (SA12[i] % 3 == 1)
            s12[SA12[i] / 3] = name;        // left half
        else
            s12[SA12[i] / 3 + n0] = name;   // right half
    }

    // recurse if names are not yet unique
    if (name < n02) {
        buildSuffixArrayFastest(s12, SA12, n02, name);
        // store unique names in s12 using the suffix array 
        for (int i = 0; i < n02; i++)
            s12[SA12[i]] = i + 1;
    } else { // generate the suffix array of s12 directly
        for (int i = 0; i < n02; i++)
            SA12[s12[i] - 1] = i;
    }

    // stably sort the mod 0 suffixes from SA12 by their first character
    for (int i = 0, j = 0; i < n02; i++)
        if (SA12[i] < n0)
            s0[j++] = 3 * SA12[i];
    radixPass(s0, SA0, s, 0, n0, K);

    // merge sorted SA0 suffixes and sorted SA12 suffixes
    for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
#define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
        int i = GetI(); // pos of current offset 12 suffix
        int j = SA0[p]; // pos of current offset 0  suffix
        if (SA12[t] < n0 ? leq(s[i], s12[SA12[t] + n0], s[j], s12[j / 3])
            : leq(s[i], s[i + 1], s12[SA12[t] - n0 + 1], s[j], s[j + 1], s12[j / 3 + n0])) {
            // suffix from SA12 is smaller
            SA[k] = i;
            if (++t == n02) { // done --- only SA0 suffixes left
                for (k++; p < n0; p++, k++)
                    SA[k] = SA0[p];
            }
        } else {
            SA[k] = j;
            if (++p == n0) { // done --- only SA12 suffixes left
                for (k++; t < n02; t++, k++)
                    SA[k] = GetI();
            }
        }
    }
}

// O(N)
template <typename T>
vector<int> makeSuffixArrayFastest(T s, int n) {
    if (n < 1)
        return vector<int>();
    else if (n < 2)
        return vector<int>(1, 0);

    //TODO: check maximum value
    int alphabetSize = 26; //= *max_element(s, s + n) - 'a' + 1;
    vector<int> SA(n + 3);

    vector<int> in(n + 3);
    for (int i = 0; i < n; i++)
        in[i] = s[i] - 'a' + 1; //TODO: check conversion from character to value

    buildSuffixArrayFastest(in, SA, n, alphabetSize);
    SA.resize(n);

    return SA;
}

}
