
GCC=gcc

$(shell pg_config >pg.mk)
include pg.mk

MONO_CPPFLAGS = `pkg-config --cflags --libs mono-2 glib-2.0`
MONO_LIBS = `pkg-config --cflags --libs mono-2 glib-2.0`
SHLIB_LINK = `pkg-config --cflags --libs mono-2 glib-2.0`

all: _pldotnetcore.h pldotnet.so 

pldotnetcore.dll: pldotnetcore.cs 
	mcs -out:pldotnetcore.dll -target:library -r:Mono.CSharp.dll pldotnetcore.cs

_pldotnetcore.h: pldotnetcore.dll
	xxd -i pldotnetcore.dll _pldotnetcore.h

%.o: %.c
	$(GCC) -Wall $(MONO_CPPFLAGS) -I$(INCLUDEDIR) -I$(INCLUDEDIR-SERVER) -fpic -c $^

pldotnet.so: pldotnet.o mono_functions.o functionlist.o utils.o typeconv.o internal.o 
	$(GCC) -shared $(MONO_CPPFLAGS) -o $@ $^

clean:
	rm -f *.o pldotnet.so pldotnetcore.dll _pldotnetcore.h


