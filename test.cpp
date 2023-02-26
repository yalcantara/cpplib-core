//
// Created by Yaison Alcantara 3 on 2/20/23.
//

#include <ylib/core/TimeTest.h>
#include <ylib/core/LangTest.h>
#include <ylib/utils/PropertiesTest.h>

using namespace ylib::core;
using namespace ylib::utils;

int main(){
    testTime();
    testLang();
    testProperties();

    return EXIT_SUCCESS;
}
