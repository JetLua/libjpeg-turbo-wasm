build:
	emcc main.c -I/Users/jet/Desktop/libjpeg-turbo-3.0.3/ -L/Users/jet/Desktop/mozjpeg-master/ -lturbojpeg -o index.js -sEXPORT_ES6=1 -sENVIRONMENT=web -sMODULARIZE -sALLOW_MEMORY_GROWTH=1 -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,getValue,setValue -sEXPORTED_FUNCTIONS=_malloc,_free,_realloc
cc:
	clang main.c -I/opt/homebrew/include -L/opt/homebrew/lib  -o main
