object = src/Agenda.cpp src/AgendaUI.cpp src/AgendaService.cpp src/Storage.cpp src/Date.cpp src/User.cpp src/Meeting.cpp

main : build/Agenda.o
	g++ -I include $(object) -o bin/AgendaUI
build/AgendaUI.o : build/AgendaUI.o
	g++ -I include 

