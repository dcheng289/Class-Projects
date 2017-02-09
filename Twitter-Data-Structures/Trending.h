#ifndef TRENDING_H
#define TRENDING_H

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

class Trending : public QWidget
{
  Q_OBJECT
public:
  Trending(TwitEng* twit_ptr)
  {
    ptr = twit_ptr;
    trendingFeed = new QListWidget;
    // Set up initial layout
    QVBoxLayout* trendLayout = new QVBoxLayout;
    trendButton = new QPushButton("Trending Topics");
    trendLayout->addWidget(trendButton);
    setLayout(trendLayout);

    otherLayout = new QVBoxLayout;
    otherWin = new QWidget;

    trendingTitle = new QLabel();
    trendingTitle->setText(QString::fromStdString("Trending"));
    trendingTitle->setAlignment(Qt::AlignCenter);
    QFont f("Arial", 12, QFont::Bold);
    trendingTitle->setFont(f);
    otherLayout->addWidget(trendingTitle);

    refreshButton = new QPushButton("Refresh");
    otherLayout->addWidget(refreshButton);
    trendingFeed = new QListWidget();
    otherLayout->addWidget(trendingFeed);
    closeButton = new QPushButton("Close");
    otherLayout->addWidget(closeButton);

    otherWin->setLayout(otherLayout);

    QObject::connect(trendButton, SIGNAL(clicked()), this, SLOT(trendButtonClicked()
    ));

    QObject::connect(trendButton, SIGNAL(clicked()), this, SLOT(refreshClick()
    ));

    QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(closeButtonClicked()
    ));

    QObject::connect(refreshButton, SIGNAL(clicked()), this, SLOT(refreshClick()
    ));
  }

public slots:
  void trendButtonClicked()
  {
    ptr->trending();
    otherWin->show();
  };

  void closeButtonClicked()
  {
    otherWin->hide();
  };

  void refreshClick()
  {
    ptr->trending();
    trendingFeed->clear();
    for (unsigned i = 0; i < ptr->get_trendingHashtags().size(); ++i)
    {
        std::string temp = ptr->get_trendingHashtags()[i];
        temp += "\t";

        std::stringstream ss;
        ss << ptr->get_trendingQuantity()[i];
        std::string str = ss.str();

        temp += str;

        trendingFeed->addItem(QString::fromStdString(temp));    
    }

  };


private:
  QPushButton* trendButton;
  QPushButton* closeButton;
  QWidget* otherWin;
  QPushButton* refreshButton;
  QLabel* trendingTitle;
  TwitEng* ptr;
  QListWidget* trendingFeed;
  QVBoxLayout* otherLayout;


};
#endif