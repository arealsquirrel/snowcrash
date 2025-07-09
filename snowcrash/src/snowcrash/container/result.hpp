
#ifndef SC_RESULT_HPP
#define SC_RESULT_HPP

#include <snowcrash/core/core.hpp>

namespace SC {

/*
 * its a monad that can return either
 * a result of type A or an error code of type u16
 */
template<typename A>
struct Result {
public:
	enum ResultType {
		ResultType_filled,
		ResultType_null
	};

public:
	Result(A &result){
		m_resultType = ResultType_filled;
		m_result = result;
	}

	Result(const Result &result) {
		m_resultType = result.m_resultType;
	
		if(m_resultType == ResultType_null) {
			m_errorCode = result.get_error_code();
		} else {
			m_resultType = result.get_result();
		}
	}

	Result(u16 error) {
		m_resultType = ResultType_null;
		m_errorCode = error;
	}

public:
	A &get_result() const { return m_result; }
	u16 get_error_code() const { return m_errorCode; }
	ResultType get_result_type() const { return m_resultType; }

private:
	union {
		A m_result;
		u32 m_errorCode;
	};

	ResultType m_resultType;
};

}

#endif

