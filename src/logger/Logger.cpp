/*
 * Logger.cpp
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#include <iostream>
#include "Logger.h"

Logger::Logger() : outstream(std::cout.rdbuf()), file("log.txt", std::fstream::out)
{
}

Logger& Logger::operator<<(Logger::StandardEndLine manip)
{
	manip(outstream);
	manip(file);
	outstream.flush();
	file.flush();
	return *this;
}
