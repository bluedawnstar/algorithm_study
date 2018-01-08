class Twitter {
    unordered_map<int, unordered_set<int>> following;   // user id -> user ids
    vector<pair<int, int>> tweets;                      // (tweet id, user id)
public:
    /** Initialize your data structure here. */
    Twitter() {

    }

    /** Compose a new tweet. */
    void postTweet(int userId, int tweetId) {
        tweets.emplace_back(tweetId, userId);
    }

    /** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
    vector<int> getNewsFeed(int userId) {
        vector<int> res;
        auto it = following.find(userId);
        for (int i = (int)tweets.size() - 1; i >= 0 && (int)res.size() < 10; i--) {
            if (userId == tweets[i].second
                || (it != following.end() && it->second.find(tweets[i].second) != it->second.end()))
                res.push_back(tweets[i].first);
        }
        return res;
    }

    /** Follower follows a followee. If the operation is invalid, it should be a no-op. */
    void follow(int followerId, int followeeId) {
        following[followerId].insert(followeeId);
    }

    /** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
    void unfollow(int followerId, int followeeId) {
        following[followerId].erase(followeeId);
    }
};

/**
* Your Twitter object will be instantiated and called as such:
* Twitter obj = new Twitter();
* obj.postTweet(userId,tweetId);
* vector<int> param_2 = obj.getNewsFeed(userId);
* obj.follow(followerId,followeeId);
* obj.unfollow(followerId,followeeId);
*/