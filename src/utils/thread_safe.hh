/*
 * shiro - High performance, high quality osu!Bancho C++ re-implementation
 * Copyright (C) 2018 Marc3842h, czapek
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SHIRO_THREAD_SAFE_HH
#define SHIRO_THREAD_SAFE_HH

#include <vector>
#include <mutex>

namespace shiro::utils::thread_safe {

    template <typename M = std::recursive_mutex>
    class simple_lock {
        M &m;

    public:
        simple_lock(M &m)
            : m(m) {
            m.lock();
        }

        simple_lock(const simple_lock &&other)
            : m(other.m) {
            m.try_lock();
        }

        simple_lock(const simple_lock &other)
            : m(other.m) {
            m.try_lock();
        }

        ~simple_lock() {
            m.unlock();
        }
    };

    template <typename T, typename M = std::recursive_mutex>
    class locked_iterable {
        T &v;
        simple_lock<M> l;

    public:
        locked_iterable(T &v, M &m)
            : l(m)
            , v(v) {
        }

        decltype(v.begin()) begin() {
            return v.begin();
        }

        decltype(v.end()) end() {
            return v.end();
        }

        decltype(v.cbegin()) cbegin() {
            return v.cbegin();
        }

        decltype(v.cend()) cend() {
            return v.cend();
        }
    };

    // clang-format off
    template <typename T, typename M = std::recursive_mutex>
    class locked_object {
        T v;
        M m;

    public:
        // Forward all args to the enclosed type
        template <typename... Args>
        locked_object(Args... args)
            : v(std::forward<Args>(args)...) {
        }

        locked_object(const locked_object &other) {
        }

        [[nodiscard]]
        std::pair<T &, simple_lock<M>> get() {
			auto p = std::make_pair(std::ref(v), simple_lock(m));

			return p;
            // return std::make_pair(std::ref(v), simple_lock(m));
        }

        // Be careful!
        T &non_locked_get() {
            return v;
        }

        locked_iterable<T> iterable() {
            return locked_iterable<T>(v, m);
        }
    };
    // clang-format on

    template <typename T>
    using locked_vector = locked_object<std::vector<T>>;
};

#endif //SHIRO_THREAD_SAFE_HH
