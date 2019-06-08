#ifndef LIMITED_BUFFER_H
#define LIMITED_BUFFER_H

#include <map>
#include <algorithm>

namespace custom_containers {

using SizeType = std::size_t;

template<typename Key, typename T, SizeType MaxSize>
class LimitedBuffer
{
    static_assert((MaxSize != 0), "Container size must not be zero");
public:
    LimitedBuffer() = default;
    SizeType maxSize() const noexcept {
        return MaxSize;
    }
    SizeType size() const noexcept {
        return static_cast<SizeType>(mElements.size());
    }
    void insert(Key &&key, T &&value) {
        if (auto it = mElements.find(key);
                it != mElements.end()) {
            it->second.reset(std::forward<T>(value));
            return;
        }
        if (static_cast<SizeType>(mElements.size()) == MaxSize) {
            removeUselessElement();
        }
        mElements.emplace(std::forward<Key>(key), Helper(std::forward<T>(value)));
    }
    T &value(const Key &key) {
        if (auto it = mElements.find(key);
                it != mElements.end()) {
            ++(it->second.usageCounter);
            ++mGlobalUsageCounter;
            it->second.lastUsageCounter = mGlobalUsageCounter;
        }
        return mElements.at(key).value;
    }
    void clear() {
        mElements.clear();
        mGlobalUsageCounter = 0;
    }
private:
    void removeUselessElement() {
        const auto it = std::min_element(mElements.cbegin(), mElements.cend(),
                                   [](auto firstElem, auto secondElem) {
           return firstElem.second < secondElem.second;
        });
        mElements.erase(it);
    }
    struct Helper {
        explicit Helper(T &&val) : value(std::forward<T>(val)) {}
        T value;
        void reset(T &&val) {
            usageCounter = 0;
            lastUsageCounter = 0;
            value = std::forward<T>(val);
        }
        SizeType usageCounter{0};
        SizeType lastUsageCounter{0}; // or time
        bool operator<(const Helper &other) {
            if (usageCounter == other.usageCounter) {
                return lastUsageCounter < other.lastUsageCounter;
            }
            return usageCounter < other.usageCounter;
        }
    };
    std::map<Key, Helper> mElements;
    SizeType mGlobalUsageCounter{0};
};

} // namespace custom_containers

#endif // LIMITED_BUFFER_H
