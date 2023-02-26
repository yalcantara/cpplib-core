//
// Created by Yaison Alcantara 3 on 2/25/23.
//

#pragma once

#include <ylib/core/lang.h>
#include <ylib/test.h>
#include <ylib/utils/properties.h>

namespace ylib::utils {
    namespace fs = std::filesystem;
    using namespace ylib::test;
    using namespace ylib::utils;

    void testProperties() {

        TEST("test Properties", [] {
            // Given
            // ===============================================================
            PropertiesParser parser;
            fs::path path{"../test-resources/properties/sample.properties"};

            // When
            // ===============================================================
            Properties props = parser.load(path);
            string val1 = props.get("prop.key");
            string val2 = props.get("simple");
            string val3 = props.get("other");
            string val4 = props.get("key");
            string val5 = props.get("url");

            // Then
            // ===============================================================
            assertEquals(5, props.size());
            assertEquals("pop", val1);
            assertEquals("one", val2);
            assertEquals("234", val3);
            assertEquals("12312312312", val4);
            assertEquals("https://localhost:8080/test", val5);
        });

        TEST("test parseLine", [] {
            // Given
            PropertiesParser parser;
            string simple = "pop=push";
            string empty = "";
            string spaces = "    sample key   =   sample val  ";
            string comment = "   # some comment";
            string whitespaces = "         ";
            string tabs = "\t\t\t\t";
            string otherChars = "pop.push.peak = https://localhost:8080/test";

            // When
            auto ans1 = parser.parseLine(simple);
            auto ans2 = parser.parseLine(empty);
            auto ans3 = parser.parseLine(spaces);
            auto ans4 = parser.parseLine(comment);
            auto ans5 = parser.parseLine(whitespaces);
            auto ans6 = parser.parseLine(tabs);
            auto ans7 = parser.parseLine(otherChars);

            // Then
            assertTrue(ans1.has_value());
            auto [key1, val1] = ans1.value();
            assertEquals("pop", key1);
            assertEquals("push", val1);

            assertFalse(ans2.has_value());

            assertTrue(ans3.has_value());
            auto [key3, val3] = ans3.value();
            assertEquals("sample key", key3);
            assertEquals("sample val", val3);

            assertFalse(ans4.has_value());

            assertFalse(ans5.has_value());

            assertFalse(ans6.has_value());

            assertTrue(ans7.has_value());
            auto [key7, val7] = ans7.value();
            assertEquals("pop.push.peak", key7);
            assertEquals("https://localhost:8080/test", val7);
        });
    }
}