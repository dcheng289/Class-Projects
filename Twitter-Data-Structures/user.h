#ifndef USER_H
#define USER_H
#include <set>
#include <list>
#include "tweet.h"

/* Forward Declaration to avoid #include dependencies */
class Tweet;

class User {
 public:

  // Constructor
  User(std::string name);

  // Destructor
  ~User();

  // Returns name of the user
  std::string name() const; 

  // Returns set of the user's followers (the users who follow this user)
  std::set<User*> followers() const;

  // Returns set of the user's following (the users he follows)
  std::set<User*> following() const;

  // Returns list of public tweets posted by this user
  std::list<Tweet*> tweets() const; 

  // Adds a user to the set of this user's followers
  void addFollower(User* u);

  // Adds a user to the set of whom this user follows
  void addFollowing(User* u);

  // Adds to the list of tweets posted by this user
  void addTweet(Tweet* t);

  // Adds to the list of private tweets for this user
  void addPrivateTweet(Tweet* t);

  // Adds to the list of mentioned tweets for this user
  void addMention(Tweet* t);

  // Returns the private feed (with public and private tweets) in a vector
  std::vector<Tweet*> getMainFeed();

  // Returns the mention feed in a vector
  std::vector<Tweet*> getMentionFeed();

  // Getters and settters for lowlink (for Tarjan's)
  int get_lowlink();

  void set_lowlink(int num);

  // Getters and setters for index (for Tarjan's)
  int get_index();

  void set_index(int num);

  //  Getters and setters for onStack (for Tarjan's)
  bool get_onStack();

  void set_onStack(bool tf);

  void set_password(int password);

  int password();

 private:

  // Converts tweets from list<Tweet*> to vector<Tweet*>
  void push_tweets(std::vector<Tweet*>& feed, 
    std::list<Tweet*> tweet_list);

  // User's string name
  std::string name_;

  // Set of pointers to the followers  
  std::set<User*> followers_;
  
  // Set of pointers to who the user is following
  std::set<User*> following_; 
  
  // List that contains pointers to all of the user's tweets
  std::list<Tweet*> tweets_; 

  // List of private tweets (tweets that start w/ @ that primary tag me or 
  // written by me; will not show up in followers' main feeds
  std::list<Tweet*> private_tweets;

  // List that contains pointers to mentions list
  std::list<Tweet*> mentions_;

  // Lowlink, Index, onStack (for Tarjan's)
  int lowlink;

  int index;

  bool onStack;

  // Password
  int password_;
};

#endif
