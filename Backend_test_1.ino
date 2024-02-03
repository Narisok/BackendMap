#include <ESP8266_NII_DRIVER.hpp>

#include "MainController.hpp"
#include "ZoneController.hpp"
#include "ApiZoneController.hpp"

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
    nii::Router::builder()->path("/api/zones/redirect")->call(&ApiZoneController::redirect);
    nii::Router::builder()->path("/api/zones")->call(&ApiZoneController::zones);

    nii::Router::builder()->path("/zones")->call(&ZoneController::list);
    nii::Router::builder()->path("/zones/store")->call(&ZoneController::store);
    nii::Router::builder()->path("/zones/create")->call(&ZoneController::create);
    nii::Router::builder()->path("/zones/edit/$")->call(&ZoneController::edit);


    nii::Router::builder()->path("/dir/list")->call( [] () -> nii::Response * {

      listDir("/");
 auto res = new nii::JsonResponse();
  res->json()["data"] = "list";
      return  res;
    });


  });

// TRACE("LIST FILE BEFORE");
// writeFile("/file.txt", "message");
// TRACE("LIST FILE AFTER");
// listDir("/");
}


void listDir(const char *dirname) {
  Serial.printf("Listing directory: %s\n", dirname);

  Dir root = LittleFS.openDir(dirname);

  while (root.next()) {
    File file = root.openFile("r");
    Serial.print("  FILE: ");
    Serial.print(root.fileName());
    Serial.print("  SIZE: ");
    Serial.print(file.size());
    time_t cr = file.getCreationTime();
    time_t lw = file.getLastWrite();
    file.close();
    struct tm *tmstruct = localtime(&cr);
    Serial.printf("    CREATION: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    tmstruct = localtime(&lw);
    Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
  }
}

void writeFile(const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = LittleFS.open(path, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  delay(2000);  // Make sure the CREATE and LASTWRITE times are different
  file.close();
}

void loop() {
  nii::backend::update();

  // put your main code here, to run repeatedly:

}
