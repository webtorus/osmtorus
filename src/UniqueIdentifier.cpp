#include "include/UniqueIdentifier.hpp"

long UniqueIdentifier::next()
{
	static long id = 0;

	return --id;
}
