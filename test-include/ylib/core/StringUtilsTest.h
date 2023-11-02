#pragma once

#include <ylib/core/sutils.h>
#include <ylib/test.h>

namespace ylib::core {
using namespace ylib::test;

void testSUtils() {
    TEST("test ToStringBuilder", [] {
        // Given
        ToStringBuilder sb1;
        string k1 = "pop";
        string v1 = "push";
        
        string k2 = "peak";
        Int32 v2 = 34;
        
        ToStringBuilder sb2; //empty
        
        // When
        sb1.append(k1, v1);
        sb1.append(k2, v2);
        string ans1 = sb1.str();
        
        string ans2 = sb2.str();
        
        // Then
        assertEquals("{pop: \"push\", peak: 34}", ans1);
        assertEquals("{}", ans2);
    });
}
} //ylib::core
