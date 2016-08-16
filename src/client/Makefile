INSTALLDIR = /tmp/free-music

DESKTOPDIR = ~/.local/share/applications

CC ?= gcc

PKG_CONFIG = $(shell which pkg-config)

CFLAGS = $(shell $(PKG_CONFIG) --cflags gtk+-3.0) -lmysqlclient -I../include -I./include

LIBS = $(shell $(PKG_CONFIG) --libs gtk+-3.0) -lmysqlclient

SRC = main.c player.c list.c mpg123_api.c client.c search.c

OBJS = $(SRC:.c=.o)

all: free-music

%.o: %.c
	@$(CC) -c $< $(CFLAGS) -rdynamic -o $(@F)
	@echo "cc $< -o $@"

free-music: $(OBJS)
	@$(CC) $(OBJS) $(LIBS) -rdynamic -o $(@F)
	@echo "ld $(OBJS) -o $@"

.PHONY: clean install uninstall

clean:
	-rm free-music *.o

install: free-music
	@mkdir -p $(INSTALLDIR); \
	mkdir -p $(INSTALLDIR)/etc; \
	mkdir -p $(INSTALLDIR)/etc/lists.d; \
	cp ../src/free-music $(INSTALLDIR); \
	cp ../resources/*.png $(INSTALLDIR); \
	cp ../icon/free-music.desktop $(DESKTOPDIR); \
	if [ -d $(INSTALLDIR) ]; \
		then \
		chown -R `whoami` $(INSTALLDIR); \
		echo "Complete!"; \
	else \
		echo "Not complete!!!"; \
	fi


uninstall:
	@rm -rf $(INSTALLDIR); \
	rm -f $(DESKTOPDIR)/free-music.desktop; \
	if [ -d $(INSTALLDIR) ]; \
		then \
		echo "Not Complete!"; \
	else \
		echo "Complete!!!"; \
	fi 
