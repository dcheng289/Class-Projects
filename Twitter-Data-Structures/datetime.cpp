#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "datetime.h"

DateTime::DateTime()
{

}

DateTime::DateTime(int hh, int mm, int ss, int yy, int mon, int dd)
{
   year = yy;
   min = mm;
   day = dd;
   hour = hh;
   month = mon;
   sec = ss;
}

bool DateTime::operator<(const DateTime& other) const
{
  if (year != other.year)
    return year < other.year;

  else if (month != other.month)
    return month < other.month;

  else if (day != other.day)
    return day < other.day;

  else if (hour != other.hour)
    return hour < other.hour;

  else if (month != other.month)
    return month < other.month;

  else if (sec != other.sec)
    return sec < other.sec;
  
  else
    return false; // only should reach this if they are equal
}

std::ostream& operator<<(std::ostream& os, const DateTime& other)
{
  os << other.year << "-";
  os << std::setw(2) << std::setfill('0') << other.month << "-";
  os << std::setw(2) << std::setfill('0') << other.day << " ";
  os << std::setw(2) << std::setfill('0') << other.hour << ":";
  os << std::setw(2) << std::setfill('0') << other.min << ":";
  os << std::setw(2) << std::setfill('0') << other.sec;
  return os;
}

std::istream& operator>>(std::istream& is, DateTime& dt)
{
  std::string buf;
  std::istringstream ss;

  // Storing year
  getline(is, buf, '-');
  ss.str(buf);
  ss >> dt.year;
  ss.clear();

  // Storing month
  getline(is, buf, '-');
  ss.str(buf);
  ss >> dt.month;
  ss.clear();

  // Storing day
  getline(is, buf, ' ');
  ss.str(buf);
  ss >> dt.day;
  ss.clear();

  // Storing hour
  getline(is, buf, ':');
  ss.str(buf);
  ss >> dt.hour;
  ss.clear();

  // Storing min
  getline(is, buf, ':');
  ss.str(buf);
  ss >> dt.min;
  ss.clear();

  // Storing sec
  getline(is, buf, ' ');
  ss.str(buf);
  ss >> dt.sec;
  ss.clear();

  return is;
}