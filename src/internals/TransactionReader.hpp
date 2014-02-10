#pragma once

#include <cstdint>
using namespace std;

namespace internals {

class TransactionReader
{
public:
	virtual ~TransactionReader() {};
    virtual int32_t getTransactionSupport() = 0;
    virtual void getTransactionBounds(int32_t* &begin, int32_t* &end) = 0;
};

}
