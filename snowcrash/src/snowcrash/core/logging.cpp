
#include "logging.hpp"
#include "snowcrash/container/allocator.hpp"
#include "snowcrash/core/core.hpp"
#include <ios>
#include <iostream>
#include <ostream>

namespace SC {

Allocator Log::s_logAllocator(nullptr);
Log Log::s_coreLog(&Log::s_logAllocator, "core", LogLevel_Trace);

Log::Log(Allocator *allocator, const char *name, LogLevel level)
	: m_allocator(allocator),
	  m_logName(allocator, name),
	  m_level(level),
	  m_logSinks(allocator) {}

Log::~Log() {
	m_logSinks.for_each([&](LogSink *sink, u32 i){
		m_allocator->free(sink);
	});
}

LogSink::LogSink(Allocator *allocator)
	: m_allocator(allocator) {}

LogSink::~LogSink() = default;

LogSinkColorCout::LogSinkColorCout(Allocator *allocator)
	: LogSink(allocator) {}

LogSinkColorCout::~LogSinkColorCout() = default;


LogSinkFile::LogSinkFile(Allocator *allocator, const char *path) 
	: LogSink(allocator) {

	m_file.open(path, std::ios::out);
}

LogSinkFile::~LogSinkFile() {
	m_file.close();
}

void LogSinkFile::log(const LogData &data, const char *message, char *logName) {
	m_file << "[" << logName << "]" << "[" << log_level_to_string(data.level) << "]"
			  << "[" << data.fileName << ":" << data.lineNumber << "]"
			  << " " << message << "\n";

	m_file.flush();
}

void LogSinkColorCout::log(const LogData &data, const char *message, char *logName) {
	const char *color;

	switch (data.level) {
		case LogLevel_Trace: { color = "\e[0;32m"; break; }
		case LogLevel_Info: { color = "\e[0;34m"; break; }
		case LogLevel_Warn: { color = "\e[0;35m"; break; }
		case LogLevel_Error: { color = "\e[0;31m"; break; }
	}

	std::cout << color
			  << "[" << logName << "]"
			  << "[" << data.fileName << ":" << data.lineNumber << "]"
			  << " " << message << "\e[0m" << std::endl;
}

Log *Log::get_core_log() {
	return &s_coreLog;
}

}

