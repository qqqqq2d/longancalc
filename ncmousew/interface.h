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
	void debug_key(char key, bool found)
	{
		obj.debug_key(key, found);
	}
	void debug_mem_write(double mem_var)
	{
		obj.debug_mem_write(mem_var);
	}
	void show_stored(int index, double mem_var)
	{
		obj.show_stored(index, mem_var);
	}
	void show_mem_read(int index, double mem_var)
	{
		obj.show_mem_read(index, mem_var);
	}
	void show_unary_result(char* operation, double result)
	{
		obj.show_unary_result(operation, result);
	}
	void show_result(double result)
	{
		obj.show_result(result);
	}
};

#endif //_INTERFACE_H_
