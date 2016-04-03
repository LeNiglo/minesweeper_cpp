NAME		= minesweeper

SRCS		= main.cpp \
			  Game.cpp \
			  Board.cpp \
			  Cell.cpp

OBJS		= $(SRCS:.cpp=.o)

LIBDIR		= -L/usr/local/lib/
INCDIR		= -I/usr/local/include/

CXX			= g++
CXXFLAGS	+= -W -Wall -Wextra -Werror -pedantic
CXXFLAGS	+= $(INCDIR)
CXXFLAGS	+= -std=c++11

SFML		= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

LIBS		+= $(LIBDIR)
LIBS		+= $(SFML)

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) -o $(NAME) $(OBJS) $(CXXFLAGS) $(LIBS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all
