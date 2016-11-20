#include "Date.hpp"
#include <iomanip>
#include <sstream>

Date::Date() : m_year(0), m_month(0), m_day(0), m_hour(0), m_minute(0) {};

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
  m_year = t_year;
  m_month = t_month;
  m_day = t_day;
  m_hour = t_hour;
  m_minute = t_minute;
}

Date::Date(std::string t_dateString) {
  m_year = 0;
  m_month = 0;
  m_day = 0;
  m_hour = 0;
  m_minute = 0;
  bool flag = 1;
  if (t_dateString.length() != 16)
    flag = 0;
  if (t_dateString[4] != '-' || t_dateString[7] != '-'
    || t_dateString[10] != '/' ||
    t_dateString[13] != ':')
    flag = 0;
  for (int i = 0; i < 16; ++i) {
    if(i == 4 || i == 7 || i == 10 || i == 13) {
      continue;
    }
    if(t_dateString[i] < '0' || t_dateString[i] > '9') 
      flag = 0;
  }
  if (flag) {
    m_year = (t_dateString[0] - '0') * 1000 + (t_dateString[1] - '0') * 100 +
                (t_dateString[2] - '0') * 10 + (t_dateString[3] - '0');
    m_month = (t_dateString[5] - '0') * 10 + (t_dateString[6] - '0');
    m_day = (t_dateString[8] - '0') * 10 + (t_dateString[9] - '0');
    m_hour = (t_dateString[11] - '0') * 10 + (t_dateString[12] - '0');
    m_minute = (t_dateString[14] - '0') * 10 + (t_dateString[15] - '0');
  }
}

int Date::getYear(void) const {
  return m_year;
}

void Date::setYear(const int t_year) {
  m_year = t_year;
}

int Date::getMonth(void) const {
  return m_month;
}

void Date::setMonth(const int t_month) {
  m_month = t_month;
}

int Date::getDay(void) const {
  return m_day;
}

void Date::setDay(const int t_day) {
  m_day = t_day;
}

int Date::getHour(void) const {
  return m_hour;
}

void Date::setHour(const int t_hour) {
  m_hour = t_hour;
}

int Date::getMinute(void) const {
  return m_minute;
}

void Date::setMinute(const int t_minute) {
  m_minute = t_minute;
}

bool Date::isValid(const Date t_date) {
  int year = t_date.getYear();
  int month = t_date.getMonth();
  int day = t_date.getDay();
  int hour = t_date.getHour();
  int minute = t_date.getMinute();
  if(t_date.m_year < 1000 || year > 9999 || month < 1 || day < 1 || minute < 0 || hour < 0)
    return false;
  if (month > 12 || hour > 23 || minute > 59)
    return false;
  if (month == 2) {
    if (year % 400 != 0 && (year % 4 != 0 || year % 100 == 0))
      if (day > 28)
        return false;
    if (day > 29)
      return false;
  }
  if (month == 4 || month == 6 || month == 9 || month == 11)
    if (day > 30)
      return false;
  if (day > 31)
    return false;
  return true;
}

Date Date::stringToDate(const std::string t_dateString) {
  return Date(t_dateString);
}

std::string Date::dateToString(Date t_date) {
  if (!isValid(t_date))
    return "0000-00-00/00:00";

  std::stringstream ss;
  ss << t_date.m_year << ((t_date.m_month > 9)?"-":"-0") << t_date.m_month << ((t_date.m_day > 9)?"-":"-0")
    << t_date.m_day << ((t_date.m_hour > 9)?"/":"/0") << t_date.m_hour << ((t_date.m_minute > 9)?":":":0")
    << t_date.m_minute;
  std::string str = ss.str();
  return str;
}

Date &Date::operator=(const Date &t_date) {
  if (this != &t_date) {
      this -> setYear(t_date.m_year);
      this -> setMonth(t_date.m_month);
      this -> setDay(t_date.m_day);
      this -> setMinute(t_date.m_minute);
      this -> setHour(t_date.m_hour);
  }
  return *this;
}

bool Date::operator==(const Date &t_date) const {
  if (m_year == t_date.m_year && m_month == t_date.m_month && m_day ==
      t_date.m_day && m_hour == t_date.m_hour && m_minute == t_date.m_minute)
    return true;
  return false;
}

bool Date::operator>(const Date &t_date) const {
  if (m_year < t_date.m_year)
    return false;
  if (m_year > t_date.m_year)
    return true;
  if (m_month < t_date.m_month)
    return false;
  if (m_month > t_date.m_month)
    return true;
  if (m_day < t_date.m_day)
    return false;
  if (m_day > t_date.m_day)
    return true;
  if (m_hour < t_date.m_hour)
    return false;
  if (m_hour > t_date.m_hour)
    return true;
  if (m_minute < t_date.m_minute)
    return false;
  if (m_minute > t_date.m_minute)
    return true;
  return false;
}


bool Date::operator<(const Date &t_date) const {
  return !(*this > t_date || *this == t_date);
}

bool Date::operator>=(const Date &t_date) const {
  return (*this > t_date || *this == t_date);
}

bool Date::operator<=(const Date &t_date) const {
  return !(*this > t_date);
}
