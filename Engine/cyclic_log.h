#pragma once

namespace DXP
{

struct CyclicLog
{
    CyclicLog(size_t size) : buffer(size) { };

    const std::vector<char>& GetLogBufferRef();

protected:
    void Write(std::string_view log);

private:
    CyclicBuffer buffer;
};

template <typename Mutex>
struct CyclicLogSink : public spdlog::sinks::base_sink<Mutex>, public CyclicLog
{
    CyclicLogSink(size_t size) : CyclicLog(size) { };

protected:
    virtual void sink_it_(const spdlog::details::log_msg& msg) override {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

        CyclicLog::Write(fmt::to_string(formatted));
    };

    virtual void flush_() override { };
};

using CyclicLogSinkST = CyclicLogSink<std::mutex>;
using CyclicLogSinkMT = CyclicLogSink<spdlog::details::null_mutex>;

};
