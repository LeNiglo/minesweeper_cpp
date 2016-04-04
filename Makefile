NAME		= minesweeper

SRCS		= src/main.cpp \
			  src/Game.cpp \
			  src/Board.cpp \
			  src/Arbiter.cpp \
			  src/Cell.cpp

OBJS		= $(SRCS:.cpp=.o)

LIBDIR		= -L/usr/local/lib/
INCDIR		= -I/usr/local/include/ -I./inc/

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
