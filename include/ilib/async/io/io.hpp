#ifndef G64748384848389_H637373939309499_C7484849404040040494
#define G64748384848389_H637373939309499_C7484849404040040494
#include <cstddef>
#include <ilib/async/executor.hpp>
#include <ilib/async/task.hpp>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#ifdef __unix__
#define ILIB_IO_UNIX
#elif defined(_WIN32) || defined(_WIN64) || defined(WINDOWS)
#define ILIB_IO_WIN
#endif

// :rofl: use winbloats's overlapped and lynix's epoll cuz this is dumb and stupid and not efficient

namespace ilib::io {
using read_buffer = std::span<const std::byte>;
using write_buffer = std::span<std::byte*>;
enum class host { Unix, Windows };
host get_host() {
#ifdef ILIB_IO_UNIX
    return host::Unix;
#elif defined(ILIB_IO_WIN)
    return host::Windows;
#else
    throw std::runtime_error("ilib::io::get_host: unkown OS");  // :v, add a pretty exception macro later
#endif
}
struct path {
    path(std::string_view p) : value{p}, h{get_host()}, sep{h == host::Unix ? '/' : '\\'} {}
    path(const path& p) : value{p.value}, h{get_host()}, sep{h == host::Unix ? '/' : '\\'} {}

    path operator=(std::string_view new_) {
        value = new_;
        return *this;
    }

    path operator=(const path& new_) {
        value = new_.value;
        return *this;
    }
    path operator/(const std::string& sub) {
        if (value.empty()) return path(value + sub);
        return path((value + sep) + sub);
    }

    path operator/(const path& other) {
        if (value.empty()) return path(value + other.value);
        return path((value + sep) + other.value);
    }

    path& operator/=(const std::string& sub) {
        if (value.empty()) value += sub;
        (value += sep) += sub;
        return *this;
    }

    path& operator/=(const path& other) {
        if (value.empty()) value += other.value;
        (value += sep) += other.value;
        return *this;
    }

    std::string extension() {
        auto size = value.rfind('.');
        auto sep_pos = value.find('/');
        if (size != std::string::npos && size < sep_pos) return value.substr(size, value.size());
        return {};
    }

    std::string filename() {
        auto size = value.rfind(sep);
        if (size != std::string::npos) return value.substr(size, value.size());
        return {};
    }

    std::string parent_path() {
        auto size_p = value.rfind(sep);
        auto size = value.rfind(sep, size_p + 1);
        if (size != std::string::npos) return value.substr(size_p, size);
        return {};
    }

    std::string root_path() {
        auto size = value.find(sep);
        if (size != std::string::npos) return value.substr(0, size);
        return {};
    }

    friend std::ostream& operator<<(std::ostream& os, const path& p) {
        return (os << p.to_string());
    }

    std::string to_string() const {
        return value;
    }

   private:
    std::string value;
    host h;
    char sep;
};

using namespace std::literals::string_literals;
#ifdef ILIB_IO_WIN
#include <windows.h>
using file_type = HANDLE;

enum class mode { read = OF_READ, write = OF_WRITE, read_write = OF_READWRITE };

const static HANDLE STDIN = GetStdHandle(STD_INPUT_HANDLE);
const static HANDLE STDOUT = GetStdHandle(STD_OUTPUT_HANDLE);
const static HANDLE STDERR = GetStdHandle(STD_ERROR_HANDLE);

ilib::task<bool> path_exists(const path& p) {
    co_return co_await ilib::async_run([&p]() -> bool { return PathFileExists(p.to_string().c_str()); });
}
ilib::task<path> get_current_directory() {
    co_return co_await ilib::async_run([]() -> ilib::Result<path, std::string> {
        TCHAR* buffer[MAX_PATH];
        auto out = GetCurrentDirecotry(MAX_PATH, buffer);
        auto err = GetLastError();
        if (err == 0) {
            char buf[MAX_PATH];
            std::wcstombs(buf, buffer, MAX_PATH);
            return path(buf);
        } else {
            LPSTR buff;
            std::size_t size = FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
                err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buff, 0, nullptr);
            throw std::runtime_error("ilib::io::get_current_directory: "s + std::string(buff, size));
        }
    });
}

ilib::task<void> set_current_directory(const path& p) {
    co_await ilib::async_run([&p] {
        SetCurrentDirectory(p.to_string());
        if (auto err = GetLastError()) {
            LPSTR buff;
            std::size_t size = FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
                err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buff, 0, nullptr);
            throw std::runtime_error("ilib::io::set_current_directory: "s + std::string(buff, size));
        }
    });
}
ilib::task<std::size_t> get_file_size(const path& p) {
    co_return co_await ilib::async_run([&p] {
        HANDLE hdl = OpenFile(p.to_string().c_str(), OP_READ);

        if (auto err = GetLastError()) {
            LPSTR buff;
            std::size_t size = FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr,
                err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buff, 0, nullptr);
            throw std::runtime_error("ilib::io::get_file_size: "s + std::string(buff, size));
        }
        return GetFileSize(hdl, nullptr);
    });
}

#elif defined(ILIB_IO_UNIX)
using file_type = int;
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <climits>

enum class mode { read = O_RDONLY, write = O_WRONLY, read_write = O_RDWR, append = O_APPEND };

constexpr static int STDIN = 0;
constexpr static int STDOUT = 1;
constexpr static int STDERR = 2;

ilib::task<bool> path_exists(const path& p) {
    co_return co_await ilib::async_run([&p]() -> bool { return access(p.to_string().c_str(), F_OK) == 0; });
}

ilib::task<path> get_current_directory() {
    co_return co_await ilib::async_run([]() {
        char buf[PATH_MAX];
        auto res = getcwd(buf, PATH_MAX - 1);
        if (res == nullptr) {
            throw std::runtime_error("get_current_directory: "s + std::strerror(errno));
        } else {
            return path(buf);
        }
    });
}

ilib::task<void> set_current_directory(const path& p) {
    co_await ilib::async_run([&p]() {
        if (chdir(p.to_string().c_str())) {
            throw std::runtime_error("set_current_directory: "s + std::strerror(errno));
        }
    });
}

ilib::task<std::size_t> get_file_size(const path& p) {
    co_return co_await ilib::async_run([&p] {
        struct stat st;
        stat(p.to_string().c_str(), &st);
        return st.st_size;
    });
}

ilib::task<file_type> async_open(const path& p, ilib::io::mode m) {
    co_return co_await ilib::async_run([&p, &m] {
        file_type out = open(p.to_string().c_str(), static_cast<int>(m));
        if (auto err = errno; out == -1) { throw std::runtime_error("ilib::io::open: "s + std::strerror(err)); }
        return out;
    });
}
ilib::task<void> async_write(file_type f, read_buffer buffer, std::size_t size) {
    co_return co_await ilib::async_run([&f, &buffer, &size] {
        auto status = write(f, static_cast<const void*>(buffer.data()), size);
        if (auto err = errno; status == -1) throw std::runtime_error("ilib::io::async_write: "s + std::strerror(errno));
    });
}
ilib::task<write_buffer> async_read(file_type f, std::size_t size) {
    co_return co_await ilib::async_run([&f, &size] {
        char* buffer = new char[size];
        if (read(f, buffer, size) == -1) { throw std::runtime_error("ilib::io::async_read: "s + std::strerror(errno)); }
        write_buffer buf(buffer, buffer + size);
        delete[] buffer;
        return buf;
    });
}
#endif
ilib::task<void> print(std::span<char> str) {
    co_await async_write(STDOUT, std::as_bytes(str), str.size());
}

template <std::size_t N>
ilib::task<void> print(const char (&arr)[N]) {
    co_await async_write(STDOUT, std::as_bytes(std::span<const char>(arr, arr + N)), N);
}

ilib::task<void> print(std::string_view str) {
    co_await async_write(STDOUT, std::as_bytes(std::span<const char>(str)), str.size());
}

ilib::task<void> print(std::string str) {
    co_await async_write(STDOUT, std::as_bytes(std::span<const char>(str)), str.size());
}

ilib::task<void> print(char c) {
    co_await print(std::string(1, c));
}

ilib::task<void> print(const path& p) {
    co_await print(p.to_string());
}

template <class... T>
ilib::task<void> print(T... args) {
    (co_await print(args), ...);
}
}  // namespace ilib::io
#endif
