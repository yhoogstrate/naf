#!/bin/bash


rm unnaf/unnaf; rm ennaf/ennaf ; make


./unnaf/unnaf -c test.fa.old.naf

echo ""
echo ""
echo "---------------------" 
echo "" 
echo "" 

./ennaf/ennaf test.fa --temp-dir /tmp -o /dev/null

echo ""
echo ""
echo "---------------------" 
echo "" 
echo "" 


./unnaf/unnaf -c test.fa.naf
