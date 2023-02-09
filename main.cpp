#include <cstdio>
#include <array>
#include <cstring>
#include <iostream>
#include <vector>

#include <optional>

#include <ylib/db/dpiw.h>
#include <ylib/logging/Logger.h>
#include <ylib/web/rest.h>

using namespace ylib::core;
using namespace ylib::db::dpiw;
using namespace ylib::logging;
using namespace ylib::web;
using namespace web;

#define var auto

int main(){

    

    const char* base = "https://openexchangerates.org/";
    RestClient client{base};

    Query q;
    q.put("app_id", "4a231b45057a4dd99df148d5abca58c8");
    q.put("base", "USD");
    //JPY - Japanese Yen
    //HKD - Hong Kong Dollar
    //SEK - Swedish krona
    //SGD - Singapore Dollar
    //NOK - Norwegian Krone
    //CNY - Chinese Yuan (Renminbi)
    //CLP - Chilean Peso
    q.put("symbols", "USD,EUR,JPY,GBP,AUD,CAD,CHF,CNY,HKD,SEK,SGD,KRW,NOK,MXN,INR,RUB,CLP,DOP");

    string txt = client.wget("/api/historical/2020-06-05.json", q);

    
    println(txt);
    json::value ans = json::value::parse(txt);

    json::value rates = ans.at("rates");

    double usd = rates.at("USD").as_double();
    double eur = rates.at("EUR").as_double();
    double jpy = rates.at("JPY").as_double();
    double gbp = rates.at("GBP").as_double();
    double aud = rates.at("AUD").as_double();
    double cad = rates.at("CAD").as_double();
    double chf = rates.at("CHF").as_double();
    double cny = rates.at("CNY").as_double();
    double hkd = rates.at("HKD").as_double();
    double sek = rates.at("SEK").as_double();
    double sgd = rates.at("SGD").as_double();
    double krw = rates.at("KRW").as_double();
    double nok = rates.at("NOK").as_double();
    double mxn = rates.at("MXN").as_double();
    double inr = rates.at("INR").as_double();
    double rub = rates.at("RUB").as_double();
    double clp = rates.at("CLP").as_double();
    double dop = rates.at("DOP").as_double();

    println(usd);
    return EXIT_SUCCESS;
}
