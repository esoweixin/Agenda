#include "Storage.hpp"
#include "Path.hpp"
#include <fstream>
#include <sstream>

std::shared_ptr<Storage> Storage::m_instance = nullptr;

Storage::Storage() {
  m_dirty = false;
  this -> readFromFile();
}

static void removeQuotation(std::vector<std::string> tokens) {
  for (std::vector<std::string>::iterator it = tokens.begin();
      it != tokens.end(); ++it) {
    if ((*it)[0] == '\"')
      it -> erase(it -> begin());
    if ((*it)[(*it).length() - 1] == '\"')
      it -> erase(it -> end() - 1);
  }
}

static std::vector<std::string> split(const std::string& t_str, char t_delim) {
  std::stringstream ss(t_str);
  std::string tmp;
  std::vector<std::string> tokens;
  while (getline(ss, tmp, t_delim)) {
    tokens.push_back(tmp);
  }

  removeQuotation(tokens);
  return tokens;
}

std::vector<std::string> stringToVector(const std::string& t_str) {
  std::stringstream ss(t_str);
  std::string tmp;
  std::vector<std::string> p_list;
  while (std::getline(ss, tmp, '&')) {
    p_list.push_back(tmp);
  }
  return p_list;
}

bool Storage::readFromFile(void) {
  std::fstream user_in;
  user_in.open(Path::userPath, std::ios::in);
  if (!user_in.good())
    return false;
  std::string line;
  while (std::getline(user_in, line)) {
    std::vector<std::string> t_user = split(line, ',');
    for (auto it = t_user.begin(); it != t_user.end(); ++it) {
      (*it).erase((*it).begin());
      (*it).erase((*it).end() - 1);
    }
    m_userList.push_back(User(t_user[0], t_user[1], t_user[2], t_user[3]));
  }
  user_in.close();

  std::fstream meeting_in;
  meeting_in.open(Path::meetingPath, std::ios::in);
  if (!meeting_in.good())
    return false;
  while (std::getline(meeting_in, line)) {
    std::vector<std::string> t_meeting = split(line, ',');
    for (auto it = t_meeting.begin(); it != t_meeting.end(); ++it) {
      (*it).erase((*it).begin());
      (*it).erase((*it).end() - 1);
    }
    m_meetingList.push_back(Meeting(t_meeting[0], stringToVector(t_meeting[1]),
        Date(t_meeting[2]), Date(t_meeting[3]), t_meeting[4]));
  }
  meeting_in.close();
}

std::string vectorToString(std::vector<std::string> &participator) {
  std::string result ="";
  for (auto it = participator.begin(); it != participator.end(); ++it) {
    if(!(it == participator.begin()))
      result += '&';
    result += *it;
  }
  return result;
}

bool Storage::writeToFile(void) {
  if (!m_dirty) {
    return false;
  }

  std::fstream io;
  if (io.fail())
    return false;
  io.open(Path::userPath, std::ios::out);
  for (User &it : m_userList) {
    io << "\"" << it.getName() << "\"" << "," << "\"" << it.getPassword()
        << "\"" << "," << "\"" << it.getEmail() << "\"" << "," << "\""
        << it.getPhone() << "\"" << std::endl;
  }
  io.close();

  io.open(Path::meetingPath, std::ios::out);
  if (io.fail())
    return false;
  for (Meeting &it : m_meetingList) {
    std::vector<std::string> participatorList = it.getParticipator();
    io << "\"" << it.getSponsor() << "\"" << "," << "\""
      << vectorToString(participatorList) << "\"" << "," << "\""
      << Date::dateToString(it.getStartDate()) << "\"" << "," << "\""
      << Date::dateToString(it.getEndDate()) << "\"" << ","
      << "\"" << it.getTitle() << "\"" << std::endl;
  }
  io.close();
  return true;
}

std::shared_ptr<Storage> Storage::getInstance(void) {
  if (m_instance == nullptr)
    m_instance = std::shared_ptr<Storage>(new Storage);
  return m_instance;
}

Storage::~Storage() {
  if (this -> m_dirty) {
    this -> writeToFile();
  }
}

void Storage::createUser(const User & t_user) {
  m_userList.push_back(t_user);
  m_dirty = true;
}

std::list<User> Storage::queryUser(std::function<bool(const User &)> filter)
  const {
  std::list<User> n_list;
  for (auto &it : m_userList)
    if (filter(it))
      n_list.push_back(it);
  return n_list;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                 std::function<void(User &)> switcher) {
  int n = 0;
  for (auto &it : m_userList)
    if (filter(it)) {
      switcher(it);
      ++n;
      m_dirty = true;
    }
  return n;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
  int n = 0;
  for (auto it = m_userList.begin(); it != m_userList.end(); ++it)
    if (filter(*it)) {
      it = m_userList.erase(it);
      ++n;
      --it;
    }
  return n;
}

void Storage::createMeeting(const Meeting & t_meeting) {
  m_meetingList.push_back(t_meeting);
  m_dirty = true;
}

std::list<Meeting> Storage::queryMeeting
                    (std::function<bool(const Meeting &)> filter) const {
  std::list<Meeting> n_list;
  for (auto &it : m_meetingList)
    if (filter(it))
      n_list.push_back(it);
  return n_list;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                    std::function<void(Meeting &)> switcher) {
  int n = 0;
  for (auto &it : m_meetingList)
    if (filter(it)) {
      switcher(it);
      ++n;
      m_dirty = true;
    }
  return n;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
  int n = 0;
  for (auto it = m_meetingList.begin(); it != m_meetingList.end(); ++it)
    if (filter(*it)) {
      it = m_meetingList.erase(it);
      ++n;
      --it;
      m_dirty = true;
    }
  return n;
}

bool Storage::sync(void) {
  return writeToFile();
}
