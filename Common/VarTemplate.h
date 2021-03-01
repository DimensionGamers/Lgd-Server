#ifndef VAR_TEMPLATE_H
#define VAR_TEMPLATE_H

#pragma pack(1)
template <typename type>

struct declare
{
private:
	type data;
public:
	declare<type>& operator=(declare<type> const& that)
	{
		this->data = that.data;
		return *this;
	}

	bool operator !() const { return this->data == 0; }
	//type operator const () { return this->data; }

	bool operator != (const declare<type> & _that) const { return this->data != _that.data; }
	bool operator == (const declare<type> & _that) const { return this->data == _that.data; }
	bool operator <  (const declare<type> & _that) const { return this->data < _that.data; }
	bool operator <= (const declare<type> & _that) const { return this->data <= _that.data; }
	bool operator >  (const declare<type> & _that) const { return this->data > _that.data; }
	bool operator >= (const declare<type> & _that) const { return this->data >= _that.data; }
	type operator %  (const declare<type> & _that) const { return this->data % _that.data; }
	type operator /  (const declare<type> & _that) const { return (!_that.data ? 0: this->data / _that.data); }
	type operator &  (const declare<type> & _that) const { return this->data & _that.data; }

	bool operator != (const type & _that) const { return this->data != _that; }
	bool operator == (const type & _that) const { return this->data == _that; }
	bool operator <  (const type & _that) const { return this->data < _that; }
	bool operator <= (const type & _that) const { return this->data <= _that; }
	bool operator >  (const type & _that) const { return this->data > _that; }
	bool operator >= (const type & _that) const { return this->data >= _that; }
	type operator %  (const type & _that) const { return this->data % _that; }
	type operator /  (const type & _that) const { return (!_that ? 0: this->data / _that); }
	type operator &  (const type & _that) const { return this->data & _that; }


	type get() const { return this->data; }
	void set(type d) { this->data = type(d); }

	type op_inc() { this->data++; return this->get(); }
	type op_dec() { this->data--; return this->get(); }

	template <typename id>
	void op_add(id value) { this->data = this->data + value; }
	template <typename id>
	void op_dec(id value) { this->data = this->data - value; }
	template <typename id>
	void op_mult(id value) { this->data = this->data * value; }
	template <typename id>
	void op_div(id value) { this->data = this->data / value; }
	template <typename id>
	void op_and(id value) { this->data = this->data & value; }
	template <typename id>
	void op_or(id value) { this->data = this->data | value; }
	template <typename id>
	void op_percent(id value) { if ( value ) this->data += (this->data * value / 100); }
	bool Is(type value) const { return this->data == value; }

	void limit_max(type value) { if ( this->data > value ) this->data = value; }
	void limit_min(type value) { if ( this->data < value ) this->data = value; }

	/*bool in(std::vector<type> list)
	{
		for ( std::vector<type>::const_iterator it = list.begin(); it != list.end(); ++it )
		{
			if ( this->data != *it )
				continue;

			return true;
		}

		return false;
	}*/
};

namespace Util
{
	template <typename type1,typename type2>
	void memset(declare<type1> * dest, type2 value, uint32 elements)
	{
		for ( uint32 i = 0; i < elements; ++i )
		{
			dest[i].set(value);
		}
	}

	template <typename type1,typename type2>
	void copy(declare<type1> * dest, const declare<type2> * source, uint32 elements)
	{
		for ( uint32 i = 0; i < elements; ++i )
		{
			dest[i].set(source[i].get());
		}
	}

	template <typename type1,typename type2>
	void memcpy(declare<type1> * dest, const type2 * source, uint32 elements)
	{
		if ( !source )
			return;

		for ( uint32 i = 0; i < elements; ++i )
		{
			dest[i].set(source[i]);
		}
	}

	template <typename type1,typename type2>
	void memcpy(type1 * dest, declare<type2> const* source, uint32 elements)
	{
		if ( !source )
			return;

		for ( uint32 i = 0; i < elements; ++i )
		{
			dest[i] = source[i].get();
		}
	}

	template <typename type1,typename type2>
	bool memcmp(declare<type1> * var1, declare<type2> * var2, uint32 elements)
	{
		for ( uint32 i = 0; i < elements; ++i )
		{
			if ( var1[i].get() == var2[i].get() )
				continue;

			return true;
		}

		return false;
	}

	template <typename type1,typename type2>
	bool memcmp(declare<type1> * var1, type2 * var2, uint32 elements)
	{
		for ( uint32 i = 0; i < elements; ++i )
		{
			if ( var1[i].get() == var2[i] )
				continue;

			return true;
		}

		return false;
	}
}

#define DECLARE(x,y) declare<x> y

#pragma pack()

#endif