distortos [![Build Status](https://travis-ci.org/DISTORTEC/distortos.svg)](https://travis-ci.org/DISTORTEC/distortos)
=========

object-oriented C++ RTOS for microcontrollers

[Homepage](https://distortos.org/)<br/>
[Documentation](https://distortos.org/documentation/)<br/>
[Source Code @ GitHub](https://github.com/DISTORTEC/distortos)<br/>
[Forum](https://groups.google.com/d/forum/distortos)<br/>

Configuration & building
------------------------

To configure & build *distortos* you need:
- [CMake](https://cmake.org/) (version 3.8 or later);
- [a build tool supported by CMake](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#manual:cmake-generators(7)) -
it is highly recommended to use [Ninja](https://ninja-build.org/);
- [arm-none-eabi bleeding-edge-toolchain](https://github.com/FreddieChopin/bleeding-edge-toolchain) (*GCC* version 5 or
later);

*distortos* tries to follow typical *CMake* cross-compiling workflow, which means that you always have to use a
so-called *toolchain file*. Toolchain files in *distortos* also serve another purpose - they select the board which is
going to be used by your application.

1. Download source package of *distortos* in [zip](https://github.com/DISTORTEC/distortos/archive/master.zip) or
[tar.gz](https://github.com/DISTORTEC/distortos/archive/master.tar.gz) format and extract it;
2. Create a build folder, for example `output`;
3. From within the build folder, initialize it with *CMake*, for example with
`cmake .. -DCMAKE_TOOLCHAIN_FILE=../source/board/ST_STM32F4DISCOVERY/Toolchain-ST_STM32F4DISCOVERY.cmake -GNinja` if you
want a default configuration or
`cmake -C../configurations/ST_STM32F4DISCOVERY/test/distortosConfiguration.cmake .. -GNinja` if you want to start from a
saved configuration;
4. Edit *distortos* configuration with a tool of your choice, for example `cmake-gui ..` (a GUI application) or
`ccmake ..` (*curses*-based application);
5. Execute selected build tool, for example `ninja` or `ninja -v` if you want to see all command lines while building;

You can obviously replace step 1 with `git clone https://github.com/DISTORTEC/distortos`.

Steps 2-4 can be all done from within `cmake-gui`. After starting the application use *Browse Source...* button to
select the folder with *distortos* and *Browse Build...* button to select the build folder. Then click on *Configure*
button. In the *CMakeSetup* window which appears select the generator of your choice and make sure that
*Specify toolchain file for cross-compiling* is selected before going any further. Click *Next* and specify the
toolchain file (which also selects the board), for example
`<source-folder>/source/board/ST_STM32F4DISCOVERY/Toolchain-ST_STM32F4DISCOVERY.cmake` and click *Finish* button.

### Test application

The default target of build - *all* - is just the static library with *distortos* `libdistortos.a`. If you want to build
the test application, specify `distortosTest` as the target (for example `ninja distortosTest` if you use *Ninja*).

### tl;dr

    $ wget https://github.com/DISTORTEC/distortos/archive/master.tar.gz
    $ tar -xf master.tar.gz
    $ cd distortos-master
    $ mkdir output
    $ cd output
    $ cmake .. -DCMAKE_TOOLCHAIN_FILE=../source/board/ST_STM32F4DISCOVERY/Toolchain-ST_STM32F4DISCOVERY.cmake -GNinja
    $ cmake-gui ..
    $ ninja

or

    $ wget https://github.com/DISTORTEC/distortos/archive/master.tar.gz
    $ tar -xf master.tar.gz
    $ cd distortos-master
    $ mkdir output
    $ cd output
    $ cmake -C../configurations/ST_STM32F4DISCOVERY/test/distortosConfiguration.cmake .. -GNinja
    $ cmake-gui ..
    $ ninja

Generating board
----------------

To generate a board you need:
- [Python](https://www.python.org/) (version 2.7, version 3.6 or later);
- [Jinja2](http://jinja.pocoo.org/) template engine for *Python* (version 2.10 or later);
- [ruamel.yaml](https://bitbucket.org/ruamel/yaml) YAML loader/dumper package for *Python*;

Both *Jinja2* and *ruamel.yaml* can be easily installed with `pip install jinja2 && pip install ruamel.yaml` (or
`python -m pip install jinja2` followed by `python -m pip install ruamel.yaml` on *Windows*), however they may also be
available in the package manager of your system.

Board generator - `scripts/generateBoard.py` - takes a `*.yaml` file as an input and produces a folder containing
various board files: source files, headers, *CMake* files (including *CMake* toolchain file) and so on. The input
`*.yaml` file describes the board hardware in a tree-like form. The idea is very close to *devicetree* and in fact
earlier versions of board generator used *devicetree* files.

To get an idea about the format of the board YAML files, take a look at some of the existing files, for example
`source/board/ST_STM32F4DISCOVERY/ST_STM32F4DISCOVERY.yaml` - which describes *STM32F4DISCOVERY* board from *ST* - or
`source/chip/STM32/STM32F4/chipYaml/ST_STM32F407VG.yaml` - which describes *STM32F407VG* chip used on this board. There
is also some documentation about YAML bindings in `documentation/yaml-bindings`

Assuming that you already have *distortos* either as part of your project or as a standalone folder, the basic
invocation of the board generator is just `path/to/distortos/scripts/generateBoard.py path/to/board.yaml` (or
`python path/to/distortos/scripts/generateBoard.py path/to/board.yaml` on *Windows*), for example
`./scripts/generateBoard.py source/board/ST_STM32F4DISCOVERY/ST_STM32F4DISCOVERY.yaml`. You may also generate so-called
*raw-boards*, using chip YAML file as the input directly, for example
`./scripts/generateBoard.py source/chip/STM32/STM32F4/chipYaml/ST_STM32F407VG.yaml -o output/path/of/raw/board`.
