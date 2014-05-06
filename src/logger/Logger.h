/*
 * Logger.h
 *
 *  Created on: 04.05.2014
 *      Author: Christian
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <ostream>
#include <string>
#include <ostream>
#include <fstream>

class Logger
{
public:
	Logger();
	~Logger() = default;

	// this is the type of std::cout
	typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

	// this is the function signature of std::endl
	typedef CoutType& (*StandardEndLine)(CoutType&);

	template<class T>
	Logger& operator<<(const T& str);

	Logger& operator<<(StandardEndLine manip);

private:
	std::ostream outstream;
	std::fstream file;

};

template<class T>
Logger& Logger::operator<<(const T& str)
{
	outstream << str;
	file << str;
	outstream.flush();
	return *this;
}

#endif /* LOGGER_H_ */
