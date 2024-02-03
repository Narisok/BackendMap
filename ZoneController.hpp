#include <string>
#pragma once

#include <ESP8266_NII_DRIVER.hpp>

#include "Zone.hpp"

struct ZoneController {
  inline nii::Response *list() {
    auto res = new nii::HtmlResponse();

    res->setData(R"raw-data(
            <H1>Zones</H1>
            <div class="list-group">
        )raw-data");

    for (auto &zone : DB<Zone>::get()) {

      res->addData(
        std::string("<a href=\"/zones/edit/")
        + zone.key
        + "\" class=\"list-group-item list-group-item-action\">Zone "
        + zone.key
        + "</a>"
        );
    }

    res->addData(R"raw-data(
            </div>
            <div>
                <a href="/zones/create">Add new zone</a>
            </div>
            <div>
                <a href="/">Go to main page</a>
            </div>
        )raw-data");

    return res;
  }

  inline nii::Response *create() {
    auto res = new nii::HtmlResponse();

    res->setData(R"raw-data(
            <H1>Create new zone</H1>
            <div class="list-group">
        )raw-data");

    res->addData(R"raw-data(
               <form action="/zones/store" method="POST">
                  <div class="form-group row ">
                      <span class="form-label col">Key</span>
                      <input type="text" name="key" class="form-control col">
                  </div>
                  <div class="form-group row ">
                      <span class="form-label col">From led</span>
                      <input type="number" name="from" class="form-control col">
                  </div>
                  <div class="form-group row ">
                      <span class="form-label col">To led</span>
                      <input type="number" name="to" class="form-control col">
                  </div>
                  <div class="form-group row ">
                      <span class="form-label col">Mode</span>
                      <select name="mode"  class="form-control form-select col" id="">
                          <option value="0">Default</option>
                          <option value="1">Color</option>
                          <option value="2">Rainbow</option>
                      </select>
                  </div>
                  <div class="form-group">
                      <button type="submit" class="btn btn-sm btn-primary">Send</button>
                  </div>
              </form>
            )raw-data");

    res->addData(R"raw-data(
            </div>
            <div>
                <a href="/zones"> <- Back</a>
            </div>
            <div>
                <a href="/">Go to main page</a>
            </div>
        )raw-data");

    return res;
  }

  inline nii::Response *store() {
    std::string key = nii::server.arg("key").c_str();
    int mode = nii::server.arg("mode").toInt();
    int from = nii::server.arg("from").toInt();
    int to = nii::server.arg("to").toInt();

    Zone zone(key, mode, from, to);

    DB<Zone>::sync(zone);


    return new nii::RedirectResponse("/zones");
  }

  inline nii::Response *edit(std::string key) {
    auto res = new nii::HtmlResponse();

    Zone zone;
     for (auto &item : DB<Zone>::get()) {
        if (item.key == key) {
          zone = item;
          break;
        }
     }

    res->setData(R"raw-data(
            <H1>Edit zone "
        )raw-data");
    res->addData(zone.key);
    res->addData(R"raw-data(
            "</H1>
        <div class="list-group">
        )raw-data");

    res->addData(R"raw-data(<form action="/zones/store" method="POST">)raw-data");

    res->addData(R"raw-data(<div class="form-group row ">
            <span class="form-label col">Key</span>
            <input type="text" name="key" value=")raw-data");
    res->addData((zone.key));
    res->addData(R"raw-data(" class="form-control col"></div>)raw-data");

    res->addData(R"raw-data(<div class="form-group row ">
            <span class="form-label col">From led</span>
            <input type="number" name="from" value=")raw-data");
    res->addData(std::to_string(zone.from));
    res->addData(R"raw-data(" class="form-control col"></div>)raw-data");



    res->addData(R"raw-data(<div class="form-group row ">
            <span class="form-label col">To led</span>
            <input type="number" name="to" value=")raw-data");
    res->addData(std::to_string(zone.to));
    res->addData(R"raw-data(" class="form-control col"></div>)raw-data");


    res->addData(R"raw-data(<div class="form-group row ">
                    <span class="form-label col">Mode</span>
                    <select name="mode"  class="form-control form-select col">
          )raw-data");

    for (int i = 0; i < 3; i++) {

      res->addData(std::string("<option value=\">") + std::to_string(i) + "\"" + (i == zone.mode ? " selected>" : ">"));

      switch (i) {
        case 0:
          res->addData("Default");
          break;
        case 1:
          res->addData("Color");
          break;
        case 2:
          res->addData("Rainbow");
          break;
      }

      res->addData("</option>");
    }


    res->addData(R"raw-data(
                    </select>
                </div>
                <div class="form-group">
                    <button type="submit" class="btn btn-sm btn-primary">Send</button>
                </div>
            </form>
        )raw-data");


    res->addData(R"raw-data(
            </div>
            <div>
                <a href="/zones"> <- Back</a>
            </div>
            <div>
                <a href="/">Go to main page</a>
            </div>
        )raw-data");

    return res;
  }
};