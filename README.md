# shiro [![Circle CI](https://img.shields.io/circleci/token/2f51bb1a72234696bbc4644894b40dd3687253dd/project/github/Marc3842h/shiro/master.svg?logo=circleci)](https://circleci.com/gh/Marc3842h/shiro)

High performance, high quality osu!Bancho C++ re-implementation

## Features

This server handles the following features:

* Client login
* Online user listing and their presence
* Score submission
* In-game scoreboards
* Public and private chat
* Spectator
* Replays
* Multiplayer
* Screenshots
* osu!direct

## Installation

Installation of shiro is simple and can be done using a few simple steps.

#### Dependencies

Install the following dependencies

* cmake
* C++17 compiler
* libmysql
* libboost
* libcurl
* OpenSSL
* tcmalloc (Optional)

#### Building

Clone the repository and execute the following below:

```bash
$ cmake .   # Generates Unix Makefile and checks for missing dependencies
$ make      # Builds and links the project into a executeable
```

The output executable can be found in the `bin` directory.

#### Running

Copy all files from the `resources` directory into
the same directory as the `shiro` executeable. After that
you can start shiro like any other program:

```
$ chmod +x shiro
$ ./shiro
```

If everything could be setup and a connection to the database could be made,
you should see a similar message:

```
2018-07-13 18:11:10.182 (0.047s) [main thread]    shiro.cc:37    0| Welcome to shiro. Listening on http://127.0.0.1:8080/.
2018-07-13 18:11:10.182 (0.047s) [main thread]    shiro.cc:38    0| Press CTRL + C to quit.
```

shiro is now accepting connections from osu! on the address
it's listening on.

## License

shiro is licensed under the Shiro Private License. Public
distribution of source code and binaries are forbidden
without prior confirmation from both authors (@Marc3842h and @czapek).
