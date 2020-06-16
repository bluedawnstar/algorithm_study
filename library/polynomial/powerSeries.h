#pragma once

#include "ntt.h"
#include "polyFFTMod2.h"

/*
  1) e^x = x^0/0! + x^1/1! + x^2/2! + x^3/3! + ...

     e^x - 1 = x^1/1! + x^2/2! + x^3/3! + ...

     (e^x + e^-x)/2 = x^0/0! + x^2/2! + x^4/4! + ...

  2) (1 + x)^n = C(n,0)*x^0 + C(n,1)*x^1 + C(n,2)*x^2 + ... + C(n,n)*x^n

               = P(n,0)*x^0/0! + P(n,1)*x^1/1! + P(n,2)*x^2/2! + ... + P(n,n)*x^n/n!


  3) 1 + x + x^2 + x^3 + ... = 1 / (1 - x)

     1 - x + x^2 - x^3 + ... = 1 / (1 + x)

  4) ln(1 + x) = x/1 - x^2/2 + x^3/3 - x^4/4 + ...

  5) ln(1 - x) = - x/1 - x^2/2 - x^3/3 - x^4/4 - ...
*/

//template <int mod = 1'000'000'007, int root = 5>
template <int mod = 998'244'353, int root = 3>
struct PowerSeriesMod {
    vector<int> inverse;

    // N = max degree
    void prepare(int N) {
        inverse.resize(N + 1);

        inverse[0] = 0;
        inverse[1] = 1;
        for (int i = 2; i <= N; i++)
            inverse[i] = int((mod - 1ll * (mod / i) * inverse[mod % i] % mod) % mod);
    }

    /*
      f(x) =  (1 + x^a1 + x^(2*a1) + x^(3*a1) + ... + x^(b1*a1))
            * (1 + x^a2 + x^(2*a2) + x^(3*a2) + ... + x^(b2*a2))
            * ...
            * (1 + x^an + x^(2*an) + x^(3*an) + ... + x^(bn*an))

           =  (1 + (x^a1)^1 + (x^a1)^2 + (x^a1)^3 + ... + (x^a1)^b1)
            * (1 + (x^a2)^1 + (x^a2)^2 + (x^a2)^3 + ... + (x^a2)^b2)
            * ...
            * (1 + (x^an)^1 + (x^an)^2 + (x^an)^3 + ... + (x^an)^bn)

           =  (1 - x^(a1*(b1 + 1))) / (x - x^a1)
            * (1 - x^(a2*(b2 + 1))) / (x - x^a2)
            * ...
            * (1 - x^(an*(bn + 1))) / (x - x^an)

      ln(f(x)) =  ln(1 - x^(a1*(b1 + 1))) - ln(x - x^a1)
                + ln(1 - x^(a2*(b2 + 1))) - ln(x - x^a2)
                + ...
                + ln(1 - x^(an*(bn + 1))) - ln(x - x^an)

               =  - x^(a1*(b1 + 1))/1 - x^(2*a1*(b1 + 1))/2 - x^(3*a1*(b1 + 1))/3 - ...
                  + x^(a1)/1 + x^(2*a1)/2 + x^(3*a1)/3 + ...

                  - x^(a2*(b2 + 1))/1 - x^(2*a2*(b2 + 1))/2 - x^(3*a2*(b2 + 1))/3 - ...
                  + x^(a2)/1 + x^(2*a2)/2 + x^(3*a2)/3 + ...

                  ...

                  - x^(an*(bn + 1))/1 - x^(2*an*(bn + 1))/2 - x^(3*an*(bn + 1))/3 - ...
                  + x^(an)/1 + x^(2*an)/2 + x^(3*an)/3 + ...

               = c1 * x^1 + c2 * x^2 + c3 * x^3 + ...

      f(x) = e^{ln(f(x))} = e^{c1 * x^1 + c2 * x^2 + c3 * x^3 + ...}
    */
    // poly = { (a1, b1}, {a2, b2}, {a3, b3}, ..., {an, bn} }
    // N = max degree
    vector<int> calculateAllCombinationWithNTT(const vector<pair<int, int>> poly, int N) {
        if (inverse.empty())
            prepare(N);

        unordered_map<int, int> qM;     // (1 - x^(ai*(bi + 1))) * ...
        unordered_map<int, int> pM;     // (1 - x^ai) * ...

        for (int i = 0; i < int(poly.size()); i++) {
            if (1ll * poly[i].first * (poly[i].second + 1) <= N) {
                int ab = poly[i].first * (poly[i].second + 1);
                ++qM[ab];
            }
            ++pM[poly[i].first];
        }

        vector<int> lnF(N + 1);
        for (const auto& it : pM) {
            int d = it.first;
            int n = it.second;
            for (int i = d, j = 1; i <= N; i += d, j++) {
                lnF[i] += int(1ll * n * inverse[j] % mod);
                if (lnF[i] >= mod)
                    lnF[i] -= mod;
            }
        }
        for (const auto& it : qM) {
            int d = it.first;
            int n = it.second;
            for (int i = d, j = 1; i <= N; i += d, j++) {
                lnF[i] -= int(1ll * n * inverse[j] % mod);
                if (lnF[i] < 0)
                    lnF[i] += mod;
            }
        }

        return NTT<mod, root>::exp(lnF);
    }

    vector<int> calculateAllCombinationWithFFT(const vector<pair<int, int>> poly, int N) {
        if (inverse.empty())
            prepare(N);

        unordered_map<int, int> qM;     // (1 - x^(ai*(bi + 1))) * ...
        unordered_map<int, int> pM;     // (1 - x^ai) * ...

        for (int i = 0; i < int(poly.size()); i++) {
            if (1ll * poly[i].first * (poly[i].second + 1) <= N) {
                int ab = poly[i].first * (poly[i].second + 1);
                ++qM[ab];
            }
            ++pM[poly[i].first];
        }

        vector<int> lnF(N + 1);
        for (const auto& it : pM) {
            int d = it.first;
            int n = it.second;
            for (int i = d, j = 1; i <= N; i += d, j++) {
                lnF[i] += int(1ll * n * inverse[j] % mod);
                if (lnF[i] >= mod)
                    lnF[i] -= mod;
            }
        }
        for (const auto& it : qM) {
            int d = it.first;
            int n = it.second;
            for (int i = d, j = 1; i <= N; i += d, j++) {
                lnF[i] -= int(1ll * n * inverse[j] % mod);
                if (lnF[i] < 0)
                    lnF[i] += mod;
            }
        }

        PolyFFTMod2<mod> fft;
        return fft.exp(lnF);
    }
};
