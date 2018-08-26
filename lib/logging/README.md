# Logging Lib

Header only lib composed only by one header file

## Features
* Unlimited parameter list
* Sequential formatting
* Supports all types that can be printed on stdout with operator<< 

## Use

First include header
```cpp
#include <rickycorte/Logging.hpp>
```

Example use:
```cpp
int a = 1;

// Print [DEBUG] test 1
RC_DEBUG("test ", a);

// Print [INFO] test 1
RC_INFO("test ", a);

// Print [WARNING] test 1
RC_WARNING("test ", a);

// Print [ERROR] test 1
RC_ERROR("test ", a);

// Print [CRITICAL] test 1
RC_CRITICAL("test ", a);
```

