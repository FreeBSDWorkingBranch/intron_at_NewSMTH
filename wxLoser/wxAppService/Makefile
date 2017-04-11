# wxWidgets
WXCFG	=	wxgtk2u-3.0-config

# GTK 2.x
GTKCFLAGS=	`pkg-config --cflags gtk+-2.0`
GTKLIBS=	`pkg-config --libs gtk+-2.0`

CC	=	cc
CXX	=	c++
CPPFLAGS=	`$(WXCFG) --cppflags` $(GTKCFLAGS) -I/usr/local/include
CFLAGS	=	-g `$(WXCFG) --cflags` $(GTKCFLAGS) -I/usr/local/include
CXXFLAGS=	-g -std=c++11 \
		$(CPPFLAGS) `$(WXCFG) --cxxflags` $(GTKCFLAGS) \
		-I/usr/local/include \
		-Wno-potentially-evaluated-expression

LD	=	c++
LIBS	=	`$(WXCFG) --libs` $(GTKLIBS) -L/usr/local/lib
LDFLAGS	=	-g

OBJS	=	test.o wxAppService.o

EXE	=	test_service

.SUFFIXES:
.SUFFIXES: .o .c .cpp
.PHONY: all clean depend

all: $(EXE)

$(EXE):	$(OBJS)
	$(LD) -o $@ $(LDFLAGS) $> $^ $(LIBS)

.c.o:
	iconv -f GBK -t UTF-8 $< > tmp_$<
	$(CC) $(CFLAGS) -o $@ -c tmp_$<
	rm -f tmp_$<
.cpp.o:
	iconv -f GBK -t UTF-8 $< > tmp_$<
	$(CXX) $(CXXFLAGS) -o $@ -c tmp_$<
	rm -f tmp_$<

clean:
	rm -fv *.o $(EXE) *.core
depend:
	echo -n "" > depend.mak
	if test `find . -name \*.c | wc -l` -gt "0"; then for i in *.c; do $(CC) -E -MM $(CPPFLAGS) $$i >> depend.mak; done; fi
	if test `find . -name \*.cpp | wc -l` -gt "0"; then for i in *.cpp; do $(CXX) -E -MM $(CPPFLAGS) $$i >> depend.mak; done; fi

#include depend.mak

