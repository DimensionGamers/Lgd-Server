#ifndef MINI_DUMP_H
#define MINI_DUMP_H

#ifdef WIN32

LONG WINAPI unhandled_handler(struct _EXCEPTION_POINTERS* apExceptionInfo);

class MiniDump
{
	SingletonInstance(MiniDump);

	public:
		MiniDump();
		virtual ~MiniDump();

		void Start(std::string const& name);

		std::string const GetName() const { return this->name; };
	private:
		std::string name;
};

#endif // WIN32

#endif