
FILES := main.c utils.c

CC := gcc
CFLAGS := -Wall -Wextra -Werror

ifeq ($(OS), Windows_NT)
	NAME := dazum.exe
	CPPFLAGS := -I./raylib-5.0_win64_mingw-w64/include
	LDLIBS := ./raylib-5.0_win64_mingw-w64/lib/libraylib.a -lgdi32 -lwinmm -mwindows
else
	NAME := dazum
	CPPFLAGS := -I./raylib-5.0_linux_amd64/include
	LDLIBS := ./raylib-5.0_linux_amd64/lib/libraylib.a -lm -lX11
endif

$(NAME): $(FILES)
	$(CC) $(CFLAGS) -o $(NAME) $(FILES) $(CPPFLAGS) $(LDLIBS)

clean:
	$(RM) $(NAME)

fclean: clean

run: $(NAME)
	./$(NAME)

.PHONY: clean fclean run
