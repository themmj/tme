#ifndef _SIGCOUNTER_H
#define _SIGCOUNTER_H

#include <memory>
#include <unordered_map>
#include <signal.h>
#include "core/storage.hpp"

class SignalCounter {
    std::unordered_map<int, int> m_counters;

    SignalCounter() : m_counters() {}

    public:
    ~SignalCounter() {}

    static int assertionFailed;

    static tme::core::Handle<SignalCounter> instance() {
        static std::shared_ptr<SignalCounter> counter(new SignalCounter());
        return counter;
    }

    void listen(int sig) const {
        signal(sig, [](int signum) {
            SignalCounter::instance()->add(signum);
        });
    }

    void add(int sig) {
        auto res = m_counters.find(sig);
        int current = 0;
        if (res != m_counters.end()) {
            current = res->second;
        }
        m_counters.insert_or_assign(sig, ++current);
    }

    int get(int sig) const {
        auto res = m_counters.find(sig);
        if (res != m_counters.end()) {
            return res->second;
        }
        return 0;
    }
};

#endif

