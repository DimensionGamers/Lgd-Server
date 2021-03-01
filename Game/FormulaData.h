#ifndef FORMULA_DATA_H
#define FORMULA_DATA_H

namespace Numerics{
	template<class T>
	class Numerical{

	private:
		T number;
	public:
		Numerical(T value = 0) : number(value){}
		Numerical(const std::string& arg){
			const_cast<Numerical&>(*this)(arg);
		}
		/**
		* Attempts to assign the argument value to the value
		* of this Object's type.
		* If the value is invalid, nothing happens.
		*/
		std::string operator()(const std::string& arg){
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			try{
				ss << arg;
				ss >> number;
			}
			catch (...){
				// currently unsupported
			}
			return ss.str();
		}
		/**
		* Attempts to assign the argument value to the value of
		* this Object's type.
		*/
		T operator()(T value){
			return (number = value);
		}
		/**
		* Returns a string representation of this Object's number
		*/
		std::string getString(){
			std::stringstream ss(std::stringstream::in | std::stringstream::out);
			ss << number;
			return ss.str();
		}
		/**
		* Returns a copy of this Object's number
		*/
		T getValue(){
			return number;
		}
		/**
		* Extraction operator used to return the underlying value
		* during operations assosciated with primitive types.
		*/
		operator T& (){
			return number;
		}
		/**
		* const version of the above operator. Returns a copy
		* of this Object's number.
		*/
		operator T () const{
			return number;
		}
	};
	/* Some meaningful typedefs for Numerical types */
	typedef Numerical<short> Short;
	typedef Numerical<unsigned short> U_Short;
	typedef Numerical<int> Integer;
	typedef Numerical<unsigned int> U_Integer;
	typedef Numerical<double> Double;
	typedef Numerical<float> Float;
	typedef Numerical<char> Byte;
	typedef Numerical<unsigned char> U_Byte;
	typedef Numerical<wchar_t> Wide_Byte;
	typedef Numerical<long int> Long;
	typedef Numerical<unsigned long int> U_Long;
	/* For non-standard types, like __int8, __int16, __int32, and __int64 */
#ifdef ALLOW_NONSTANDARD_PRIMITIVE_TYPES
#if (ALLOW_NONSTANDARD_PRIMITIVE_TYPES == 0x01)
	typedef Numerical < __int8 > __Int8;
	typedef Numerical < unsigned __int8 > U___Int8;
	typedef Numerical < __int16 > __Int16;
	typedef Numerical < unsigned __int16 > U___Int16;
	typedef Numerical < __int32 > __Int32;
	typedef Numerical < unsigned __int32 > U___Int32;
	typedef Numerical < __int64 > __Int64;
	typedef Numerical < unsigned __int64 > U___Int64;
#endif
#endif
}

namespace EquationHelper
{
	class EquationSolver
	{
	private:
		EquationSolver();
		static std::string doOperation(const std::string&, char, const std::string&);
		static void correctedString(std::string&);
		static void removeSpaces(std::string&);
		static std::string parse(const std::string&);
		static bool isSolvable(const std::string&);
		static void calculate(std::vector<std::string>&, std::vector<char>&, const std::string&);
	public:
		static std::string solve(const std::string&, int = 50);
	};
}

typedef std::map<int32, float> FormulaDataValueMap;

struct FormulaData
{
	DECLARE_ENUM(uint8, Type);
	DECLARE_ENUM(uint16, ID);
	DECLARE_PROPERTY_STRING(Formula);

	FormulaDataValueMap m_ValueMap;
};

typedef std::vector<FormulaData*> FormulaList;

class FormulaMgr
{
	SingletonInstance(FormulaMgr);

	public:
		FormulaMgr();
		virtual ~FormulaMgr();

		void Load();
		float GetValue(uint8 type, uint16 id, int32 variable);
		float GetOrCreateValue(uint8 type, uint16 id, int32 variable);
	private:
		FormulaList m_FormulaList;
};

#endif