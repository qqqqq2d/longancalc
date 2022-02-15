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
	void back_space(int r_index)
	{
		obj.back_space(r_index);
	}
	void clear(int r_index)
	{
		obj.clear(r_index);
	}
	void add_key(int cur_row, int r_index, char key)
	{
		obj.add_key(cur_row, r_index, key);
	}
	void debug_buf(char fun, int r_index, int buf_index, char buf[42], char a_buf[20], char b_buf[20])
	{
		obj.debug_buf(fun, r_index, buf_index, buf, a_buf, b_buf);
	}
	void end()
	{
		obj.end();
	}
	void show_a(char a_buf[20])
	{
		obj.show_a(a_buf);
	}
	void debug_handle_binary_operation(char fun, char * a_buf, char * b_buf, char * buf, int pos, double a, double b)
	{
		obj.debug_handle_binary_operation(fun, a_buf, b_buf, buf, pos, a, b);
	}
};

#endif //_INTERFACE_H_
