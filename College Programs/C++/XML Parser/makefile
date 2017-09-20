
# signs are comments
CC = g++  #set the compiler
FLAGS = -g -I . #includes the flags you set for the compiler
EXE_NAME = XMLGen
OFILES = mchild.o CS240.o xmltag.o family.o person.o event.o place.o history.o marriage.o parents.o occupation.o main.o

$(EXE_NAME):	$(OFILES)
		$(CC) $(FLAGS) -o $(EXE_NAME) $(OFILES)
		chmod 711 $(EXE_NAME)

xmltag.o:	xmltag.cpp xmltag.h
		$(CC) $(FLAGS) -c xmltag.cpp
mchild.o:	mchild.cpp mchild.h
		$(CC) $(FLAGS) -c mchild.cpp

CS240.o:	CS240.cpp CS240.h
		$(CC) $(FLAGS) -c CS240.cpp

family.o:	family.cpp family.h
		$(CC) $(FLAGS) -c family.cpp

person.o:	person.cpp person.h
		$(CC) $(FLAGS) -c person.cpp

event.o:	event.h event.cpp
		$(CC) $(FLAGS) -c event.cpp

place.o:	place.cpp place.h
		$(CC) $(FLAGS) -c place.cpp

history.o:	history.cpp history.h
		$(CC) $(FLAGS) -c history.cpp

marriage.o:	marriage.cpp marriage.h
		$(CC) $(FLAGS) -c marriage.cpp

parents.o:	parents.cpp parents.h
		$(CC) $(FLAGS) -c parents.cpp

occupation.o:	occupation.cpp occupation.h
		$(CC) $(FLAGS) -c occupation.cpp

main.o:		main.cpp 
		$(CC) $(FLAGS) -c main.cpp
