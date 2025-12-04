#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <source_location>
#include <mutex>

class Tracer {
private:
    std::string message_;
    std::chrono::system_clock::time_point start_time_;
    const std::source_location location_;
    static inline std::mutex output_mutex_;
    static inline thread_local std::vector<std::string> call_stack_;

public:
    explicit Tracer(const std::string& message = "",
        const std::source_location location = std::source_location::current())
        : message_(message),
        location_(location) {

        start_time_ = std::chrono::system_clock::now();

        if (!message_.empty()) {
            log("ENTER: " + message_);
        }
        else {
            log("ENTER: " + std::string(location_.function_name()));
        }
        std::string stack_entry;
        if (!message_.empty()) {
            stack_entry = message_;
        }
        else {
            stack_entry = location_.function_name();
        }
        call_stack_.push_back(stack_entry);
    }
    ~Tracer() {
        if (!call_stack_.empty()) {
            call_stack_.pop_back();
        }
        std::string exit_msg;
        if (!message_.empty()) {
            exit_msg = "EXIT: " + message_;
        }
        else {
            exit_msg = "EXIT: " + std::string(location_.function_name());
        }
        auto end_time = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time_);
        log(exit_msg + " [took " + std::to_string(duration.count()) + "ms]");
    }
    static void log(const std::string& message,
        const std::source_location location = std::source_location::current()) {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        std::lock_guard<std::mutex> lock(output_mutex_);
        std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
            << "." << std::setfill('0') << std::setw(3) << ms.count() << "] "
            << message << " [" << location.file_name()
            << ":" << location.line() << "]" << std::endl;
    }
    static void trace(const std::string& message,
        const std::source_location location = std::source_location::current()) {
        log(message, location);
    }
    static void print_call_stack() {
        std::lock_guard<std::mutex> lock(output_mutex_);
        std::cout << "=== CALL STACK ===" << std::endl;
        if (call_stack_.empty()) {
            std::cout << "Call stack is empty" << std::endl;
        }
        else {
            for (size_t i = 0; i < call_stack_.size(); ++i) {
                std::cout << "#" << i << " " << call_stack_[i] << std::endl;
            }
        }
    }
    static size_t stack_depth() {
        return call_stack_.size();
    }
    Tracer(const Tracer&) = delete;
    Tracer& operator=(const Tracer&) = delete;
    Tracer(Tracer&&) = default;
    Tracer& operator=(Tracer&&) = default;
};