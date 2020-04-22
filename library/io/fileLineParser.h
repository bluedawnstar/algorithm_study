#pragma once

#include <stdio.h>
#include <string>

class FileLineParser {
public:
                        FileLineParser();
    explicit            FileLineParser(const std::string& file_path);
                        ~FileLineParser();

            bool        open(const std::string& file_path);
            void        close();

            bool        is_open() const;

            bool        prepare();

            bool        read_string(std::string& value);
            bool        read_int32(int& value);
            bool        read_int64(long long& value);
            bool        read_uint32(unsigned int& value);
            bool        read_uint64(unsigned long long& value);

private:
            FileLineParser(const FileLineParser&) = delete;
            FileLineParser(FileLineParser&&) = delete;
            FileLineParser operator =(const FileLineParser&) = delete;
            FileLineParser operator =(FileLineParser&&) = delete;

private:
            FILE*       fp;
            char*       buff;
            char*       p;

    static  const int MAX_LINE_LENGTH = 10 * 1024;
};

inline FileLineParser::FileLineParser()
    : fp(nullptr), buff(new char[MAX_LINE_LENGTH]), p(buff) {
}

inline FileLineParser::FileLineParser(const std::string& file_path)
    : fp(nullptr), buff(new char[MAX_LINE_LENGTH]), p(buff) {
    open(file_path);
}

inline FileLineParser::~FileLineParser() {
    close();
    delete[] buff;
}

inline bool FileLineParser::open(const std::string& file_path) {
    close();

    fp = fopen(file_path.c_str(), "rt");
    if (fp == nullptr)
        return false;
    return true;
}

inline void FileLineParser::close() {
    if (fp) {
        fclose(fp);
        fp = nullptr;
    }
}

inline bool FileLineParser::is_open() const {
    return fp != nullptr;
}

inline bool FileLineParser::prepare() {
    if (fp == nullptr)
        return false;

    if (fgets(buff, MAX_LINE_LENGTH, fp) == nullptr)
        return false;

    p = buff;
    while (*p && isspace(*p))
        p++;

    return *p != '\0';
}

inline bool FileLineParser::read_string(std::string& value) {
    while (*p && isspace(*p))
        p++;

    auto* s = p;
    while (*p && !isspace(*p))
        *p++;

    if (s == p)
        return false;

    value.assign(s, p - s);
    return true;
}

inline bool FileLineParser::read_int32(int& value) {
    unsigned int x = 0, sign = 0;

    while (*p && isspace(*p))
        p++;

    if (*p == '\0')
        return false;

    if (*p == '-') {
        sign = ~sign;
        p++;
    } else if (*p == '+') {
        p++;
    }

    for (; *p && !isspace(*p); p++) {
        if (*p < '0' || *p > '9')
            return false;
        x = x * 10 + (*p - '0');
    }
    value = static_cast<int>((x ^ sign) + !!sign);

    return true;
}

inline bool FileLineParser::read_int64(long long& value) {
    unsigned long long x = 0, sign = 0;

    while (*p && isspace(*p))
        p++;

    if (*p == '\0')
        return false;

    if (*p == '-') {
        sign = ~sign;
        p++;
    } else if (*p == '+') {
        p++;
    }

    for (; *p && !isspace(*p); p++) {
        if (*p < '0' || *p > '9')
            return false;
        x = x * 10 + (*p - '0');
    }
    value = static_cast<long long>((x ^ sign) + !!sign);

    return true;
}

inline bool FileLineParser::read_uint32(unsigned int& value) {
    unsigned int x = 0;

    while (*p && isspace(*p))
        p++;

    if (*p == '\0')
        return false;

    for (; *p && !isspace(*p); p++) {
        if (*p < '0' || *p > '9')
            return false;
        x = x * 10 + (*p - '0');
    }
    value = x;

    return true;
}

inline bool FileLineParser::read_uint64(unsigned long long& value) {
    unsigned long long x = 0;

    while (*p && isspace(*p))
        p++;

    if (*p == '\0')
        return false;

    for (; *p && !isspace(*p); p++) {
        if (*p < '0' || *p > '9')
            return false;
        x = x * 10 + (*p - '0');
    }
    value = x;

    return true;
}
