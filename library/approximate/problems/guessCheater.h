#pragma once

/*
  <Cheating Detection>
    https://codingcompetitions.withgoogle.com/codejam/round/000000000043580a/00000000006d1155#problem

  1. probability
     - When player i tries to answer question j,
       the probability that they answer it correctly is f(Si - Qj)

       . player i has a skill level of Si           (-3.0 <= Si <= 3.0)
       . question j has a difficulty level of Qj    (-3.0 <= Qj <= 3.0)
       . f(x) = 1 / (1 + e^-x)

     - there is a cheater who decide whether to cheat at random with 0.5 probability for each question
       . cheat with 0.5 probability for each question
       . if not cheat, then follow f(Si - Qj)

  2. how to find the cheater
     - The cheater has the maximum inversion rate.
       (inversions = the number of pairs that couldn't solve the easy problem, but the difficult problem)

  3. result
     - accuracy is about 99.7% !
     - from the solution of Gennady.Korotkevich
       https://codingcompetitions.withgoogle.com/codejam/submissions/000000000043580a/R2VubmFkeS5Lb3JvdGtldmljaA
*/
// O(N*logN)
int guessCheater(const vector<string>& S) {
    int N = int(S.size());
    int M = int(S[0].size());

    // step #1 : order by difficulty of problems
    vector<int> cnt(M);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (S[i][j] == '1')
                cnt[j]++;
        }
    }

    vector<int> order(M);   // ascending order (easy problems to hard problems)
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&cnt](int a, int b) {
        return cnt[a] > cnt[b];
    });

    // step #2 : inversion rate
    // - rate of cases where easy problems are not solved but difficult problems are solved
    vector<double> person;
    for (int i = 0; i < N; i++) {
        int inv = 0;
        int one = 0;
        int zero = 0;
        for (auto j : order) {
            if (S[i][j] == '1') {
                inv += zero;
                one++;          // solved
            } else {
                zero++;         // not solved
            }
        }

        person.emplace_back(1.0 * inv / (1.0 * one * zero));
    }

    // step #3 : guess the cheater! (the person with max inversion rate)
    return int(max_element(person.begin(), person.end()) - person.begin());
}
