#ifndef TWITENG_H
#define TWITENG_H
#include <map>
#include "user.h"
#include "datetime.h"
#include "heap.h"
#include <stack>

struct HashtagComp
{
  bool operator()(Hashtag<std::string> a, Hashtag<std::string> b)
  {
    return (a.p >= b.p);
  }
};

class TwitEng
{
 public:
  TwitEng();
  ~TwitEng();

  // Parses the Twitter database and populates internal structures
  bool parse(char* filename);


  // Allocates a tweet from its parts and adds it to internal structures
  void addTweet(std::string& username, DateTime& time, std::string& text);


  // Searches for tweets with the given words and strategy
  std::vector<Tweet*> search(std::vector<std::string>& terms, int strategy);

  // Finds mentions in a tweet and adds them to the users' mentions feeds
  void addMentions(Tweet* t, bool private_mention);

  // Helper method that takes in string username and returns User* to the user
  User* find_user(std::string userName);

  // Returns the private member all_tweets
  std::vector<Tweet*>& get_tweets();

  // Returns the private memeber user_database set
  std::set<User*>& database();

  // Tarjan's Algorithm for Strongly Connected Components
  void Tarjan(std::string filename);

  // Tarjan Helper
  void strongConnect(User* u);

  // update heap
  void trending();

  // returns trending 5
  std::vector<std::string>& get_trendingHashtags();

  // returns trending quantities
  std::vector<int>& get_trendingQuantity();

  // add user
  User* addUser(std::string username, int password);

  /* You may add other member functions */
 private:
  /* Add any other data members or helper functions here  */
  
  // Set of all existing User* pointers
  std::set<User*> user_database;

  // Vector that contains, in Tweet* form, all of the tweets
  std::vector<Tweet*> all_tweets; 

  // Map of <hashtags, set<Tweets>> that organizes hashtags
  std::map<std::string, std::set<Tweet*> > hashtag_database;

  // Set, Stack, and Index for Tarjan's
  std::set<User*> cc_set;
  std::stack<User*> cc_stack;
  int cc_index;
  std::vector<std::set<User*> > connect_database;

  // Heap
  Heap<std::string, HashtagComp>* heap_;  
  std::vector<std::string> trending_hashtags;
  std::vector<int> trending_quantity;

};


#endif
