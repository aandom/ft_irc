#pragma once

#include "error_codes.hpp"

class ErrorCode {
    public:
        ErrorCode();
        ~ErrorCode();
        ErrorCode(ErrorCode const &src);
        ErrorCode &operator=(ErrorCode const &src);

        std::map<int, std::string> _errmsgs;

    private:
};