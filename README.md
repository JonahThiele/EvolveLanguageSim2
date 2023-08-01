# EvolveLanguageSim2

A basic terminal program that simulates lexicological change from an inputed xml file of base words

## Compiling from source
Download the code and run the makefile, the code is all header based so no additional libraries are
required besides normal c++.

**Note: This is required for windows and mac.**

## Running on windows

Their is currenly an executable of the program that I have only tested on windows 10. It is located
in the builds folder.

## Usage

You first need to generate words, you can either manually create a dictionary or generate a new one
with the dictionary creator found in the InputDictionaries folder

```
./DictCreator -m 15 -a 20 -f dictionary.xml
```

These options set the max length of the words, the amount of words, and the name of the output dictionary
more info can be found with 
```
./Dictionary -h
```

After you have created at least 3 dictionaries you can run the program
you can run the program

```
./EvolveLang -g 20 -s 300 -d1 dict1 -d2 dict2 -d3 dict3
```
These arguments set the amount of generations, the amound of speakers and the 3 required dictionaries.
If you want more information about the arguments run
```
./EvolveLang -h
```
**Note: You need to run the program in place so that it has access to the prefix and wordlists**

## Credits
This project relied heavily upon the [Pugi-xml progject](https://github.com/zeux/pugixml),
and the [c++ indicators project](https://github.com/p-ranav/indicators).

If anyone is interested in helping improve the program you feedback and support would be greatly
appreciated.

