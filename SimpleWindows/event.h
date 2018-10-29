#pragma once
#include "utility.h"

#include <functional>
#include <map>
#include <vector>

namespace sw
{
    template <typename... Args>
    class event
    {
    public:
        using token_type = std::size_t;
        using handler_type = std::function<void(Args...)>;

    private:
        token_type next_token;
        std::map<token_type, handler_type> handlers;

    public:
        event() = default;
        event(const event&) = delete;
        event(event&& e) : next_token(e.next_token), handlers(std::move(e.handlers)) {}
        event& operator=(const event&) = delete;
        event& operator=(event&& e)
        {
            next_token = e.next_token;
            handlers = std::move(e.handlers);
            return *this;
        }

        template <typename F>
        token_type add(F&& f)
        {
            token_type result = next_token++;
            handlers.emplace(result, std::forward<F>(f));
            return result;
        }
        template <typename T, typename C>
        token_type add(T (C::*f)(Args...), C* c)
        {
            return add([=](Args... args) -> T { return std::mem_fn(f)(c, std::forward<Args>(args)...); });
        }
        handler_type remove(token_type t)
        {
            handler_type result = std::move(handlers[t]);
            handlers.erase(t);
            return result;
        }

        void operator()(Args... args) const
        {
            for (auto& p : handlers)
            {
                p.second(std::forward<Args>(args)...);
            }
        }
    };
} // namespace sw

#define EVENT(name, ...)                                      \
private:                                                      \
    ::sw::event<__VA_ARGS__> name##_event;                    \
                                                              \
public:                                                       \
    template <typename F>                                     \
    typename ::sw::event<__VA_ARGS__>::token_type name(F&& h) \
    {                                                         \
        return name##_event.add(h);                           \
    }                                                         \
    template <typename T, typename C>                         \
    typename ::sw::event<__VA_ARGS__>::token_type name(       \
        T (C::*f)(__VA_ARGS__), C* c)                         \
    {                                                         \
        return name##_event.add(f, c);                        \
    }                                                         \
    typename ::sw::event<__VA_ARGS__>::handler_type name(     \
        typename ::sw::event<__VA_ARGS__>::token_type t)      \
    {                                                         \
        return name##_event.remove(t);                        \
    }

#define EVENT_SENDER_E(name, sender_type, args_type)                 \
    EVENT(name, sender_type, args_type)                              \
protected:                                                           \
    virtual void on_##name(sender_type sender, args_type args) const \
    {                                                                \
        name##_event(::std::forward<sender_type>(sender),            \
                     ::std::forward<args_type>(args));               \
    }
