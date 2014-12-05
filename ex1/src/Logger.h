/*
 * Logger.cpp
 *
 *  Created on: Oct 6, 2014
 *      Author: ixi
 */
#include <iostream>
#define LOG Log(true)

#define LOG_NOEND Log(false)

// Set here if you want logging messages to be printed
const bool enableLogger = true;

class Log
{
public:
	Log(bool end) :
			end(end)
	{
	}
	~Log()
	{
		if (end && enableLogger)
		{
			// Add an newline.
			std::cout << std::endl;
		}
	}

	template<typename T>
	Log &operator <<(const T &t)
	{
		if (enableLogger)
		{
			std::cout << t;
		}
		return *this;
	}
private:
	bool end;

};
