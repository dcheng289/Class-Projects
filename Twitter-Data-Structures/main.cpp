#include <QObject>
#include <QApplication>
#include <QComboBox>
#include "TwitLayout.h"
#include "UserWidget.h"
#include "TwitSearch.h"
#include "tweet.h"
#include "user.h"
#include "twiteng.h"
#include "datetime.h"

using namespace std;

int main(int argc, char* argv[])
{
  list<int> mylist;
  if(argc < 2){
    cerr << "Please provide the twitter data file" << endl;
    return 1;
  }
  TwitEng twit;

  if ( twit.parse(argv[1]) ){
    cerr << "Unable to parse " << argv[1] << endl;
    return 1;
  }


  QApplication app(argc, argv);
  QWidget window;

  UserWidget* user = new UserWidget(&twit);
  TwitLayout* layout = new TwitLayout(&app, &twit, user);

  //TwitComboBox* scrollBox = new TwitComboBox(user, &twit);
  std::set<User*>::iterator it;
  //scrollBox->addItem(QString::fromStdString("Select User..."));

  std::set<User*> set = twit.database();
  for (it = set.begin(); it != set.end(); it++)
  {
//    UserWidget* user = new UserWidget(it, &twit);

    // Add user name to the main menu scrollbox
    std::string str = (*it)->name();
    QString qstr = QString::fromStdString(str);
    //scrollBox->addItem(qstr);

    // Keep track of the user pointer
    layout->push_back(*it);

  }

  // Adds the completed user scroll box
  //layout->addWidget(scrollBox, 1, 2, 1, 20);

  //QComboBox::connect(scrollBox, SIGNAL(currentIndexChanged(int)), 
    //            scrollBox, SLOT(selectionChanged() ) );


  // Set and show main menu interface
  //window.setLayout(layout);
  //window.show();
  
  return app.exec();
}
