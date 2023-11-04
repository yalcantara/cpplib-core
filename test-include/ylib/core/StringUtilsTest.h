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
    
    TEST("test ToStringBuilder with optional<>", [] {
        // Given
        ToStringBuilder sb1;
        ToStringBuilder sb2;
        
        // When
        sb1.appendOpt("pop", std::make_optional(string{"push"}));
        sb1.append("push", "peak");
        sb1.appendOpt("gift", std::make_optional((Int64)3334));
        sb1.appendOpt("save", std::make_optional((double)3.14));
        
        optional<string> emptyOpt = std::nullopt;
        sb2.appendOpt("void", emptyOpt);
        auto someOpt = std::make_optional(string{"abc"});
        sb2.appendOpt("math", someOpt, [](auto a){return a + "efg";});
        
        // Then
        assertEquals("{pop: \"push\", push: \"peak\", gift: 3334, save: 3.14}", sb1.str());
        assertEquals("{void: (unassigned), math: \"abcefg\"}", sb2.str());
    });
}
}
