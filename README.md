# Google Translator API

This project aims to create a c++ library for communication with the google translator API. It makes possible to make 
requests for translations for the API supported languages.

The need for this library comes from another project that I'm working on where I've needed
to translate sentences, I haven't found libraries supporting requests for the google translate API,
so I've decided to write it on my own.

## Example

```cpp
#include <iostream>
#include <gtranslator.h>

int main(int argc, char* argv[]){
    gtranslator::GTranslator translator;
    
    std::cout << translator.translate("olá mundo", "pt", "en");
    
    return 0;
}
```
**output:**
```
Hello World
```


## Installation

**Requirements:**
- LibCurl

On Debian based systems like ubuntu, you can install the requirements with apt:
```
sudo apt install -y libcurlpp-dev libcurl4
```
Install procedure with cmake:
``` 
mkdir build && cd build
cmake ..
make
sudo make install