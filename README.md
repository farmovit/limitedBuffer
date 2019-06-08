### Description
This is a very simple limited buffer implementation that allows you to store elements as key-value pairs. This buffer is watching for frequency and last time of elements usage. When buffer's size reaches a given limit, a useless element will be deleted. It is considered that useless element is element, which has the less frequent usage or, in case of having multiple that elements, element, which has not been used the longest time.

### Requirements
- cmake > 3.8
- c++17 supported compiler
- gtest

### TODO
- Extend interface
- Think about a counter's overflow. Probably, whe should reinitialize buffer when one of them reaches size limit
