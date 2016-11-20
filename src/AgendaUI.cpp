#include "AgendaUI.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::left;
using std::setw;
using std::list;

AgendaUI::AgendaUI() {
	m_userName = "";
  m_userPassword = "";
  startAgenda();
}

void AgendaUI::OperationLoop(void) {
  while (true) {
    for (int i = 0; i < 36; ++i)
      cout << '-';
    cout << " Agenda ";
    for (int i = 0; i < 36; ++i)
      cout << '-';
    cout << endl;
    cout << "Action : " << endl;
    if (m_userName == "") {
      cout << "l   - log in Agenda by user name and password" << endl;
      cout << "r   - register an Agenda account" << endl;
      cout << "q   - quit Agenda" << endl;
    } else {
      cout << "o   - log out Agenda" << endl;
      cout << "dc  - delete Agenda account" << endl;
      cout << "lu  - list all Agenda user" << endl;
      cout << "cm  - create a meeting" << endl;
      cout << "la  - list all meetings" << endl;
      cout << "las - list all sponsor meetings" << endl;
      cout << "lap - list all participate meetings" << endl;
      cout << "qm  - query meeting by title" << endl;
      cout << "qt  - query meeting by time interval" << endl;
      cout << "dm  - delete meeting by title" << endl;
      cout << "da  - delete all meetings" << endl;
    }
    for (int i = 0; i < 80; ++i)
      cout << '-';
    if (m_userName == "")
      cout << "Agenda : ~$ ";
    else
      cout << "Agenda : # ";
    if (!executeOperation(getOperation()))
      break;
  }
}

void AgendaUI::startAgenda(void) {
  system("clear");
}

std::string AgendaUI::getOperation() {
  string s;
  cin >> s;
  return s;
}

bool AgendaUI::executeOperation(string t_operation) {
  bool logined = false;
  if (m_userName != "")
    logined = true;
  if (t_operation == "l" && !logined) {
    userLogIn();
    return true;
  }
  if (t_operation == "r" && !logined) {
    userRegister();
    return true;
  }
  if (t_operation == "dc" && logined) {
    deleteUser();
    return true;
  }
  if (t_operation == "o" && logined) {
    userLogOut();
    return true;
  }
  if (t_operation == "lu" && logined) {
    listAllUsers();
    return true;
  }
  if (t_operation == "cm" && logined) {
    createMeeting();
    return true;
  }
  if (t_operation == "la" && logined) {
    listAllMeetings();
    return true;
  }
  if (t_operation == "las" && logined) {
    listAllSponsorMeetings();
    return true;
  }
  if (t_operation == "lap" && logined) {
    listAllParticipateMeetings();
    return true;
  }
  if (t_operation == "qm" && logined) {
    queryMeetingByTitle();
    return true;
  }
  if (t_operation == "qt" && logined) {
    queryMeetingByTimeInterval();
    return true;
  }
  if (t_operation == "dm" && logined) {
    deleteMeetingByTitle();
    return true;
  }
  if (t_operation == "da" && logined) {
    deleteAllMeetings();
    return true;
  }
  if (t_operation == "q" && !logined) {
    m_agendaService.quitAgenda();
    return false;
  }
  cout << "Wrong operation!" << endl;
  return true;
}

void AgendaUI::userLogIn(void) {
  cout << endl;
  cout << "[log in] [user name] [password]" << endl;
  cout << "[log in] ";
  string username, password;
  cin >> username >> password;
  if (m_agendaService.userLogIn(username, password)){
    m_userName = username;
    m_userPassword = password;
    cout << "succeed!" << endl;
  } else {
    cout << "log in fail!" << endl;
  }
}

void AgendaUI::userRegister(void) {
  cout << endl;
  cout << "[register] [username] [password] [email] [phone]" << endl;
  cout << "[register] ";
  string username, password, email, phone;
  cin >> username >> password >> email >> phone;
  if (m_agendaService.userRegister(username, password,email, phone)) {
    m_userName = username;
    m_userPassword = password;
    cout << "succeed!" << endl;
  } else {
    cout << "register fail!" << endl;
  }
}

void AgendaUI::quitAgenda(void) {
  m_agendaService.quitAgenda();
}

void AgendaUI::userLogOut(void) {
  m_userName = "";
  m_userPassword = "";
  cout << endl;
  cout << endl;
}

void AgendaUI::deleteUser(void) {
  cout << endl;
  m_agendaService.deleteUser(m_userName, m_userPassword);
  cout << "[delete agenda account] succeed!" << endl;
}

void AgendaUI::listAllUsers(void) {
  cout << endl;
  cout << "[list all users]" << endl;
  cout << endl;
  cout << left;
  cout << setw(20) << "name" << setw(30) << "email" << setw(15) << "phone" << endl;
  list<User> u_list = m_agendaService.listAllUsers();
  for(User &u : u_list) {
    cout << left;
    cout << setw(20) << u.getName() << setw(30) << u.getEmail() << setw(15)
      << u.getPhone() <<endl;
  }
}

void AgendaUI::createMeeting(void) {
  cout << endl;
  cout << "[create meeting] [the number of participators]" << endl;
  int num_of_participators;
  cout << "[create meeting] ";
  cin >> num_of_participators;

  std::vector<string> vp;
  string t_participators, title, sTime, eTime, tmp;
  for (int i = 1; i < num_of_participators + 1; ++i) {
    cout << "[please enter the participator " << i << " ]";
    cin >> tmp;
    vp.push_back(tmp);
  }
  cout << "[title][start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
    << endl;
  cin >> title >> sTime >> eTime;
  if(m_agendaService.createMeeting(m_userName, title, sTime, eTime, vp)) {
    cout << "succeed!" << endl;
  } else {
    cout << "error!" << endl;
  }
}

void AgendaUI::listAllMeetings(void) {
  cout << endl;
  cout << "[list all meetings]" << endl;
  cout << endl;
  list<Meeting> m_list = m_agendaService.listAllMeetings(m_userName);
  printMeetings(m_list);
}

void AgendaUI::listAllSponsorMeetings(void) {
  cout << endl;
  cout << "[list all meetings]" << endl;
  cout << endl;
  list<Meeting> m_list = m_agendaService.listAllSponsorMeetings(m_userName);
  printMeetings(m_list);
}

void AgendaUI::listAllParticipateMeetings(void) {
  cout << endl;
  cout << "[list all meetings]" << endl;
  cout << endl;
  list<Meeting> m_list =
          m_agendaService.listAllParticipateMeetings(m_userName);
  printMeetings(m_list);
}

void AgendaUI::queryMeetingByTitle(void) {
  cout << endl;
  cout << "[query meeting] [title]:" << endl;
  string title;
  cout << "[query meeting] ";
  cin >> title;
  list<Meeting> m_list = m_agendaService.meetingQuery(m_userName, title);
  printMeetings(m_list);
}

void AgendaUI::queryMeetingByTimeInterval(void) {
  cout << endl;
  cout <<
  "[query meeting] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"
      << endl;
  string sTime, eTime;
  cout << "[query meeting] ";
  cin >> sTime >> eTime;
  list<Meeting> m_list =
          m_agendaService.meetingQuery(m_userName, sTime, eTime);
  printMeetings(m_list);
}

void AgendaUI::deleteMeetingByTitle(void) {
  cout << endl;
  cout << "[delete meeting] [title]" << endl;
  cout << "[delete meeting] ";
  string title;
  cin >> title;
  if (m_agendaService.deleteMeeting(m_userName, title)) {
    cout << "[delete meeting by title] succeed!" << endl;
  } else {
    cout << "[error] delete meeting fail!" << endl;
  }
  cout << endl;
}

void AgendaUI::deleteAllMeetings(void) {
  cout << endl;
  if (m_agendaService.deleteAllMeetings(m_userName)) {
    cout << "[delete all meetings] succeed!" << endl;
  } else {
    cout << "[delete all meetings] failed!" << endl;
  }
  cout << endl;
}

string mvectorToString(std::vector<string> mv) {
  string result ="";
  for (auto it = mv.begin(); it != mv.end(); ++it) {
    if(!(it == mv.begin()))
      result += ',';
    result += *it;
  }
  return result;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
  cout << left;
  cout << setw(10) << "title" << setw(15) << "sponsor" << setw(20)
    << "start time" << setw(20) << "end time" << setw(50) << "participators"
    << endl;
  for(Meeting &m : t_meetings) {
    cout << setw(10) << m.getTitle() << setw(15) << m.getSponsor() << setw(20)
      << Date::dateToString(m.getStartDate()) << setw(20)
      << Date::dateToString(m.getEndDate()) << setw(50)
      << mvectorToString(m.getParticipator()) << endl;
  }
  cout << endl;
}
