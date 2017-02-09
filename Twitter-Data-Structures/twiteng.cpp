#include <algorithm>
#include <iostream>
#include "twiteng.h"
#include "heap.h"
#include <string>

TwitEng::TwitEng()
{
    heap_ = new Heap<std::string, HashtagComp>(2, HashtagComp());
}

TwitEng::~TwitEng()
{
    // Parse thru user database, delete each user object
    std::set<User*>::iterator it;
    for (it = user_database.begin(); it != user_database.end(); it++)
    {
        delete *it;
    }
}

bool TwitEng::parse(char* filename)
{
    // Open the file
    std::ifstream fin(filename);
    if (fin.fail() )
        return true;

    // Prepare to parse
    std::string buffer;
    getline(fin, buffer);
    std::istringstream s1(buffer);
    int num_users;

    // Parse in the num_users
    s1 >> num_users;
    if (s1.fail() )
        return true;

    // Parse in users and who they follow
    std::string user1;
    for (int i = 0; i < num_users; i++)
    {
        getline(fin, buffer);
        std::istringstream s2(buffer);
        s2 >> user1;

        // Store user as User* u1
        User* u1;
        if (find_user(user1) == NULL) // if user doesn't exist, create user
            u1 = new User(user1);
        else
            u1 = find_user(user1); // else, just point to the user

        int password;
        s2 >> password;
        u1->set_password(password);

        while(s2 >> buffer)
        {  
                User* u2; // Pointer to the follower
                // if follower doesn't exist, create folloser
                if (find_user(buffer) == NULL) 
                {
                    u2 = new User(buffer);
                    user_database.insert(u2);
                }
                // else, just point to the follower
                else
                {
                    u2 = find_user(buffer);
                }

                u1->addFollowing(u2);
                u2->addFollower(u1);
        }
        // Insert this completed user object in the user database
        user_database.insert(u1);
    }

    // Parse tweets
    while(!fin.eof() )
    {
        std::string userName, tweetText;
        if (!fin.fail() )
        {
            DateTime timeStamp;
            
            // Parse time, username, tweet
            fin >> timeStamp;
            fin >> userName;
            getline(fin, tweetText);

            // Add the tweet to the user
            addTweet(userName, timeStamp, tweetText);
        }
    }
    return false;
}

void TwitEng::addTweet(std::string& username, DateTime& time, std::string& text)
{
    // Go into the user_database set and find the User* for the username
    User* u = find_user(username);

    if (u == NULL) 
    {
        return; // If user doesn't exist, quit
    }
    
    // Create a tweet object
    Tweet* t = new Tweet(u, time, text);

    // Add the tweet to the User* within the user_database set

    if (text[1] == '@') // Adding a private tweet
    {
        u->addPrivateTweet(t);
        addMentions(t, true);
    }
    else // Adding a normal tweet
    {
        u->addTweet(t);
        addMentions(t, false);
    }

    // Add to the hashtag database
    for (unsigned int i = 0; i < t->hashTags().size(); i++)
    {
        std::map<std::string, std::set<Tweet*> >::iterator it;
        it = hashtag_database.find(t->hashTags()[i]);
        heap_->push(t->hashTags()[i]);
        //std::cout << "adding hashtag " << t->hashTags()[i];
        if (it == hashtag_database.end( )) // hashtag doesn't exist, insert pair
        {
            std::set<Tweet*> set1;
            set1.insert(t);
            hashtag_database.insert(std::pair<std::string,std::set<Tweet*> >
                (t->hashTags()[i], set1));
        }
        else // add tweet to existing <hashtag, set_tweet> pair
        {
            it->second.insert(t);
        }
    }

    all_tweets.push_back(t);
}


std::vector<Tweet*> TwitEng::search(std::vector<std::string>& terms, int strategy)
{

    std::map<std::string, std::set<Tweet*> >::iterator it;
    std::set<Tweet*>::iterator its;
    std::vector<Tweet*> search;
    if (terms.size() == 0)
        return search;
    else if (terms.size() == 1)
    {
        it = hashtag_database.find(terms[0]);
        if (it != hashtag_database.end())
        {
            for (its = it->second.begin(); its != it->second.end(); its++)
            {
                search.push_back(*its);
            }
        }
        return search;
    }

    if (strategy == 1) // OR strategy
    {
        std::set<Tweet*> union_;
        it = hashtag_database.find(terms[0]);
        std::set<Tweet*> temp;
        if (it != hashtag_database.end()) // checking that hashtag exists
            temp = it->second; // set the initial set equal to first hashtag set

        //union_ = temp;
        // Loop thru the rest of hashtags, and union with the existing set
        for (unsigned int i = 1; i < terms.size(); i++)
        {
            it = hashtag_database.find(terms[i]);
            if (it != hashtag_database.end()) // checking that hashtag exists
            {
                set_union(temp.begin(),temp.end(),it->second.begin(),
                    it->second.end(), std::inserter(union_,union_.begin()));
                temp = union_; // update temp
            }
            else
                union_ = temp; // do nothing
        }

        // Convert set into vector to be returned
        for (its = union_.begin(); its != union_.end(); its++)
        {
            search.push_back(*its);
        }
    }

    else if (strategy == 0) // AND strategy
    {
        std::set<Tweet*> intersect;
        it = hashtag_database.find(terms[0]);
        std::set<Tweet*> temp;
        if (it != hashtag_database.end()) // checking that hashtag exists
            temp = it->second; // set the initial set to first hashtag set

        // Loop thru remaining hashtags, and intersect with existing set
        for (unsigned int i = 1; i < terms.size(); i++)
        {
            it = hashtag_database.find(terms[i]);
            if (it != hashtag_database.end()) // checking that hashtag exists
            {
                set_intersection(temp.begin(),temp.end(),it->second.begin(),
                    it->second.end(), std::inserter(intersect,intersect.begin()));
            }
            temp = intersect;
        }

        // Conver set into vector to be returned
        for (its = intersect.begin(); its != intersect.end(); its++)
        {
            search.push_back(*its);
        }
    }
    return search;
}


void TwitEng::addMentions(Tweet* t, bool private_mention)
{
   // Handle adding normal mentions here
  std::set<std::string> mentioned_users;
  std::istringstream ss(t->text());
  std::string buf;

  while (ss >> buf)
  {
    if (buf[0] == '@')
    {
        buf.erase(0,1); // Erase the first character 
        if (find_user(buf) != NULL)
        {
            find_user(buf)->addMention(t);
        }
    }

    // If the tweet is private, add to mentionee feeds and stop parsing
    if (private_mention == true)
    {
        if (find_user(buf) != NULL)
        {
            find_user(buf)->addPrivateTweet(t);
            break; // No one else can see this tweet except for mentionee
        }
    }
  }
}

User* TwitEng::find_user(std::string userName)
{
    // Parse thru user database and return the pointer to the user object
    // that matches the userName string
    std::set<User*>::iterator it;
    for (it = user_database.begin(); it != user_database.end(); it++)
    {
        if (userName == (*it)->name() )
        {
            return *it;
        }
    }

    return NULL;
}

std::vector<Tweet*>& TwitEng::get_tweets()
{
    return all_tweets;
}

std::set<User*>& TwitEng::database()
{
    return user_database;
}

void TwitEng::Tarjan(std::string filename)
{
    cc_index = 0;

    std::set<User*>::iterator it;

    // Making connections
    for (it = user_database.begin(); it != user_database.end(); it++)
    {
        if ((*it)->get_index() == -1)
        {
            strongConnect(*it);
        }
    }

    // Printing it out
    std::ofstream fout(filename.c_str());

    for (unsigned int i = 0; i < connect_database.size(); i++)
    {
        fout << "Component " << i+1 << ": " << "\n";
        std::set<User*> temp = connect_database[i];
        std::set<User*>::iterator its;

        for (its = temp.begin(); its != temp.end(); its++)
        {
            fout << (*its)->name() << "\n";
        }
        fout << "\n";
    }
    fout.close();
}

void TwitEng::strongConnect(User* u)
{
    u->set_index(cc_index);
    u->set_lowlink(cc_index);
    cc_index++;
    cc_stack.push(u);
    u->set_onStack(true);

    std::set<User*> temp = u->following();
    for (std::set<User*>::iterator it = temp.begin(); it != temp.end(); it++)
    {
        if ((*it)->get_index() == -1)
        {
            strongConnect(*it);
            int num = std::min(u->get_lowlink(), (*it)->get_lowlink());
            u->set_lowlink(num);
        }
        else if ((*it)->get_onStack()) 
        {
            int num = std::min(u->get_lowlink(), (*it)->get_index());
            u->set_lowlink(num);
        }
    }

    if (u->get_lowlink() == u->get_index())
    {
        std::set<User*> connected_component;

        while (u != cc_stack.top())
        {
            connected_component.insert(cc_stack.top());
            cc_stack.top()->set_onStack(false);
            cc_stack.pop();
        }
        connected_component.insert(cc_stack.top());
        cc_stack.top()->set_onStack(false);
        cc_stack.pop();
        connect_database.push_back(connected_component);
    }
}

void TwitEng::trending()
{
    trending_hashtags.clear();
    trending_quantity.clear();
    int index = heap_->heapVectorSize();
    if (index > 5)
        index = 5;

    for (int i = 0; i < index; i++)
    {
        trending_hashtags.push_back(heap_->top());
        trending_quantity.push_back(heap_->topPriority());
        heap_->pop();
    }

    for (int i = 0; i < index; i++)
    {
        for (int j = 0; j < trending_quantity[i]; j++)
        {
            heap_->push(trending_hashtags[i]);
        }
    }
}

std::vector<std::string>& TwitEng::get_trendingHashtags()
{
    return trending_hashtags;
}

std::vector<int>& TwitEng::get_trendingQuantity()
{
    return trending_quantity;
}

User* TwitEng::addUser(std::string username, int password)
{
    User* u = new User(username);  
    u->set_password(password);
    user_database.insert(u); 
    return u;
}