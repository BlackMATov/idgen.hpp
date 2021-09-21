# idgen.hpp

[![linux][badge.linux]][linux]
[![darwin][badge.darwin]][darwin]
[![windows][badge.windows]][windows]
[![codecov][badge.codecov]][codecov]
[![language][badge.language]][language]
[![license][badge.license]][license]

[badge.darwin]: https://img.shields.io/github/workflow/status/BlackMATov/idgen.hpp/darwin/main?label=Xcode&logo=xcode
[badge.linux]: https://img.shields.io/github/workflow/status/BlackMATov/idgen.hpp/linux/main?label=GCC%2FClang&logo=linux
[badge.windows]: https://img.shields.io/github/workflow/status/BlackMATov/idgen.hpp/windows/main?label=Visual%20Studio&logo=visual-studio
[badge.codecov]: https://img.shields.io/codecov/c/github/BlackMATov/idgen.hpp/main?logo=codecov
[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-yellow
[badge.license]: https://img.shields.io/badge/license-MIT-blue

[darwin]: https://github.com/BlackMATov/idgen.hpp/actions?query=workflow%3Adarwin
[linux]: https://github.com/BlackMATov/idgen.hpp/actions?query=workflow%3Alinux
[windows]: https://github.com/BlackMATov/idgen.hpp/actions?query=workflow%3Awindows
[codecov]: https://codecov.io/gh/BlackMATov/idgen.hpp
[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/MIT_License

[idgen]: https://github.com/BlackMATov/idgen.hpp

## Requirements

- [gcc](https://www.gnu.org/software/gcc/) **>= 7**
- [clang](https://clang.llvm.org/) **>= 7**
- [msvc](https://visualstudio.microsoft.com/) **>= 2017**

## Installation

[idgen.hpp][idgen] is a header-only library. All you need to do is copy the headers files from `headers` directory into your project and include them:

```cpp
#include "idgen.hpp/idgen.hpp"
```

Also, you can add the root repository directory to your [cmake](https://cmake.org) project:

```cmake
add_subdirectory(external/idgen.hpp)
target_link_libraries(your_project_target idgen.hpp)
```

## Examples

> coming soon

## API

> coming soon

## [License (MIT)](./LICENSE.md)
