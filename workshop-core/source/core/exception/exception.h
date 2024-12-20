#pragma once

#include <stdexcept>
#include <string>
#include <cstdarg>

#include "core/preprocessor/api.h"

namespace wk
{
	class Exception : public std::exception
	{
	public:
		Exception() = default;
		Exception(const std::string& message) : m_message(message) { }

		Exception(const char* format, ...);

		virtual ~Exception() = default;

	public:
		const char* what() const noexcept override { return m_message.c_str(); }

	protected:
		void proceed_vargs(const char* format, va_list args);

	protected:
		std::string m_message;
	};
}
