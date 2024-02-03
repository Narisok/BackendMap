#include "Response.hpp"
#pragma once

#include <ESP8266_NII_DRIVER.hpp>

struct ApiZoneController
{

    inline nii::Response *set()
    {
        DynamicJsonDocument request(1024);
        deserializeJson(request, nii::server.arg("plain"));

        if (request["zones"].is<JsonArray>()) {
          for (auto elem : request["zones"].as<JsonArray>()) {
            std::string zoneKey = elem["key"].as<std::string>();
            int mode = elem["mode"].as<int>();

            // std::cout << "SET zone value:" << zoneKey << ":" << mode << std::endl;
            Zone zone;

            for (auto &item : DB<Zone>::get()) {
                if (item.key == zoneKey) {
                  zone = item;
                  zone.mode = mode;
                  DB<Zone>::sync(zone, false);
                  break;
                }
            }
          }
        }

        DB<Zone>::save();

        auto res = new nii::JsonResponse();

        res->json()["status"] = true;
        res->json()["payload"] = request;

        return res;
    }

    inline nii::Response *zones()
    {
        auto res = new nii::JsonResponse();

        auto items = DB<Zone>::get();

        res->json()["data"] = JsonArray();

        for (int i = 0; i < items.size(); i++) {
          res->json()["data"][i]["key"] = items[i].key;
          res->json()["data"][i]["mode"] = items[i].mode;
          res->json()["data"][i]["from"] = items[i].from;
          res->json()["data"][i]["to"] = items[i].to;
        }

        return res;

    }

    inline nii::Response *redirect()
    {
        // DynamicJsonDocument request(1024);
        // deserializeJson(request, nii::server.arg("plain"));


        // auto res = new nii::JsonResponse();

        // res->json()["status"] = true;
        // res->json()["other"] = request;

        return new nii::RedirectResponse("/zones");
    }



};