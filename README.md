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
* libmcrypt
* zlib
* [HinnantDate](https://github.com/HowardHinnant/date)
* [sqlpp11](https://github.com/rbock/sqlpp11)
* [sqlpp11-connector-mysql](https://github.com/rbock/sqlpp11-connector-mysql)
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
$ chmod a+x shiro
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

## Modules

List of required and optional modules developed for Shiro:

| Name | Language | Dependency | License | Description | Info |
| ---- | -------- | ---------- | ------- | ----------- | ---- |
| [libosu](https://github.com/cyanidee/libosu) | C++ | **Required** | GPL-3.0 | Library for writing and reading osu! packets | |
| [libakame](https://github.com/Marc3842h/libakame) | C++ | **Required** | MIT | A osu! difficulty and pp calculator with support for all four game modes as well as converted maps | |
| [kuro](https://github.com/Marc3842h/kuro) | TypeScript | Optional | AGPL-3.0 | Browser facing portion of shiro | While shiro can run just fine without frontend, it is recommended to have one for convenience. |
| [kyo](https://github.com/Marc3842h/kyo) | | Optional | LGPL-3.0 | High quality and modern C++ osu! server switcher | |
| [shirogane](https://github.com/Marc3842h/shirogane) | Rust | Optional | LGPL-3.0 | osu! Beatmap mirror developed for shiro | Required when setting beatmap mirror to `custom` in `direct.toml` config file. |

All modules are submodules of this repository, even when the module is not a required dependency.

## Docker ![Docker](https://img.shields.io/docker/pulls/marc3842h/shiro.svg)

Two docker images are available for shiro:

* `marc3842h/shiro:latest` Image used for easy setup of shiro, contains all dependencies and a setup nginx. Requires building and running of shiro manually. Based on latest Ubuntu stable.
* `marc3842h/shiro:ci` Image used for continuous integration, used **only** for building shiro and running its tests. Based on latest Ubuntu rolling release.

## License

shiro is licensed under the Shiro Private License. Public
distribution of source code and binaries are forbidden
without prior confirmation from both authors (@Marc3842h and @czapek).
