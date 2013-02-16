#ifndef UNIQUE_IDENTIFIER_HPP
#define UNIQUE_IDENTIFIER_HPP

class UniqueIdentifier
{
public:
	static long next()
	{
		static long id = 0;

		return --id;
	}
};

#endif
