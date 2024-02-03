#include <ESP8266_NII_DRIVER.hpp>

#include "MainController.hpp"
#include "ZoneController.hpp"
#include "ApiZoneController.hpp"
#include "ZoneMode.hpp"

#ifndef STASSID
#define STASSID "GigaProg"//"MikroTik-2GHz"
#define STAPSK "1q3e5t7u"//"wantGIGABIT"
#endif



const char *ssid = STASSID;
const char *passPhrase = STAPSK;

nii::Response *test()
{
  return new nii::Response();
}


nii::Response* foo()
{
    auto res = new nii::JsonResponse();

    res->json()["sensor"] = "gps";
    res->json()["time"]   = 1351824120;
    res->json()["data"][0] = 48.756080;
    res->json()["data"][1] = 2.302038;


    return res;
}


nii::Response* bar()
{
    auto res = new nii::HtmlResponse();

    res->setData(R"raw-data(
        <H1>Hello</H1>
    )raw-data");

    res->addData(R"raw-data(
        <div class="label label-sm label-primary">main</div>
    )raw-data");

    return res;
}


void setup() {
  // put your setup code here, to run once:
  nii::backend::setup(ssid, passPhrase, [] () {
    nii::Router::builder()->path("/")->call(&MainController::index);


    nii::Router::builder()->path("/api/zones/set")->call(&ApiZoneController::set);
    nii::Router::builder()->path("/api/zones/make")->call(&ApiZoneController::make);
    nii::Router::builder()->path("/api/zones/redirect")->call(&ApiZoneController::redirect);
    nii::Router::builder()->path("/api/zones")->call(&ApiZoneController::zones);

    nii::Router::builder()->path("/zones")->call(&ZoneController::list);
    nii::Router::builder()->path("/zones/store")->call(&ZoneController::store);
    nii::Router::builder()->path("/zones/create")->call(&ZoneController::create);
    nii::Router::builder()->path("/zones/edit/$")->call(&ZoneController::edit);

  });

  ZoneMode::begin();

}




void loop() {
  nii::backend::update();

  ZoneMode::update();

  // put your main code here, to run repeatedly:

}
