# Hikari Config Lib

Load/Save settings on json files

## Features

* Global settings file
* Default value save
* No need to generate/ship a default config (automatic generation at runtime)
* Dynamic config generation (keys are added only when you need)

## Use

First ```#include "config/Config.hpp"```

### Global Config

```cpp
using namespace RickyCorte;

std::string res; // get function returns only strings

// You should first call Config::Global::Init()
// This call can be omited, the first call of a get/set will also call Init()

// get the key "test"
res = Config::Global::Get("test");   
 
// get the key "test", if missing return "rip"            
res = Config::Global::Get("test", "rip");

// set the key "test" to "Sono un baka :3"        
Config::Global::Set("test", "Sono un baka :3");

// Clean up global config variables   
Config::Global::Dispose();
```

### Local Config

```cpp

std::string res; // get function returns only strings

// open a config file named "myfile"
// if it exist this loads the content
// Note: this class insance will be bindend to myfile untill disposed
ConfigFile conf("myfile");

// get the key "test"
res = conf.Get("test");   
 
// get the key "test", if missing return "rip"            
res = conf.Get("test", "rip");

// set the key "test" to "Sono un baka :3"        
conf.Set("test", "Sono un baka :3");


```

## Limitations

* Only top level paths are allowed
* Only string values for both input and output