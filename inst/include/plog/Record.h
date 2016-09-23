#pragma once
#include <sstream>
#include <plog/Util.h>
#include <plog/Severity.h>

namespace plog
{
    namespace detail
    {
        //////////////////////////////////////////////////////////////////////////
        // Stream output operators as free functions

        inline void operator<<(util::nstringstream& stream, const char* data)
        {
            data = data ? data : "(null)";

            std::operator<<(stream, data);
        }

        inline void operator<<(util::nstringstream& stream, const std::string& data)
        {
            plog::detail::operator<<(stream, data.c_str());
        }
    }

    class Record
    {
    public:
        Record(Severity severity, const char* func, size_t line, const void* object)
            : m_severity(severity), m_object(object), m_line(line), m_func(func)
        {
            util::ftime(&m_time);
        }

        //////////////////////////////////////////////////////////////////////////
        // Stream output operators

        Record& operator<<(char data)
        {
            char str[] = { data, 0 };
            *this << str;
            return *this;
        }

#ifndef __ANDROID__
        Record& operator<<(wchar_t data)
        {
            wchar_t str[] = { data, 0 };
            *this << str;
            return *this;
        }
#endif

        template<typename T>
        Record& operator<<(const T& data)
        {
            using namespace plog::detail;

            m_message << data;
            return *this;
        }

        //////////////////////////////////////////////////////////////////////////
        // Getters

        const util::Time& getTime() const
        {
            return m_time;
        }

        Severity getSeverity() const
        {
            return m_severity;
        }

        const void* getObject() const
        {
            return m_object;
        }

        size_t getLine() const
        {
            return m_line;
        }

        const util::nstring getMessage() const
        {
            return m_message.str();
        }

        std::string getFunc() const
        {
            return util::processFuncName(m_func);
        }

    private:
        util::Time          m_time;
        const Severity      m_severity;
        const void* const   m_object;
        const size_t        m_line;
        util::nstringstream m_message;
        const char* const   m_func;
    };
}
