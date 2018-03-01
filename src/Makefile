#makefile

#DEBUG = -03


OBJ 		= main.o \
			  uart.o \
			  color.o \
			  packet.o \
			  motion.o \
			  config.o \
			  task.o \
			  mission.o \
			  sensor.o
			 # ./tasks/task1.o \
			 # ./tasks/task2.o

DEPS 		= uart.h \
			  color.h \
			  motioncmd.h \
			  packet.h \
			  motion.h \
			  config.h \
			  mission.h \
			  task.h \
			  sensor.h

INCLUDE		= -I/usr/local/include


LDFLAGS		= -L/usr/local/lib
LDLIBS    	= -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt

#Any special libraries you are using in your project (e.g. -lbcm2835 -lrt `pkg-config --libs gtk+-3.0` ), or leave blank
#LIBS = -lbcm2835 -lrt

#Set any compiler flags you want to use (e.g. -I/usr/include/somefolder `pkg-config --cflags gtk+-3.0` ), or leave blank
CFLAGS	:= -Wall $(INCLUDE) $(LDFLAGS) $(LDLIBS) -Winline -pipe
CFLAGS	+= -lrt -g
#Set the compiler you are using ( gcc for C or g++ for C++ )
CC	= gcc

#Set the filename extensiton of your C files (e.g. .c or .cpp )
EXTENSION = .c

# define a rule that applies to all files ending in the .o suffix, 
# which says that the .o file depends upon the .c version of the file 
# and all the .h files included in the DEPS macro.  
# Compile each object file
%.o: %$(EXTENSION) $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#Combine them into the output file
#Set your desired exe output file name here

orcs_run: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

#Cleanup
.PHONY: clean

clean:
	rm -f *.o *~ core *~ 
