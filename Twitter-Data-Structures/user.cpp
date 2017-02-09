#include <algorithm>
#include "user.h"
#include "msort.h"


User::User(std::string name)
{
  name_ = name;
  lowlink = -1;
  index = -1;
  onStack = false;
}

User::~User()
{
    std::list<Tweet*>::iterator it;
    for (it = tweets_.begin(); it != tweets_.end(); it++)
    {
      delete *it;
    }
}


std::string User::name() const
{
  return name_;
}

std::set<User*> User::followers() const
{
   return followers_;
}

std::set<User*> User::following() const
{
   return following_;
}

std::list<Tweet*> User::tweets() const
{
   return tweets_;
}

void User::addFollower(User* u)
{
   followers_.insert(u);
}
  
void User::addFollowing(User* u)
{
   following_.insert(u);
   std::list<Tweet*>::iterator it;
   for (it = mentions_.begin(); it != mentions_.end(); it++)
   {
     addPrivateTweet(*it);
   }
}

void User::addTweet(Tweet* t)
{
   tweets_.push_back(t);
}

void User::addPrivateTweet(Tweet* t)
{

  User* other = t->user();

  if (other == this) // if I wrote it, then add
  {
    private_tweets.push_back(t);
  }
  else // someone else wrote it
  {
    std::set<User*>::iterator it;
    for (it = following_.begin(); it != following_.end(); it++)
    {
      if (other == *it) // if I follow the original tweeter
      {
        private_tweets.push_back(t); // then add the mention to my private main
        break;                       // no need to keep looping
      }
    }                              
  } 
}

void User::addMention(Tweet* t)
{
   mentions_.push_back(t);
}

std::vector<Tweet*> User::getMainFeed() // public AND private tweets
{
   std::vector<Tweet*> feed;
   std::set<User*>::iterator its; 

   // Push user's normal and private tweets in
   push_tweets(feed, tweets_);
   push_tweets(feed, private_tweets);


   // Push user's following tweets in
   for (its = following_.begin(); its != following_.end(); its++)
   {
      push_tweets(feed, (*its)->tweets() );
   }  

   // Sorting the main feed w/ mergeSort
   mergeSort(feed, TweetComp());

   return feed;
}

std::vector<Tweet*> User::getMentionFeed()
{
   std::vector<Tweet*> feed;
   //  Push and sort the mentions feed
   push_tweets(feed, mentions_);

   mergeSort(feed, TweetComp());

   return feed;
}

// Getters and settes for lowlink (for Tarjan's)
int User::get_lowlink()
{
  return lowlink;
}

void User::set_lowlink(int num)
{
  lowlink = num;
}

// Getters and setters for index (for Tarjan's)
int User::get_index()
{
  return index;
}

void User::set_index(int num)
{
  index = num;
}

// Getters and setters for onStack (for Tarjan's)
bool User::get_onStack()
{
  return onStack;
}

void User::set_onStack(bool tf)
{
  onStack = tf;
}

// Helper method
  // Converts tweet_list to a vector

void User::push_tweets(std::vector<Tweet*>& feed, std::list<Tweet*> tweet_list)
{
   std::list<Tweet*>::iterator it;
   for (it = tweet_list.begin(); it != tweet_list.end(); it++)
   {
      feed.push_back(*it);
   }
}

void User::set_password(int password)
{
  password_ = password;
}

int User::password()
{
  return password_;
}