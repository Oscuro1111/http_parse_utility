#!/bin/bash

function build_lib(){
    cd lib
    gcc -shared -o $1.$2 $(ls ../objs/*.o)
    cd .. 
}

if [ -d lib ]
then
      build_lib $1 $2 $3
else
   mkdir lib&&build_lib $1 $2 $3
fi
