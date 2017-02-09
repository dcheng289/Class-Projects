#ifndef TWEET_H
#define TWEET_H
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "datetime.h"
#include "user.h"

/* Forward declaration */
class User;

class Tweet
{
 public:

  // Default Constructor
  Tweet();

  // Constructor
  Tweet(User* user, DateTime& time, std::string& text);

  // Return tweet timestamp
  DateTime const & time() const;

  // Return tweet's text
  std::string const & text() const;

  // Returns vector that contains hashtagged words w/o # sign
  std::vector<std::string> hashTags() const;

  // Returns true if this time is less than other time
  bool operator<(const Tweet& other) const; 

  // Outputs the tweet to the given ostream in format:
  // YYYY-MM-DD HH::MM::SS username tweet_text
  friend std::ostream& operator<<(std::ostream& os, const Tweet& t);

  /* Create any other public or private helper functions you deem 
     necessary */

  User* user() const;

  std::string const &name() const;

  // Stores all the hashtags in this tweet into a hashtags vector
  void store_hashTags(std::string tweet);



 private:
/* Add any other data members or helper functions here  */

  std::string capitalize(std::string& input); // Capitalizes the hashtags
  DateTime timeStamp; // Timestamp for the tweet
  std::string userName; // username string
  std::string tweetText; // tweet text
  std::vector<std::string> hashtags; // vector of all hashtags in this tweet
  User* user_; // the user object who wrote the tweet

};

/***********************************/
/* Leave this alone --             */
/*   it will help sort your tweets */
/***********************************/
struct TweetComp
{
  bool operator()(Tweet* t1, Tweet* t2)
  {
    return (*t2 < *t1);
  }
};
#endif
