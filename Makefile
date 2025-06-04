#Directory definition
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := inc
LIB_DIR := 

#Filenames definition
NAME := ircserv

SRC_NAMES := main.cpp

#INC_NAMES := 

OBJECTS := $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC_NAMES)))

#INCLUDES := $(patsubst %.h, $(INC_DIR)/%.h, $(INC_NAMES))

DEPS := $(OBJECTS:.o=.d)

#Compilateur and flags
CC = c++

CFLAGS = -Wall -Werror -Wextra -MMD -MP -std=c++98
CFLAGS += -g 
#CFLAGS += -I $(INC_DIR)

#LIB_NAMES :=
#LIBS_TAG :=
#LIBS_TAG += $(patsubst lib%.a, -l%, $(LIB_NAMES))

DEBUG ?=

CUR_DIR := $(shell pwd)

#TARGETS
all: $(OBJECTS) $(NAME) 

$(NAME): Makefile $(INCLUDES) $(OBJECTS) | $(LIB_DIR)
	$(CC) $(CFLAGS) $(DEBUG) $(OBJECTS) -o $@ $(LIBS_TAG) $(LIBS_TAG)

$(OBJ_DIR)/%.o: %.cpp Makefile | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(DEBUG) -c $<  -o $@ 

$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(LIB_DIR):
	@mkdir $(LIB_DIR)

flags:
	@echo $(CFLAGS)

show:
	@echo $(OBJECTS)
	@echo $(OBJECTS_BONUS)
#	@echo $(SRC_NAMES)

clean: 
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(LIB_DIR)

re: fclean all
ifneq ($(DEPS), )
-include $(DEPS)
endif
.PHONY: all flags clean fclean re show
