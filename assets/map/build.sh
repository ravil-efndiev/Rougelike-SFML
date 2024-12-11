#/bin/zsh
cd build
cmake -S .. -B .
cmake --build .
if [ $? -eq 0 ]; then
    if [ $# -ge 1 ] && [ $1 = "edit" ]; then
        ./game edit ../assets/map/test_map.yml
    else
        ./game
    fi
fi
