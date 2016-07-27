
all: test.dll 

test.dll: test.cs
	gmcs -out:test.dll -target:library test.cs ../shared/pldotnet.cs

clean: 
	rm -f *.dll

