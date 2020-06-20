// Copyright (c) 2018-2020, The TurtleCoin Developers // Copyright (c) 2020, TRRXITTE inc.
//
// Please see the included LICENSE file for more information

#pragma once

#include <string>

const std::string windowsAsciiArt = 
"\n||====================================================================||\n"
"||////////////////////////////////////////////////////////////////////||\n"
"|| 100$ =================| FEDERAL RESERVE NOTE |=============== 100$ ||\n"
"|||||                    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                  |||||\n"
"||<<|    TRRXITTE inc.                                             |>>||\n"
"||>>|                          88888888888         T44737133B      |<<||\n"
"||<<|                              $$$                             |>>||\n"
"||>>|                              888                             |<<||\n"
"||<<|      T44737133B              $$$             One Hundred     |>>||\n"
"||>>|                                                              |<<||\n"
"||<<|      Treasurer     ________________________     Secretary 17 |>>||\n"
"|||||                  ~|UNITED STATES OF AMERICA|~                |||||\n"
"|| 100$ ==================  ONE HUNDRED traaitt ================ 100$ ||\n"
"||////////////////////////////////////////////////////////////////////||\n"
"||====================================================================||\n";

const std::string nonWindowsAsciiArt =
"\n||====================================================================||\n"
"||////////////////////////////////////////////////////////////////////||\n"
"|| 100$ =================| FEDERAL RESERVE NOTE |=============== 100$ ||\n"
"|||||                    ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                  |||||\n"
"||<<|    TRRXITTE inc.                                             |>>||\n"
"||>>|                          88888888888         T44737133B      |<<||\n"
"||<<|                              $$$                             |>>||\n"
"||>>|                              888                             |<<||\n"
"||<<|      T44737133B              $$$             One Hundred     |>>||\n"
"||>>|                                                              |<<||\n"
"||<<|      Treasurer     ________________________     Secretary 17 |>>||\n"
"|||||                  ~|UNITED STATES OF AMERICA|~                |||||\n"
"|| 100$ ==================  ONE HUNDRED traaitt ================ 100$ ||\n"
"||////////////////////////////////////////////////////////////////////||\n"
"||====================================================================||\n";

/* Windows has some characters it won't display in a terminal. If your ascii
   art works fine on Windows and Linux terminals, just replace 'asciiArt' with
   the art itself, and remove these two #ifdefs and above ascii arts */
#ifdef _WIN32

const std::string asciiArt = windowsAsciiArt;

#else
const std::string asciiArt = nonWindowsAsciiArt;
#endif
