#include <Backend.hpp>

#pragma once



struct Zone
{
    std::string key;
    int mode;
    int from;
    int to;

    Zone()
        : key()
        , mode(0)
        , from(0)
        , to(0)
    {}

    Zone(const Zone &other)
        : key(other.key)
        , mode(other.mode)
        , from(other.from)
        , to(other.to)
    {}

    Zone( Zone &&other)
        : key(std::move(other.key))
        , mode(other.mode)
        , from(other.from)
        , to(other.to)
    {}

    Zone(
        std::string key,
        int mode,
        int from,
        int to
    )
        : key(key)
        , mode(mode)
        , from(from)
        , to(to)
    {

    }

    Zone &operator=(const Zone &other)
    {
        this->key = other.key;
        this->mode = other.mode;
        this->from = other.from;
        this->to = other.to;

        return *this;
    }


    bool db_same(const Zone &other)
    {
        return other.key == this->key;
    }

    // void db_sync()
    // {
    //     DB<Zone>::sync(*this);
    // }

    std::string db_write()
    {
        StaticJsonDocument<200> json;

        json["key"] = this->key;
        json["mode"] = this->mode;
        json["from"] = this->from;
        json["to"] = this->to;

        std::string stream;

        serializeJson(json, stream);

        return stream;
    }

    static Zone db_from(const std::string &stream)
    {
        StaticJsonDocument<200> json;
        deserializeJson(json, stream);

        Zone zone;

        zone.key = json["key"].as<std::string>();
        zone.mode = json["mode"].as<int>();
        zone.from = json["from"].as<int>();
        zone.to = json["to"].as<int>();

        return zone;
    }

    static std::string db_table()
    {
        return "/zones";
    }

};