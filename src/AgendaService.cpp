#include "AgendaService.hpp"
#include <list>

AgendaService::AgendaService() {
  startAgenda();
}

AgendaService::~AgendaService() {
  quitAgenda();
}

bool AgendaService::userLogIn(const std::string userName,
  const std::string password) {
  std::list<User> nl = m_storage -> queryUser([&userName](const User& t_user) 
    -> bool {
    if (t_user.getName() == userName)
      return true;
    return false;
  });
  if (nl.empty())
    return false;
  if (nl.begin() -> getPassword() == password)
    return true;
  return false;
}

bool AgendaService::userRegister(const std::string userName, 
                      const std::string password,
                      const std::string email, const std::string phone) {
  std::list<User> nl = m_storage -> queryUser([&userName](const User& t_user) 
    -> bool {
    if (t_user.getName() == userName)
      return true;
    return false;
  });
  if (!nl.empty())
    return false;
  m_storage -> createUser(User(userName, password, email, phone));
  return true;
}

bool AgendaService::deleteUser(const std::string userName, 
                          const std::string password) {
  std::list<User> nl = m_storage -> queryUser([&userName](const User& t_user) 
    -> bool {
    if (t_user.getName() == userName)
      return true;
    return false;
  });
  if (nl.empty())
    return false;
  if (nl.begin() -> getPassword() != password)
    return false;
  m_storage -> deleteMeeting([userName](const Meeting& t_meeting) -> bool{
    if (t_meeting.getSponsor() == userName 
      || t_meeting.isParticipator(userName)) {
      return true;
    }
    return false;
  }); 
  m_storage -> deleteUser([userName](const User& t_user) -> bool {
    if (t_user.getName() == userName){
      return true;
    }
    return false;
  });
  return true;
}

std::list<User> AgendaService::listAllUsers(void) const {
  return m_storage -> queryUser([](const User& t_user) {return true;});
}

bool AgendaService::createMeeting(const std::string userName,
                       const std::string title,
                       const std::string startDate, const std::string endDate,
                       const std::vector<std::string> participator) {
  for (auto it = participator.begin(); it != participator.end(); ++it) {
    if((*it) == userName)
      return false;
    for(auto it2 = participator.begin(); it2 != it; ++it2) {
      if((*it) == (*it2))
        return false;
    }
  }
  Date s_date(startDate);
  Date e_date(endDate);
  std::list<User> nl = m_storage -> queryUser([&userName](const User& t_user) 
    -> bool {
    if (t_user.getName() == userName)
      return true;
    return false;
  });
  if (nl.empty())
    return false;
  if(!Date::isValid(s_date) || !Date::isValid(e_date) || s_date >= e_date)
    return false;

  std::list<Meeting> existMeeting =
                      m_storage -> queryMeeting([](const Meeting& t_meeting) {return true;});
  for(Meeting &t_meeting : existMeeting) {
    if (existMeeting.empty())
      break;

    if (t_meeting.isParticipator(userName) || t_meeting.getSponsor() == userName);
      if (t_meeting.getStartDate() < e_date && 
          t_meeting.getEndDate() > s_date)
        return false;
  }

  for(auto it = participator.begin(); it != participator.end(); ++it) {
    nl = m_storage -> queryUser([&it](const User& t_user) 
    -> bool {
      if (t_user.getName() == *it)
       return true;
      return false;
    });
    if (nl.empty())
      return false;

    if (existMeeting.empty())
      break;

    for(Meeting t_meeting : existMeeting) {
      if (t_meeting.isParticipator(*it) || t_meeting.getSponsor() == *it);
        if (t_meeting.getStartDate() < e_date && 
            t_meeting.getEndDate() > s_date)
          return false;
    }
  }

  existMeeting = m_storage -> queryMeeting([title](const
    Meeting& m) {
    if (m.getTitle() == title)
      return true;
    return false;
  });
  if (!existMeeting.empty())
    return false;

  m_storage -> createMeeting(Meeting(userName, participator, s_date, e_date,
    title));

  return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string title) const {
  std::list<Meeting> result = m_storage -> queryMeeting([&userName, &title](const
    Meeting& m) -> bool {
    if ((m.getSponsor() == userName || m.isParticipator(userName)) &&
      m.getTitle() == title) {
      return true;
    }
    return false;
  });
  return result;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string userName,
                                    const std::string startDate,
                                    const std::string endDate) const {
  std::list<Meeting> result = m_storage -> queryMeeting([&userName, &startDate, &endDate](const
    Meeting& m) -> bool {
    if ((m.getSponsor() == userName || m.isParticipator(userName)))
      if(m.getStartDate() <= Date::stringToDate(endDate) &&
          m.getEndDate() >= Date::stringToDate(startDate)) {
        return true;
    }
    return false;
  });
  return result;
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string userName)
                                          const {
  std::list<Meeting> result = m_storage -> queryMeeting([&userName](const Meeting& m) {
    if (m.getSponsor() == userName || m.isParticipator(userName)) {
      return true;
    }
    return false;
  });
  return result;
}

std::list<Meeting> AgendaService::listAllSponsorMeetings
            (const std::string userName) const {
  std::list<Meeting> result = m_storage -> queryMeeting([&userName](const Meeting& m) {
    if (m.getSponsor() == userName) {
      return true;
    }
    return false;
  });
  return result;
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
        const std::string userName) const {
  std::list<Meeting> result = m_storage -> queryMeeting([&userName]
                                              (const Meeting& m) {
    if (m.isParticipator(userName)) {
      return true;
    }
    return false;
  });
  return result;
}

bool AgendaService::deleteMeeting(const std::string userName,
        const std::string title) {
  int n = m_storage -> deleteMeeting([&userName, &title](const
    Meeting& m) -> bool {
    if ((m.getSponsor() == userName) && m.getTitle() == title) {
      return true;
    }
    return false;
  });
  if (n > 0)
    return true;
  return false;
}

bool AgendaService::deleteAllMeetings(const std::string userName) {
  int n = m_storage -> deleteMeeting([&userName](const
    Meeting& m) -> bool {
    if (m.getSponsor() == userName) {
      return true;
    }
    return false;
  });
  if (n > 0)
    return true;
  return false;
}

void AgendaService::startAgenda(void) {
  m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
}
