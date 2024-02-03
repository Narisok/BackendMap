#pragma once

#include <ESP8266_NII_DRIVER.hpp>

struct MainController
{
    inline nii::Response *index()
    {
        auto response = new nii::HtmlResponse();

        response->setData(R"raw-data(
            <h1>Main page</h1>
            <div>
                <a href="/zones">Zones</a>
            </div>
        )raw-data");

        return response;
    }
};