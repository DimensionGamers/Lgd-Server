#ifndef BASE64_H
#define BASE64_H

class base64
{
	public:
		static std::string encode(uint8 const* , size_t len);
		static std::string decode(std::string const& s);
		static bool is_base64(unsigned char c);

		static const std::string chars;
};

#endif