if [ ! -d "./obj" ]; then
    mkdir "./obj" 
fi

if [ ! -d "./bin" ]; then
    mkdir "./bin"
fi


make clean
make all
bin/driver