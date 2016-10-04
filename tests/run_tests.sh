#!/bin/bash

SOPATH="`pwd`/pldotnet-so/pldotnet.so"

cd pldotnet-so; make clean; make; cd ..
cd sql && psql  -v SOPATH="$SOPATH" -f install.sql raisin raisin; cd ..

cd tests && make && psql -v PWD="`pwd`" -f test.sql raisin raisin && psql -v PWD="`pwd`" -f inline.sql raisin raisin && cd ..

