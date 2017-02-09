#ifndef DATETIME_H
#define DATETIME_H
#include <iostream>

/**
 * Models a timestamp in format YYYY-MM-DD HH:MM:SS 
 */
struct DateTime
{

  // Default constructor for current system time/date
  DateTime();

  // Alternative constructor 
  DateTime(int hh, int mm, int ss, int yy, int mon, int dd);

  // Return true if the timestamp is less-than other's
  bool operator<(const DateTime& other) const;


  // Outputs the timestamp to the given ostream in format:
  //   YYYY-MM-DD HH::MM::SS
  friend std::ostream& operator<<(std::ostream& os, const DateTime& other);


  // Inputs the timestamp to the given istream expecting the format:
  //   YYYY-MM-DD HH::MM::SS
  friend std::istream& operator>>(std::istream& is, DateTime& dt);

  /* Data members -- they can all be public 
   * which is why we've made this a struct */
  int hour;
  int min;
  int sec;
  int year;
  int month;
  int day;
};

#endif
