#ifndef TWITSEARCH_H
#define TWITSEARCH_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <string>
#include <sstream>
#include <vector>
#include "twiteng.h"
#include "tweet.h"

class TwitSearch : public QWidget
{
  Q_OBJECT
public:
  TwitSearch(TwitEng* twit_ptr)
  {
    ptr = twit_ptr;

    // Set up initial layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainButton = new QPushButton("&Search");
    mainLayout->addWidget(mainButton);
    setLayout(mainLayout);
    QVBoxLayout* otherLayout = new QVBoxLayout;
    otherWin = new QWidget;

    andSearch = new QPushButton("And Search");
    orSearch = new QPushButton("Or Search");
    otherLayout->addWidget(andSearch);
    otherLayout->addWidget(orSearch);

    searchText = new QLineEdit();
    otherLayout->addWidget(searchText);

    displayResults = new QListWidget();
    otherLayout->addWidget(displayResults);

    closeButton = new QPushButton("&Close");
    otherLayout->addWidget(closeButton);
    otherWin->setLayout(otherLayout);
    QObject::connect(mainButton, SIGNAL(clicked()), this, SLOT(mainButtonClicked()
    ));
    QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()
    ));

    QObject::connect(andSearch, SIGNAL(clicked()), this, SLOT(andClick()
    ));
    
    QObject::connect(orSearch, SIGNAL(clicked()), this, SLOT(orClick()
    ));
  };

public slots:
  void mainButtonClicked()
  {
    otherWin->show();
  };

  void closeButtonClicked()
  {
    otherWin->hide();
  };

  void andClick()
  {
    displayResults->clear();
    std::string text = searchText->text().toStdString();
    capitalize(text);
    std::vector<std::string> hashtags = convert_to_vector(text);
    std::vector<Tweet*> hits = ptr->search(hashtags, 0);
    for (unsigned int i = 0; i < hits.size(); i++)
    {
        std::stringstream ss;
        ss << *hits[i];
        QString buf = QString::fromStdString(ss.str());
        displayResults->addItem(buf);
    }
  };

  void orClick()
  {
    displayResults->clear();
    std::string text = searchText->text().toStdString();
    capitalize(text);
    std::vector<std::string> hashtags = convert_to_vector(text);
    std::vector<Tweet*> hits = ptr->search(hashtags, 1);
    for (unsigned int i = 0; i < hits.size(); i++)
    {
        std::stringstream ss;
        ss << *hits[i];
        QString buf = QString::fromStdString(ss.str());
        displayResults->addItem(buf);
    }
  };
private:
  QPushButton* mainButton;
  QWidget* otherWin;
  QPushButton* closeButton;
  QPushButton* andSearch;
  QPushButton* orSearch;
  QLineEdit* searchText;
  QListWidget* displayResults;
  TwitEng* ptr;

    std::string capitalize(std::string& input)
    {
        for (unsigned int i = 0; i < input.size(); i++)
        {
            if (isupper(input[i]))
            {
                input[i] = tolower(input[i]);
            }
        }
        return input;
    };

    std::vector<std::string> convert_to_vector(std::string text)
    {
        std::istringstream is;
        is.str(text);
        std::string buf;
        std::vector<std::string> hashtags;
        while( is >> buf )
        {
            hashtags.push_back(buf);
        }
        return hashtags;
    };
};
#endif