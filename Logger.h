#pragma once
#include <string>
#include <memory>
#include <vector>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/fmt/bundled/format.h>

namespace spdlog { class logger; }

class Logger
{
private:
	Logger();
	~Logger();
	std::shared_ptr<spdlog::logger> logger_;
	static constexpr const char* LOG_FILE_PATH	= "logs/server.log";
	static constexpr size_t MAX_FILE_SIZE		= 5 * 1024 * 1024;
	static constexpr size_t MAX_FILE_COUNT		= 3;
	static constexpr const char* LOGGER_NAME	= "server_logger";
	static constexpr const char* PATTERN		= "[%Y-%m-%d %H-%M-%S.%e] [%^%1%$] [thread %t] %v";
	
public:
	static Logger& instance();
	Logger(const Logger&) = delete;
	Logger& operator(const Logger&) = delete;
	
	void trace(const string& message);
	void debug(const string& message);
	void info(const string& message);
	void warn(const string& message);
	void error(const string& message);
	void critical(const string& message);
	
	// Принимает строку с {} и любое количество аргументов любых типов
	template<typename... Args> void trace(const string& str, Args&&...)
	{
		/* 
		 * fmt::format подставляет args вместо {} в format и возвращает готовую строку.
		 * std::forward<Args>(args)... передаёт каждый аргумент без лишнего копирования.
		 * После этого вызывается обычный trace(const std::string&) для записи в лог.
		
						Пример использования
						
		 * Logger::instance().info("Порт {}, пользователь {}", 8080, "Alice");
		 * внутри превратится в: info(fmt::format("Порт {}, пользователь {}", 8080, "Alice"))
		 * результат: "Порт 8080, пользователь Alice" – и запись в лог.
		 */
		trace(fmt::format(str, std::forward<Args>(args)...));
	}
	template<typename... Args> void debug(const string& str, Args&&...)
	{
		debug(fmt::format(str, std::forward<Args>(args)...));
	}
	template<typename... Args> void info(const string& str, Args&&...)
	{
		info(fmt::format(str, std::forward<Args>(args)...));
	}
	template<typename... Args> void warn(const string& str, Args&&...)
	{
		warn(fmt::format(str, std::forward<Args>(args)...));
	}
	template<typename... Args> void error(const string& str, Args&&...)
	{
		error(fmt::format(str, std::forward<Args>(args)...));
	}
	template<typename... Args> void critical(const string& str, Args&&...)
	{
		critical(fmt::format(str, std::forward<Args>(args)...));
	}
	
};
