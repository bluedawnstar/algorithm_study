static ostream& operator <<(ostream& os, const pair<int, int>& rhs) {
    cout << "(" << rhs.first << ", " << rhs.second << ")";
    return os;
}

static ostream& operator <<(ostream& os, const pair<int, pair<int, int>>& rhs) {
    cout << "(" << rhs.first << ", " << rhs.second.first << ", " << rhs.second.second << ")";
    return os;
}

static ostream& operator <<(ostream& os, vector<int>& rhs) {
    cout << "(";
    if (!rhs.empty())
        cout << rhs[0];
    for (int i = 1; i < (int)rhs.size(); i++)
        cout << ", " << rhs[i];
    cout << ")";

    return os;
}

static ostream& operator <<(ostream& os, vector<pair<int, int>>& rhs) {
    cout << "(";
    if (!rhs.empty())
        cout << rhs[0];
    for (int i = 1; i < (int)rhs.size(); i++)
        cout << ", " << rhs[i];
    cout << ")";

    return os;
}

static ostream& operator <<(ostream& os, vector<vector<pair<int, int>>>& rhs) {
    for (int i = 0; i < (int)rhs.size(); i++)
        cout << i << ": " << rhs[i] << endl;
    return os;
}

static ostream& operator <<(ostream& os, vector<string>& rhs) {
    cout << "(";
    if (!rhs.empty())
        cout << "\"" << rhs[0] << "\"";
    for (int i = 1; i < (int)rhs.size(); i++)
        cout << ", " << "\"" << rhs[i] << "\"";
    cout << ")";

    return os;
}
