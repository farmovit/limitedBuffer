#ifndef LIMITED_BUFFER_H
#define LIMITED_BUFFER_H

#include <map>
#include <set>

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
    void insert(const Key &key, T &&value) {
        ++mGlobalUsageCounter;
        auto helper = Helper(key, std::forward<T>(value), 0, mGlobalUsageCounter);
        if (removeHelper(key)) {
            insertHelper(std::move(helper));
            return;
        }
        if (static_cast<SizeType>(mElements.size()) == MaxSize) {
            removeUselessElement();
        }
        insertHelper(std::move(helper));
    }
    const T &value(const Key &key) {
        if (auto it = mElements.find(key);
                it != mElements.end()) {
            ++mGlobalUsageCounter;
            auto helper = Helper(key, it->second->value, it->second->usageCounter + 1, mGlobalUsageCounter);
            if (removeHelper(it)) {
                insertHelper(std::move(helper));
            }
        }
        return mElements.at(key)->value;
    }
    void clear() noexcept {
        mElements.clear();
        mHelpers.clear();
        mGlobalUsageCounter = 0;
    }
private:
    struct Helper
    {
        Helper(const Key &k, T &&val, SizeType usageCounter, SizeType lastCounter)
            : key(k),
              value(std::forward<T>(val)),
              usageCounter(usageCounter),
              lastUsageCounter(lastCounter) {}
        Helper(const Key &k, const T &val, SizeType usageCounter, SizeType lastCounter)
            : key(k),
              value(val),
              usageCounter(usageCounter),
              lastUsageCounter(lastCounter) {}
        bool operator<(const Helper &other) const {
            if (usageCounter == other.usageCounter) {
                return lastUsageCounter < other.lastUsageCounter;
            }
            return usageCounter < other.usageCounter;
        }
        Key key;
        T value;
        SizeType usageCounter;
        SizeType lastUsageCounter;
    };
private:
    std::set<Helper> mHelpers;
    std::map<Key, typename std::set<Helper>::iterator> mElements;
    SizeType mGlobalUsageCounter{0};
private:
    void insertHelper(Helper &&helper) noexcept {
        auto emplacedHelper = mHelpers.emplace(std::move(helper));
        if (emplacedHelper.second) {
            mElements.emplace(emplacedHelper.first->key, emplacedHelper.first);
        }
    }
    bool removeHelper(typename decltype(mElements)::iterator it) {
        mHelpers.erase(it->second);
        mElements.erase(it);
        return true;
    }
    bool removeHelper(const Key &key) {
        auto it = mElements.find(key);
        if (it == mElements.end()) {
            return false;
        }
        return removeHelper(it);
    }
    void removeUselessElement() {
        mElements.erase(mHelpers.begin()->key);
        mHelpers.erase(mHelpers.begin());
    }
};

} // namespace custom_containers

#endif // LIMITED_BUFFER_H
