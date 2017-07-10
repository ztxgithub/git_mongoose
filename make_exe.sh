cp main.cpp publish_subscribe.c &&
gcc publish_subscribe.c ./src/mongoose.c -o publish_subscribe -g -W -Wall -I./include/ -Wno-unused-function  -DMG_ENABLE_THREADS -pthread
