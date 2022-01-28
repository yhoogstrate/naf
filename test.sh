#!/bin/bash

#make && ./ennaf/ennaf test.fa --temp-dir /tmp ; ./unnaf/unnaf -c test.fa.naf

make && ./unnaf/unnaf -c test.fa.old.naf && echo "" && echo "" && echo "---------------------"  && echo "" && echo "" &&  ./unnaf/unnaf -c test.fa.naf
