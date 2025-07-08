
#ifndef SC_LOGGING_HPP
#define SC_LOGGING_HPP

#include "snowcrash/container/allocator.hpp"
#include "snowcrash/container/arraylist.hpp"
#include "snowcrash/container/string.hpp"
#include <format>
#include <fstream>
#include <snowcrash/core/core.hpp>
#include <string>

namespace SC {

enum LogLevel {
	LogLevel_Trace,
	LogLevel_Info,
	LogLevel_Warn,
	LogLevel_Error
};

struct LogData {
	const char *fileName;
	const int lineNumber;
	LogLevel level;
};

class LogSink {
public:
	LogSink(Allocator *allocator);
	~LogSink();

public:
	virtual void log(const LogData &data, const char *message, char *logName) = 0;

protected:
	Allocator *m_allocator;
};

class LogSinkColorCout : public LogSink {
public:
	LogSinkColorCout(Allocator *allocator);
	~LogSinkColorCout();

	void log(const LogData &data, const char *message, char *logName);
};

class LogSinkFile : public LogSink {
public:
	LogSinkFile(Allocator *allocator, const char *path);
	~LogSinkFile();

	void log(const LogData &data, const char *message, char *logName);

private:
	std::fstream m_file;
};

class Log {
public:
	Log(Allocator *allocator, const char *name, LogLevel level);
	~Log();

public:
	template<typename ...Fmt>
	void log(const LogData &data, const char *message, Fmt &&...fmt) {
		if(data.level < m_level) return;

		std::string fmess = std::vformat(message, std::make_format_args(fmt...));

		m_logSinks.for_each([&](LogSink *sink, u32 i){
			sink->log(data, fmess.c_str(), m_logName.c_str());
		});
	}

	void log(const LogData &data, const char *message) {
		m_logSinks.for_each([&](LogSink *sink, u32 i){
			sink->log(data, message, m_logName.c_str());
		});
	}

	template<typename T, typename ...Args>
	LogSink *add_sink(Args &&...args) {
		return m_logSinks.add(
			m_allocator->allocate<T>(1, 
				m_allocator, std::forward<Args>(args)...));
	}

public:
	static Log *get_core_log();

private:
	ArrayList<LogSink*> m_logSinks;
	String m_logName;
	LogLevel m_level;

	Allocator *m_allocator;

private:
	static Allocator s_logAllocator;
	static Log s_coreLog;
};

constexpr const char *log_level_to_string(enum LogLevel level) {
	switch (level) {
		case LogLevel_Trace: return "trace";
		case LogLevel_Info: return "info";
		case LogLevel_Warn: return "warn";
		case LogLevel_Error: return "error";
	}
}

}


#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define SC_LOG(l, level, ...) l.log(LogData{__FILENAME__, __LINE__, level}, __VA_ARGS__)

#ifdef SC_LOGGING

#define SC_CORE_TRACE(...) Log::get_core_log()->log(LogData{__FILENAME__, __LINE__, LogLevel_Trace}, __VA_ARGS__)
#define SC_CORE_INFO(...) Log::get_core_log()->log(LogData{__FILENAME__, __LINE__, LogLevel_Info}, __VA_ARGS__)
#define SC_CORE_WARN(...) Log::get_core_log()->log(LogData{__FILENAME__, __LINE__, LogLevel_Warn}, __VA_ARGS__)
#define SC_CORE_ERROR(...) Log::get_core_log()->log(LogData{__FILENAME__, __LINE__, LogLevel_Error}, __VA_ARGS__)

#else

#define SC_CORE_TRACE(...)
#define SC_CORE_INFO(...)
#define SC_CORE_WARN(...)
#define SC_CORE_ERROR(...)

#endif

#ifdef SC_ENABLE_ASSERT

#define SC_ASSERT(cond, ...) if(!(cond)) { \
								SC_CORE_ERROR("ASSERT FAILED :3 ({})", #cond); \
								SC_CORE_ERROR(__VA_ARGS__); \
								SC_CRASH; \
							}

#else

#define SC_ASSERT(cond, mess)

#endif

#endif

