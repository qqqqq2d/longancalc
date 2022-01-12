#ifndef _INTERFACE_H_
#define _INTERFACE_H_

template<typename Impl>
struct interface
{
	Impl & obj;
	interface(Impl& obj) : obj(obj)
	{
	}
	char get_key()
	{
		return obj.get_key();
	}
};

#endif //_INTERFACE_H_
