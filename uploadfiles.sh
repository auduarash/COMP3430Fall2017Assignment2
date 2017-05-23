make clean;
pwd;
mv Makefile tempMakefile;
cp aviaryMakefile Makefile;
scp *.c *.h Makefile \
    audua@aviary.cs.umanitoba.ca:~/Desktop/3430/assignments/a2
mv tempMakefile Makefile;