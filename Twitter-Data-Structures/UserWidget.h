#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QtGui>
#include <QObject>
#include <QLabel>
#include <QGridLayout>
#include <QTextEdit>
#include <QApplication>
#include <QComboBox>
#include <ctime>
#include <string>
#include "twiteng.h"
#include <sstream>
#include <iostream>

class UserWidget : public QWidget
{
    Q_OBJECT

public: // User* cur_user, 
    UserWidget(TwitEng* twit_ptr)
    {
        ptr = twit_ptr;
        cur_user = NULL;
        
        // Create the layout
        user_layout = new QGridLayout();

        // Add user name to top of their widget 
        user_name = "empty";
        name = new QLabel();
        name->setText(QString::fromStdString(user_name));
        user_layout->addWidget(name, 0, 0);

        // Follow a new user button and input
        QPushButton* addFollow = new QPushButton("Follow a new user");
        user_layout->addWidget(addFollow, 0, 2, 1, 1);
        newFollow = new QLineEdit;
        user_layout->addWidget(newFollow, 0, 3, 1, 1);
        QObject::connect(addFollow, SIGNAL(clicked()), this, SLOT(followClick()
        ));

        // Add a new tweet button and input
        tweetText = new QTextEdit();
        user_layout->addWidget(tweetText, 2, 0, 1, 2);
        addTweet = new QPushButton("Tweet!");
        user_layout->addWidget(addTweet, 1, 0, 1, 1);
        QObject::connect(addTweet, SIGNAL(clicked()), this, SLOT(tweetClick()
        ));

        // Widgets to get the feeds
        main_feed = new QListWidget;
        mention_feed = new QListWidget;
        user_following = new QListWidget;

        // Create the main feed
        QLabel* main_label = new QLabel(QString::fromStdString("Main Feed"));
        user_layout->addWidget(main_label, 3, 0);
        user_layout->addWidget(main_feed, 4, 0); 

        // Create the mention feed
        QLabel* mention_label = new QLabel(QString::fromStdString("Mention Feed"));
        user_layout->addWidget(mention_label, 3, 2);
        user_layout->addWidget(mention_feed, 4, 2, 1 ,2);

        // Create the following list
        QLabel* following_label = new QLabel(QString::fromStdString("Following"));
        user_layout->addWidget(following_label, 1, 2);
        user_layout->addWidget(user_following, 2, 2, 1, 2);

        // Adding feeds to the user layout
        this->setLayout(user_layout);
    };

   ~UserWidget()
   {};

   void updateWidget()
   {
        // Clear old feeds
        main_feed->clear();
        mention_feed->clear();
        user_following->clear();

        // Update the new user pointer
        cur_user = ptr->find_user(user_name);

        // Update name heading
        name->setText(QString::fromStdString(user_name));

        // Update Main Feed
        for (unsigned int i = 0; i < cur_user->getMainFeed().size(); i++)
        {
            Tweet* tweet_buf = cur_user->getMainFeed()[i];
            std::stringstream ss;
            ss << *tweet_buf;
            std::string text_buf = ss.str();
            main_feed->addItem(QString::fromStdString(text_buf));
        }

        // Update Mention Feed
        for (unsigned int i = 0; i < cur_user->getMentionFeed().size(); i++)
        {
            Tweet* tweet_buf = cur_user->getMentionFeed()[i];
            std::stringstream ss;
            ss << *tweet_buf;
            std::string text_buf = ss.str();
            mention_feed->addItem(QString::fromStdString(text_buf));
        }

        // Update the following list
        std::set<User*>::iterator itf;
        std::set<User*> s1 = cur_user->following();
        for (itf = s1.begin(); itf != s1.end(); ++itf)
        {
            std::string buf = (*itf)->name();
            user_following->addItem(QString::fromStdString(buf));
        }

   }

   void set_user(std::string name)
   {
        user_name = name;
   }


public slots:
    void tweetClick()
    {
        // Convert the inputted tweet text to string
        QString qtext = tweetText->toPlainText();
        std::string text = qtext.toStdString();

        // Prepare timestamp
        time_t rawtime;
        struct tm * timeinfo;
        time (&rawtime);
        timeinfo = localtime(&rawtime);
        DateTime dt(timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, 
        (timeinfo->tm_year)+1900, timeinfo->tm_mon+1, timeinfo->tm_mday);

        text.insert(0, " "); // append a space

        // Add to TwitEng
        ptr->addTweet(user_name, dt, text);

        Tweet* new_tweet = ptr->get_tweets().back();

        // Update feeds
        std::stringstream ss;
        ss << *new_tweet;
        std::string text_buf = ss.str();

        updateWidget();
        tweetText->clear();
    };

    void followClick()
    { 
        // Prepare by getting pointers to this user and other user
        std::string text = newFollow->text().toStdString();
        User* this_user = ptr->find_user(user_name);
        User* other_user = ptr->find_user(text);

        // If both users exist, add following connection
        if (this_user != NULL && other_user != NULL)
        {
            this_user->addFollowing(other_user);
            other_user->addFollower(this_user);
        }

        updateWidget();
        newFollow->clear();
    }

    void userChanged()
    {
        cur_user = ptr->find_user(user_name);
        // If null, do nothing
        if (cur_user == NULL)
            return;

        updateWidget();
        this->show();
    }

    void closeButtonClicked()
    {
        this->hide();
    }

private:
    TwitEng* ptr;
    std::string user_name;
    QLabel* name;
    User* cur_user;

    QGridLayout* user_layout;
    QPushButton* addTweet;
    QTextEdit* tweetText;
    QLineEdit* newFollow;

    // Widgets to get feeds
    QListWidget* main_feed;
    QListWidget* mention_feed;
    QListWidget* user_following;
};


#endif

