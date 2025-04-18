#include "utils.h"

inline bool is_lower_case(unsigned char c) {
    return ((c) >= 'a' && (c) <= 'z');
}

inline bool is_upper_case(unsigned char c) {
    return ((c) >= 'A' && (c) <= 'Z');
}

inline bool is_alpha(unsigned char c) {
    return (is_lower_case(c) || is_upper_case(c));
}

inline bool is_numerical(unsigned char c) {
    return ((c) >= '0' && (c) <= '9');
}

inline unsigned char to_lower_case(unsigned char c) {
    return  (is_upper_case(c) ? (c) + ('a' - 'A') : (c));
}

inline unsigned char to_upper_case(unsigned char c) {
    return (is_lower_case(c) ? (c) - 'a' + 'A' : (c));
}