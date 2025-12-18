cmake . -B build && make -C build && \
echo "copying build/compile_commands.json to root" && \
cp build/compile_commands.json . && \
./build/nivy

