#ifndef __EXCEPT_H
#define __EXCEPT_H

#include "licensing.h"

class KeyExceptionImpl : public Exception {
public:
	KeyExceptionImpl( int code, const char_t * message = NULL)
	{
		SetCode(code);
		if (message)
			SetMessage(message);
	}

	virtual ~KeyExceptionImpl() {}

	virtual int GetCode() { return m_code; }
	virtual const char_t * GetExceptionMessage() { return m_message.c_str(); }
	virtual void Destroy() { delete this; }
	void SetCode(int code) { m_code = code; }
	void SetMessage(const char_t * message) { m_message = message; }

protected:
	int m_code;
	string_t m_message;
};

#endif
