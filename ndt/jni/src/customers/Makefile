
#CROSS_COMPILE = arm-linux-gnueabihf-

CC       = $(CROSS_COMPILE)gcc 
AR	     = $(CROSS_COMPILE)ar 

CFLAGS   = -Wall -O -g -lm -std=c99 -static

INCLUDE += -I $(ROOT_DIR)/main
INCLUDE += -I $(ROOT_DIR)/src/algorithm
INCLUDE += -I $(ROOT_DIR)/src/customers
INCLUDE += -I $(ROOT_DIR)/src/customers/vivo
INCLUDE += -I $(ROOT_DIR)/src/hardware
INCLUDE += -I $(ROOT_DIR)/src/hardware/datastore
INCLUDE += -I $(ROOT_DIR)/src/project

DIR_OBJ		= $(ROOT_DIR)/output/obj
DIR_BIN		= $(ROOT_DIR)/output/bin

SRCS 		:= $(wildcard *.c)
OBJS		:= $(patsubst %.c, %.o, $(notdir ${SRCS}))

TARGET   = 

$(OBJS):$(SRCS)
	$(CC) $(INCLUDE) $(CFLAGS) -c $^

all:$(OBJS)

	# $(AR) cqs $(TARGET) $(OBJS)
	mv *.o  $(DIR_OBJ)
	# mv $(TARGET) $(DIR_BIN)

clean:
	rm -f *.o

