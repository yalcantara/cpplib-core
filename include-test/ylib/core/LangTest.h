//
// Created by Yaison Alcantara 3 on 2/23/23.
//

#pragma once

#include <ylib/core/lang.h>
#include <ylib/test.h>

using namespace ylib::test;

void testLang() {

    TEST("test sfput", [] {
        // Given
        string single = "{}";
        string repeated = "{}{}{}{}";
        string mixed = "Pop {}, push: '{}', peak: \"{}\"";
        string largeNumbers = "{} + {} = {}";
        string floats = "{} / {} = {}";
        string booleans = "true: {}; false: {}; True: {}; False: {}";

        // When
        string ans1 = sfput(single, "pop");
        string ans2 = sfput(repeated, "a", "b", "c", "d");
        string ans3 = sfput(mixed, "123", "abc", "efg456");
        string ans4 = sfput(largeNumbers, 123456789, 987654321, 1111111110);
        string ans5 = sfput(floats, 3.14, 2.0, 1.57);
        string ans6 = sfput(booleans, true, false, True, False);

        // Then
        assertEquals("pop", ans1);
        assertEquals("abcd", ans2);
        assertEquals("Pop 123, push: 'abc', peak: \"efg456\"", ans3);
        assertEquals("123456789 + 987654321 = 1111111110", ans4);

        // sfput uses std::to_string for double values.
        // to_string(3.14) returns "3.140000"
        assertEquals("3.140000 / 2.000000 = 1.570000", ans5);
        assertEquals("true: true; false: false; True: True; False: False", ans6);
    });
}
