#pragma once

template <typename T, typename U>
static ostream& operator <<(ostream& os, const pair<T, U>& rhs) {
    os << "(" << rhs.first << ", " << rhs.second << ")";
    return os;
}

static ostream& operator <<(ostream& os, const pair<int, pair<int, int>>& rhs) {
    os << "(" << rhs.first << ", " << rhs.second.first << ", " << rhs.second.second << ")";
    return os;
}

template <typename T>
static ostream& operator <<(ostream& os, const vector<T>& rhs) {
    os << "{ ";
    if (!rhs.empty())
        os << rhs[0];
    for (int i = 1; i < (int)rhs.size(); i++)
        os << ", " << rhs[i];
    os << " }";

    return os;
}

static ostream& operator <<(ostream& os, const vector<pair<int, int>>& rhs) {
    os << "{ ";
    if (!rhs.empty())
        os << rhs[0];
    for (int i = 1; i < (int)rhs.size(); i++)
        os << ", " << rhs[i];
    os << " }";

    return os;
}

static ostream& operator <<(ostream& os, const vector<vector<pair<int, int>>>& rhs) {
    for (int i = 0; i < (int)rhs.size(); i++)
        os << i << ": " << rhs[i] << endl;
    return os;
}

static ostream& operator <<(ostream& os, const vector<string>& rhs) {
    os << "{ ";
    if (!rhs.empty())
        os << "\"" << rhs[0] << "\"";
    for (int i = 1; i < (int)rhs.size(); i++)
        os << ", " << "\"" << rhs[i] << "\"";
    os << " }";

    return os;
}
