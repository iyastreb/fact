#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

namespace
{

/* Amount of bytes reserved for each entry*/
const int32_t RESERVE_ENTRY_BYTES = 10;
const char ENTRY_DELIMITER = '#';
const char ENTRY_KEY_EQUAL = '=';

typedef pair<string, string> entry;
typedef vector<entry> record;

/**
 * Validates key and values.
 * Only non-empty alpha-numerical string is allowed
 * @return true if entry is valid, false otherwise
 */
bool is_valid (const string & str)
{
    return (!str.empty() && find_if(str.begin(), str.end(),
            [=](char ch){ return !isalnum(ch);}) == str.end());
}

/**
 * Encoder for entry
 * @param entry Entry to encode
 * @param[out] out Output string buffer, used to reuse string buffer and
 *                 avoid creating many temporary strings
 * @throws runtime_error if entry is invalid
 */
void encode_entry (const entry & entry, string & out)
{
    if (!is_valid(entry.first)) {
        throw runtime_error("Entry key " + entry.first + " is malformed");
    }
    if (!is_valid(entry.second)) {
        throw runtime_error("Entry value " + entry.second + " is malformed");
    }
    out.clear();
    out += entry.first;
    out += ENTRY_KEY_EQUAL;
    out += entry.second;
}

/**
 * Encodes record into string
 * @param rec Record to encode
 * @return Encoded string for the record
 * @throws runtime_error if record is invalid
 */
string encode (const record & rec)
{
    string res;
    // Reserve buffer to expected size
    res.reserve(rec.size() * RESERVE_ENTRY_BYTES);
    int32_t index = 0;
    string buf;
    for (const entry & e : rec) {
        encode_entry(e, buf);
        res += buf;
        if (++index != rec.size()) {
            res += ENTRY_DELIMITER;
        }
    }
    return res;
}

/**
 * Decodes string to entry
 * @param str String to decode.
 *            Use pointer to avoid redundant copies
 * @param size Length of the str token
 * @return Decoded entry
 * @throws runtime_error if can't parse the string
 */
entry decode_entry (const char * str, int32_t size)
{
    // find position of the delimiter between key and value
    const char * pos = find(str, str + size, ENTRY_KEY_EQUAL);
    if (pos == str + size) {
        throw runtime_error("Entry is malformed: " + string(str, size));
    }
    // delimiter position
    int32_t index = pos - str;
    entry res = { string(str, index), string(pos + 1, size - index - 1) };

    // validate created entry
    if (!is_valid(res.first)) {
        throw runtime_error("Entry key " + res.first + " is malformed");
    }
    if (!is_valid(res.second)) {
        throw runtime_error("Entry value " + res.second + " is malformed");
    }
    return res;
}

/**
 * Decodes string to record
 * @param str String to decode.
 * @return Decoded record
 * @throws runtime_error if can't parse the string
 */
record decode (const string & str)
{
    record rec;
    if (!str.empty()) {
        const char * ptr = str.c_str();
        do {
            const char * begin = ptr;
            while (*ptr != ENTRY_DELIMITER && *ptr)
                ptr++;

            entry e = decode_entry(begin, ptr - begin);
            rec.push_back(move(e));
        } while (*ptr++);
    }
    return rec;
}

// Unit tests below

void test_is_valid ()
{
    assert(is_valid("string"));
    assert(!is_valid("hello="));
    assert(!is_valid("="));
    assert(!is_valid(""));
}

void test_should_reject_invalid_record ()
{
    record rec = {{"key=", "value"}};
    try {
        encode(rec);
        assert(false);
    } catch (const exception & ex) {
    }
}

void test_should_encode_valid_record ()
{
    record rec = {{"key1", "value1"}, {"key2", "value2"}};
    assert(encode(rec) == "key1=value1#key2=value2");
}

void test_should_encode_empty_record ()
{
    record rec = {};
    assert(encode(rec) == "");
}

void test_should_decode_empty_record ()
{
    record rec = decode("");
    assert(0 == rec.size());
}

void test_should_decode_encoded_record ()
{
    record rec = {{"key1", "value1"}, {"key2", "value2"}};
    record rec2 = decode(encode(rec));
    assert(rec.size() == rec2.size());
    for (int i = 0; i < rec.size(); ++i) {
        assert(rec[i].first == rec2[i].first);
        assert(rec[i].second == rec2[i].second);
    }
}

void test_should_reject_decode_malformed_string ()
{
    auto strings = { "key", "=", "#key", "key=value#", "===", "key=va_lue",
                     "key=value=key", "key==value", "k=v##v=k"};
    for (auto & it : strings) {
        try {
            decode(it);
            assert(false);
        } catch (const exception & ex) {
        }
    }
}

}

int main(int argc, char *argv[])
{
    test_is_valid();
    test_should_reject_invalid_record();
    test_should_encode_valid_record();
    test_should_encode_empty_record();
    test_should_decode_empty_record();
    test_should_decode_encoded_record();
    test_should_reject_decode_malformed_string();
    return EXIT_SUCCESS;
}
