// Copyright (c) 2018-2020, The TurtleCoin Developers
// Copyright (c) 2020, TRRXITTE inc. development Team
// Copyright (c) 2020, TRRXITTE inc. development Team
//
// Please see the included LICENSE file for more information

#pragma once

#include <string>

const std::string windowsAsciiArt = 
"\n||====================================================================||)\n"
  "||//$//////////////////////////////////////////////////////////////$//||\n"
  "||(100)==================| FEDERAL RESERVE NOTE |================(100)||\n"
  "||||$//        ~         '------========--------'                ||$//||\n"
  "||<< /        |$|              || ____ ||                         | >>||\n"
  "||>>|  12    ||X||            || ///..) ||         T44737133B   12 |<<||\n"
  "||<<|        || ||           || <||  >|  ||                        |>>||\n"
  "||>>|         |$|            ||  $$ --/  ||        One Hundred     |<<||\n"
  "||<<|      T44737133B        *||  ||_|  //* series                 |>>||\n"
  "||>>|  12                     *||/___|_//*   2020                  |<<||\n"
  "||<<|      Treasurer     ______|Franklin|________     Secretary 17 />>||\n"
  "||//$|                 ~|UNITED STATES OF AMERICA|~               /$||||\n"
  "||(100)===================  ONE HUNDRED traaitt =================(100)||\n"
  "||//$//////////////////////////////////////////////////////////////$//||\n"
  "||====================================================================||\n";

const std::string nonWindowsAsciiArt =
"\n||====================================================================||)\n"
  "||//$//////////////////////////////////////////////////////////////$//||\n"
  "||(100)==================| FEDERAL RESERVE NOTE |================(100)||\n"
  "||||$//        ~         '------========--------'                ||$//||\n"
  "||<< /        |$|              || ____ ||                         | >>||\n"
  "||>>|  12    ||X||            || ///..) ||         T44737133B   12 |<<||\n"
  "||<<|        || ||           || <||  >|  ||                        |>>||\n"
  "||>>|         |$|            ||  $$ --/  ||        One Hundred     |<<||\n"
  "||<<|      T44737133B        *||  ||_|  //* series                 |>>||\n"
  "||>>|  12                     *||/___|_//*   2020                  |<<||\n"
  "||<<|      Treasurer     ______|Franklin|________     Secretary 17 />>||\n"
  "||//$|                 ~|UNITED STATES OF AMERICA|~               /$||||\n"
  "||(100)===================  ONE HUNDRED traaitt =================(100)||\n"
  "||//$//////////////////////////////////////////////////////////////$//||\n"
  "||====================================================================||\n";


/* Windows has some characters it won't display in a terminal. If your ascii
   art works fine on Windows and Linux terminals, just replace 'asciiArt' with
   the art itself, and remove these two #ifdefs and above ascii arts */
#ifdef _WIN32

const std::string asciiArt = windowsAsciiArt;

#else
const std::string asciiArt = nonWindowsAsciiArt;
#endif
