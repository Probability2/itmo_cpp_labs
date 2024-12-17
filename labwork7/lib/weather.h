#pragma once

#include <iostream>
#include <fstream>
#include <conio.h>
#include "nlohmann/json.hpp"
#include <string>
#include "cpr/cpr.h"
#include <chrono>
#include <algorithm>
#include <map>
#include <ctime>

const int kEscapeCode = 0x1b;

const int kHoursPerDay = 24;

const int kDaysPerWeek = 7;

const int kTimeDay = 4;

const int kImageSize = 5;

const int kMaxForecastDays = 16;

const int kLineLength = 158;

const int kNoErrorCode = 200;

const int kShiftTime[kTimeDay] = {6, 12, 18, 23};

const std::vector<std::string> kStringParameters = {"temperature_2m", "relativehumidity_2m", "apparent_temperature",
                                "precipitation", "weathercode", "windspeed_10m", "winddirection_10m"};
                                
const char kShiftParameters = ',';

const int kTimeDaySize = 39;

const int kWeatherImageLen = 13;

struct City {
  City() = default;
  City(const std::string& name, const double latitude, const double longitude, const std::string& country);
  std::string name_;
  double latitude_;
  double longitude_;
  std::string country_;
};

const std::vector<std::string> kRain = {"    .--.    ",
                                        "   (    ).  ",
                                        "  (____(__) ",
                                        "\x1B[34m   , , , , ,\033[0m",
                                        "\x1B[34m  , , , , , \033[0m"};

const std::vector<std::string> kSnowFall =  {"    .--.    ",
                                             "   (    ).  ",
                                             "  (____(__) ",
                                             "   * * * * *",
                                             "  * * * * * "};

const std::vector<std::string> kOvercast =  {"            ",
                                             "    .--.    ",
                                             " .-(    ).  ",
                                             "(___.__)__).",
                                             "            ",};

const std::vector<std::string> kHaze     = {" _ - _ - _ -",
                                            "            ",
                                            "  _ - _ - _ ",
                                            "            ",
                                            " _ - _ - _ -",
                                            "            "};

const std::vector<std::string> kClearSky = {"\x1B[33m    \\ | /   \033[0m",
                                            "\x1B[33m    .--.    \033[0m",
                                            "\x1B[33m --(    )-- \033[0m",
                                            "\x1B[33m    .--.    \033[0m",
                                            "\x1B[33m   /    \\   \033[0m"};

const std::vector<std::string> kPartlyCloudly = {"   \x1B[33m\\  /     \033[0m",
                                                 "\x1B[33m _ /\"\"\033[0m.-.   ",
                                                 "\x1B[33m   \\_\033[0m(   ). ",
                                                 "\x1B[33m   /\033[0m(___(__)",
                                                 "            "};

const std::vector<std::string> kThunderStorm = {"    .--.    ",
                                                "   (    ).  ",
                                                "  (____(__) ",
                                                "\x1B[34m   /_/_/_/_ \033[0m",
                                                "\x1B[34m  / / / /_  \033[0m"};

const std::vector<std::string> kRainShowers =  {"    .--.    ",
                                                "   (    ).  ",
                                                "  (____(__) ",
                                                "\x1B[34m   | | | |  \033[0m",
                                                "\x1B[34m   | | | |  \033[0m"};

const std::vector<std::string> kSnowShowers =  {"    .--.    ",
                                                "   (    ).  ",
                                                "  (____(__) ",
                                                "  ********* ",
                                                "  ********* "};

const std::vector<std::string> kDrizzle     =  {"    .--.    ",
                                                "   (    ).  ",
                                                "  (____(__) ",
                                                "\x1B[34m  ,.,.,.,., \033[0m",
                                                "\x1B[34m ,.,.,.,.,.,\033[0m"};

const std::vector<std::string> kFreezingRain = {"    .--.    ",
                                                "   (    ).  ",
                                                "  (____(__) ",
                                                "\x1B[34m  /*/*/*/*/ \033[0m",
                                                "\x1B[34m /*/*/*/*/*/\033[0m"};

const std::vector<std::string> kSnowGrains   = {"    .--.    ",
                                                "   (    ).  ",
                                                "  (____(__) ",
                                                "  .*.*.*.*. ",
                                                " .*.*.*.*.*."};

const std::string kExtension = ".json";

void BuildCurrentForecast(const nlohmann::json& data);

std::string GetMonth(const int ind);

std::string BuildString(const std::vector<std::string>& vec);

std::string GetCalendarDate(const nlohmann::json& forecast, const std::size_t day);

std::string GetUnitsMeasure(const std::string& param);

void PrintLongLine();

int GetCurrTime(const nlohmann::json& data);

int GetValueWeekDay();

void FillSpace(const int length);

int GetDoubleLength(const std::string& str);

std::string GetParameter(const std::string& param);

void PrintBottomLine();

int ProccessParameter(const nlohmann::json& data, const std::string& str, const std::size_t day);

void PrintCarrySpace(int carry);

void PrintWeatherCode(const nlohmann::json& forecast, const std::size_t day);

void ViewParameters(const nlohmann::json& forecast, const std::size_t day);

std::string GetWMODescription(const uint8_t code);

std::vector<std::string> GetWMOImage(const uint8_t code);

std::string GetWeekDay(const int day);

static const std::string kMyApiAddress = "TvNcPARiXt4FeMhJ5MoMdQ==c0G6eZXobEq0X3NF";

bool BuildForecast(const nlohmann::json& data, const int days, const std::size_t city_index, std::map<std::string, City>& cities);

void MakeConsoleImage(const nlohmann::json& forecast, const City& city);
