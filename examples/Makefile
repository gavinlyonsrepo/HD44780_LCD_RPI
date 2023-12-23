# Gavin lyons 02-2022
# Makefile to build example program for HD44780_LCD_RPI library.
# URL: https://github.com/gavinlyonsrepo/HD44780_LCD_RPI

# ********************
# Options
# 1. make  
# 2. make clean
# 3. make run (note :: wraps sudo)
# ********************

# ************************************************
# User SRC directory Option Section
# Pick on example "SRC" dir path and ONE Only.
# Comment out the other's. 

SRC=src/HELLO_16x02
#SRC=src/TEST_16x02
#SRC=src/TEST_20x04
#SRC=src/CLOCK_16x02

# ************************************************

CC=g++
LDFLAGS= -lbcm2835 -lrt -lHD44780_LCD_RPI 
CFLAGS = -std=c++2a -Iinclude/ -c -Wall 
MD=mkdir
OBJ=obj
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp,  $(OBJ)/%.o, $(SRCS))
BIN=./bin
TARGET=$(BIN)/test

all: clean pre-build $(TARGET) 

run:
	sudo $(TARGET) 
	
pre-build:
	@echo '*******'
	@echo '[START!]'
	$(MD) -vp $(BIN)
	$(MD) -vp $(OBJ)


$(TARGET): $(OBJS) $(OBJ)
	@echo '[MAKE EXE FILE!]'
	$(CC)  $(OBJS) -o $@ $(LDFLAGS)
	@echo '[DONE!]'

$(OBJ)/%.o :  $(SRC)/%.cpp   $(OBJ) 
	@echo '[MAKE OBJECT FILE!]'
	$(CC) $(CFLAGS) $< -o  $@ 

clean:
	@echo '*******'
	@echo '[CLEANUP!]'
	rm -vf $(OBJS) $(TARGET)
	@echo '[DONE!]'
