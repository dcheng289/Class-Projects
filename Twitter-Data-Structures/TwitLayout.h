#ifndef TWITLAYOUT_H
#define TWITLAYOUT_H

#include <iostream>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <string>
#include <QPushButton>
#include <QObject>
#include <QApplication>
#include <QFont>
#include "TwitSearch.h"
#include "Trending.h"
#include "twiteng.h"
#include "UserWidget.h"

class TwitLayout : public QGridLayout
{
    Q_OBJECT

public:
    TwitLayout(QApplication* app, TwitEng* twit_ptr, UserWidget* user_widget)
    {
        // MainWin Widget
        mainWin = new QWidget;
        layout = new QGridLayout;

        // Set text and font for menu
        ptr = twit_ptr;
        u_ptr = user_widget;
        menuLabel = new QLabel(QString::fromStdString("Menu"));
        menuLabel->setAlignment(Qt::AlignCenter);

        QFont f("Arial", 16, QFont::Bold);
        menuLabel->setFont(f);
        layout->addWidget(menuLabel, 0, 0, 1, 4);

        // And/Or search button
        searchButton = new TwitSearch(twit_ptr);
        layout->addWidget(searchButton, 1, 0, 1, 2);

        // Save data button
        saveButton = new QPushButton("Save User Data");
        layout->addWidget(saveButton, 2, 0, 1, 2);
        QObject::connect(saveButton, SIGNAL(clicked()), this, SLOT( Save() ) );

        // Save filename input
        fileName = new QLineEdit();
        layout->addWidget(fileName, 2, 2, 1, 20);

        // Save strongly connected components button
        scc_saveButton = new QPushButton("Save Strongly Connected Components");
        layout->addWidget(scc_saveButton, 3, 0, 1, 2);
        QObject::connect(scc_saveButton, SIGNAL(clicked()), 
            this, SLOT( scc_Save() ) );

        // Save SCC filename input
        scc_fileName = new QLineEdit();
        layout->addWidget(scc_fileName, 3, 2, 1, 20);

        trendingButton = new Trending(twit_ptr);
        layout->addWidget(trendingButton, 0, 2, 1, 20);
        //QObject::connect(trendingButton, SIGNAL(clicked()), 
          //  this, SLOT( trending() ) );

        // show tweets
        showTweets = new QPushButton("Show Tweets");
        layout->addWidget(showTweets, 1, 2, 1, 20);
        connect(showTweets, SIGNAL(clicked()), u_ptr, 
                SLOT(userChanged() )  );


        // Logout button
        logoutButton = new QPushButton("Logout");
        layout->addWidget(logoutButton, 4, 0, 1, 20);
        QApplication::connect(logoutButton, SIGNAL(clicked()), this, SLOT(logout() ) );  
        QApplication::connect(logoutButton, SIGNAL(clicked()), 
            searchButton, SLOT(closeButtonClicked()) );  
        QApplication::connect(logoutButton, SIGNAL(clicked()), 
            trendingButton, SLOT(closeButtonClicked()) );  
        QApplication::connect(logoutButton, SIGNAL(clicked()), 
            u_ptr, SLOT(closeButtonClicked()) );  

        mainWin->setLayout(layout);  





        // login widget
        loginWin = new QWidget;
        login_layout = new QGridLayout;

        // username
        username = new QLineEdit();
        login_layout->addWidget(username, 0, 1, 1, 2);
        userLabel = new QLabel(QString::fromStdString("Username"));
        login_layout->addWidget(userLabel, 0, 0, 1, 1);
        
        // password
        password = new QLineEdit();
        login_layout->addWidget(password, 1, 1, 1, 2);
        passwordLabel = new QLabel(QString::fromStdString("Password"));
        login_layout->addWidget(passwordLabel, 1, 0, 1, 1);

        // login button
        loginButton = new QPushButton("Login");
        login_layout->addWidget(loginButton, 2, 0, 1, 3);
        QObject::connect(loginButton, SIGNAL(clicked()), this, SLOT( login() ) );

        // register button
        registerButton = new QPushButton("Register");
        login_layout->addWidget(registerButton, 3, 0, 1, 3);
        QObject::connect(registerButton, SIGNAL(clicked()), this, SLOT( registerClick() ) );

        // Quit button
        quitButton = new QPushButton("QUIT");
        login_layout->addWidget(quitButton, 4, 0, 1, 3);
        QApplication::connect(quitButton, SIGNAL(clicked()), app, SLOT(quit() ) ); 

        loginWin->setLayout(login_layout);

        loginWin->show();
    };

   ~TwitLayout()
   {};

    void push_back(User* user)
    {
        user_widgets.push_back(user);
    };

    int hash(std::string password)
    {
        long long x = 0;

        for (unsigned i = 0; i < password.size(); ++i)
        {
            long long temp = password[i];
            long long exponent = pow(128, password.size()-i-1);
            x += (temp*exponent);
        }

        std::vector<int> y;
        int junk = 0;
        for (int i = 0; i < 4; ++i)
        {
            if (x == 0)
                junk++;
            y.push_back(x % 65521);
            x = x / 65521;

        }

        int w[4];
    
        for (int i = 3; i > -1; i--)
        {
            w[3-i] = y[i];
        }

        int p_num = (unsigned)(45912 * w[0] + 35511 * w[1] + 65169 * w[2] + 4625 * w[3]) % 65521;

        return p_num;
    };

public slots:
    void Save()
    {
        // Get user-inputted file name
        std::string file_name = fileName->text().toStdString();
        std::ofstream fout(file_name.c_str());

        // Print out users and who they follow
        fout << ptr->database().size() << "\n";
        std::set<User*>::iterator it;
        std::set<User*> set = ptr->database();
        for (it = set.begin(); it != set.end(); it++)
        {
            fout << (*it)->name() << " ";
            fout << (*it)->password() << " ";
            std::set<User*> s1 = (*it)->following();
            for (std::set<User*>::iterator itf = s1.begin(); itf != s1.end(); itf++)
            {
                fout << (*itf)->name() << " ";
            }
            fout << "\n";
        }

        // Print out all of the tweets
        for (unsigned int i = 0; i < ptr->get_tweets().size(); i++)
        {
            fout << *(ptr->get_tweets()[i]) << "\n";
        }
        fout.close();

        QMessageBox msgBox;
        msgBox.setText("User data saved to " + fileName->text());
        msgBox.exec();
    };
    
    void scc_Save()
    {
        ptr->Tarjan(scc_fileName->text().toStdString());
        QMessageBox msgBox;
        msgBox.setText("Strongly Connected Components saved to " 
            + scc_fileName->text());
        msgBox.exec();
    };

    void logout()
    {
        mainWin->hide();
        loginWin->show();
    };


    void login()
    {
        std::string name = username->text().toStdString();

        std::string temp = password->text().toStdString();
        int p_num;

        p_num = hash(temp);

        int exists = false;
        for (unsigned i = 0; i < user_widgets.size(); i++)
        {
            if (name == user_widgets[i]->name() && p_num == user_widgets[i]->password())
            {
                exists = true;
                break;
            }
        }

        if (exists)
        {
            mainWin->show();
            loginWin->hide();
            u_ptr->set_user(name);
            //userChanged(username->text());
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Username and/or password incorrect!");
            msgBox.exec();
        }

    };

    void registerClick()
    {  
        std::string name = username->text().toStdString();
        std::string pword = password->text().toStdString();
        int p_num = hash(pword);
        int exists = false;
        for (unsigned i = 0; i < user_widgets.size(); i++)
        {
            if (name == user_widgets[i]->name())
            {
                exists = true;
                break;
            }
        }

        if (!exists)
        {
            User* u = ptr->addUser(name, p_num);
            push_back(u);
            login();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("Username already exists!");
            msgBox.exec();
        }
    };



private:
    TwitEng* ptr; // ptr to TwitEng object
    UserWidget* u_ptr;

    QGridLayout* layout;
    QWidget* mainWin;
    QLabel* menuLabel;
    TwitSearch* searchButton;
    QPushButton* saveButton;
    QLineEdit* fileName;
    QPushButton* scc_saveButton;
    QLineEdit* scc_fileName;
    QPushButton* logoutButton;
    QPushButton* showTweets;
    Trending* trendingButton;

    QGridLayout* login_layout;
    QWidget* loginWin;
    QLineEdit* username;
    QLabel* userLabel;
    QLineEdit* password;
    QLabel* passwordLabel;
    QPushButton* loginButton;
    QPushButton* registerButton;
    QPushButton* quitButton;

    std::vector<User*> user_widgets;

};

#endif
