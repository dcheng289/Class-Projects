#include "tweet.h"
#include "heap.h"

/*struct Hashtag
{
  string text;
  int val;

  Hashtag(string a, int b) : text(a), val(b) {};
  bool operator == (Hashtag& other)
  {
    return text == other.text;
  }

  void operator *=(int other)
  {
    val++;
  }

};

struct HashtagComp
{
  bool operator()(Hashtag a, Hashtag b)
  {
    return (a.val > b.val);
  }
};*/

Tweet::Tweet()
{

}

// Constructor
Tweet::Tweet(User* user, DateTime& time, std::string& text)
{
  user_ = user;
  userName = user->name();
  timeStamp = time;
  tweetText = text;
  store_hashTags(tweetText);
}

DateTime const & Tweet::time() const
{
  return timeStamp;
}

std::string const & Tweet::text() const
{
  return tweetText;
}

std::vector<std::string> Tweet::hashTags() const
{
  return hashtags;
}

bool Tweet::operator<(const Tweet& other) const
{
  return timeStamp < other.time();
}

std::ostream& operator<<(std::ostream& os, const Tweet& t)
{
  os << t.time() << " " << t.name() << t.text();
  return os;
}

User* Tweet::user() const
{
  return user_;
}

std::string const & Tweet::name() const
{
  return userName;
}

// Member Helper methods

// Stores all the hashtags in this tweet into a private member vector
void Tweet::store_hashTags(std::string tweet)
{
    capitalize(tweet);
    std::istringstream ss(tweet);
    std::string temp;
    getline(ss, temp, '#'); // Get rid of first hashtag
    while (!ss.eof())
    {
        getline(ss, temp, ' ');
        hashtags.push_back(temp);
        //Hashtag h(temp);
        //heap_->push(h);
        getline(ss, temp, '#');
    }

}

// Capitalizes the hashtags so it is case-insensitive
std::string Tweet::capitalize(std::string& input)
{
    for (unsigned int i = 0; i < input.size(); i++)
    {
        if (isupper(input[i]))
        {
            input[i] = tolower(input[i]);
        }
    }
    return input;
}
