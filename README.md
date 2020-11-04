# idgen.hpp

[![travis][badge.travis]][travis]
[![appveyor][badge.appveyor]][appveyor]
[![codecov][badge.codecov]][codecov]
[![language][badge.language]][language]
[![license][badge.license]][license]
[![paypal][badge.paypal]][paypal]

[badge.travis]: https://img.shields.io/travis/BlackMATov/idgen.hpp/main.svg?logo=travis
[badge.appveyor]: https://img.shields.io/appveyor/ci/BlackMATov/idgen-hpp/main.svg?logo=appveyor
[badge.codecov]: https://img.shields.io/codecov/c/github/BlackMATov/idgen.hpp/main.svg?logo=codecov
[badge.language]: https://img.shields.io/badge/language-C%2B%2B17-yellow.svg
[badge.license]: https://img.shields.io/badge/license-MIT-blue.svg
[badge.paypal]: https://img.shields.io/badge/donate-PayPal-orange.svg?logo=paypal&colorA=00457C

[travis]: https://travis-ci.org/BlackMATov/idgen.hpp
[appveyor]: https://ci.appveyor.com/project/BlackMATov/idgen-hpp
[codecov]: https://codecov.io/gh/BlackMATov/idgen.hpp
[language]: https://en.wikipedia.org/wiki/C%2B%2B17
[license]: https://en.wikipedia.org/wiki/MIT_License
[paypal]: https://www.paypal.me/matov

[idgen]: https://github.com/BlackMATov/idgen.hpp

## Requirements

- [gcc](https://www.gnu.org/software/gcc/) **>= 7**
- [clang](https://clang.llvm.org/) **>= 5.0**
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

## [License (MIT)](./LICENSE.md)
