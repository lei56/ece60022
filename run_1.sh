if [ ! -d "./obj" ]; then
    mkdir "./obj" 
fi

if [ ! -d "./bin" ]; then
    mkdir "./bin"
fi

make clean
make all

bin/driver -f traces/trace_1.csv
bin/driver -f traces/trace_1.csv -e 0
bin/driver -f traces/trace_1.csv -e 0 -t 0
