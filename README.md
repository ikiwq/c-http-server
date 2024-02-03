## c-http-server
This is a repository containing the source code of a simple http server made from scratch in C, using the winsock2 library.

### Usage
Clone the repository into a directory of your choice with

    git clone https://github.com/ikiwq/c-http-server.git

And them compile the source code into an executable. Linking the lws2_32 library is necessary.

     gcc main.c -o main -lws2_32
