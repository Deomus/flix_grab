#ifndef __RAND_H
#define __RAND_H

#include "bigint.h"

class random {
public:
	random();
	~random();

	unsigned rand( unsigned max );
	bigint rand( bigint & max );

protected:
	void * m_hCryptProv;
};

#endif
