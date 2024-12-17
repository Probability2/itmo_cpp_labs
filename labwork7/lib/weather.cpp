#include "weather.h"

City::City(const std::string& name, const double latitude, const double longitude, const std::string& country)
: name_(name)
, latitude_(latitude)
, longitude_(longitude)
, country_(country) {
}

void PrintLongLine() {
  for (std::size_t i = 0; i < kLineLength; ++i) {
    std::cout << '-';
  }
  std::cout << '\n';
}

void PrintBottomLine() {
  for (std::size_t i = 0; i < kLineLength; ++i) {
    std::cout << '_';
  }
  std::cout << '\n';
}

std::string GetMonth(const int ind) {
  switch(ind) {
    case 1:
      return "January";
    case 2:
      return "February";
    case 3:
      return "March";
    case 4:
      return "April";
    case 5:
      return "May";
    case 6:
      return "June";
    case 7:
      return "July";
    case 8:
      return "August";
    case 9:
      return "September";
    case 10:
      return "October";
    case 11:
      return "November";
    case 12:
      return "December";
    default:
      std::cerr << "ERROR:\nIncorrect month\n";
      std::exit(EXIT_FAILURE);
  }
}

inline void AddTwoSymbols(std::string& str1, const std::string& str2, const std::size_t ind) {
  str1 += str2[ind];
  str1 += str2[ind + 1];
}

std::string GetCalendarDate(const nlohmann::json& forecast, const std::size_t day) {
  std::string ans;
  std::string date = forecast["hourly"]["time"][kHoursPerDay * day];
  AddTwoSymbols(ans, date, 8);
  ans += ' ';
  std::string month_ind;
  AddTwoSymbols(month_ind, date, 5);
  ans += GetMonth(std::stoi(month_ind));
  ans += ", ";
  for (std::size_t i = 0; i < 4; ++i) {
    ans += date[i];
  }

  return ans;
}

std::string GetWeekDay(const int day) {
  switch (day) {
    case 0:
      return "Sunday";
    case 1:
      return "Monday";
    case 2:
      return "Tuesday";
    case 3:
      return "Wednesday";
    case 4:
      return "Thursday";
    case 5:
      return "Friday";
    case 6:
      return "Saturday";
    default:
      std::cerr << "ERROR:\nIncorrect week day\n";
      std::exit(EXIT_FAILURE);
  }
}

std::string GetWMODescription(const uint8_t code) {
  if (code == 0) {
    return "Clear Sky";
  } else if (code == 2) {
    return "Partly Cloudly";
  } else if (code <= 3) {
    return "Overcast";
  } else if (code == 45 || code == 48) {
    return "Fog";
  } else if (code == 51 || code == 53 || code == 55) {
    return "Drizzle";
  } else if (code == 56 || code == 57) {
    return "Freezing Drizzle";
  } else if (code == 61 || code == 63 || code == 65) {
    return "Rain";
  } else if (code == 66 || code == 67) {
    return "Freezing Rain";
  } else if (code == 71 || code == 73 || code == 75) {
    return "Snow fall";
  } else if (code == 77) {
    return "Snow Grains";
  } else if (code >= 80 && code <= 82) {
    return "Rain showers";
  } else if (code == 85 || code == 86) {
    return "Snow showers";
  } else if (code == 95 || code == 96 || code == 99) {
    return "ThunderStorm";
  }
  std::cerr << "ERROR:\nIncorrect WMO (weather interpretation code)\n";
  std::exit(EXIT_FAILURE);
}

std::vector<std::string> GetWMOImage(const uint8_t code) {
  if (code == 0) {
    return kClearSky;
  } else if (code == 2) {
    return kPartlyCloudly;
  } else if (code <= 3) {
    return kOvercast;
  } else if (code == 45 || code == 48) {
    return kHaze;
  } else if (code == 51 || code == 53 || code == 55) {
    return kDrizzle;
  } else if (code == 56 || code == 57) {
    return kDrizzle;
  } else if (code == 61 || code == 63 || code == 65) {
    return kRain;
  } else if (code == 66 || code == 67) {
    return kFreezingRain;
  } else if (code == 71 || code == 73 || code == 75) {
    return kSnowFall;
  } else if (code == 77) {
    return kSnowGrains;
  } else if (code >= 80 && code <= 82) {
    return kRainShowers;
  } else if (code == 85 || code == 86) {
    return kSnowShowers;
  } else if (code == 95 || code == 96 || code == 99) {
    return kThunderStorm;
  }
  std::cerr << "ERROR:\nIncorrect WMO (weather interpretation code)\n";
  std::exit(EXIT_FAILURE);
}

void PrintCarrySpace(int carry) {
  while (carry--) {
    std::cout << ' ';
  }
}

int GetDoubleLength(const std::string& str) {
  if (str == "temperature_2m" || str == "apparent_temperature" || str == "precipitation" || str == "windspeed_10m") {
    return 2;
  }

  return 0;
}

std::string GetParameter(const std::string& param) {
  if (param == "temperature_2m") {
    return "temp_2m";
  } else if (param == "apparent_temperature") {
    return "app_temp";
  } else if (param == "precipitation") {
    return "precip";
  } else if (param == "windspeed_10m") {
    return "wspeed_10m";
  } else if (param == "relativehumidity_2m") {
    return "RH";
  } else if (param == "winddirection_10m") {
    return "wdirection_10m";
  }
  std::cerr << "ERROR:\nIncorrect parameter\n";
  std::exit(EXIT_FAILURE);
}

std::string GetUnitsMeasure(const std::string& param) {
  if (param == "temperature_2m" || param == "apparent_temperature") {
    std::string ans;
    ans += " \xF8";
    ans += 'C';
    return ans;
  } else if (param == "precipitation") {
    return " mm";
  } else if (param == "windspeed_10m") {
    return " km/h";
  } else if (param == "relativehumidity_2m") {
    return "%";
  } else if (param == "winddirection_10m") {
    return "\xF8";
  }
  std::cerr << "ERROR:\nIncorrect parameter\n";
  std::exit(EXIT_FAILURE);
}

std::string BuildString(const std::vector<std::string>& vec) {
  std::string ans;
  for (int i = 0; i < vec.size(); ++i) {
    for (int j = 0; j < vec[i].length(); ++j) {
      ans += vec[i][j];
    }
    if (i != vec.size() - 1) {
      ans += ',';
    }
  }

  return ans;
}

int ProccessParameter(const nlohmann::json& forecast, const std::string& str, const std::size_t ind) {
  std::string short_name = GetParameter(str);
  auto parameter = forecast["hourly"][str][ind];
  std::string str1 = std::to_string(static_cast<int>(parameter));
  std::string measure = GetUnitsMeasure(str);
  int double_length = GetDoubleLength(str);
  int length_parameter = 2 + short_name.length() + str1.length() + double_length + measure.length();
  if (parameter < 0 && parameter > -1 && double_length == 2) {
    ++length_parameter;
  }
  std::cout << short_name << ": " << forecast["hourly"][str][ind] << measure;

  return length_parameter;
}

void ViewParameters(const nlohmann::json& forecast, const std::size_t day) {
  std::string str;
  int index_param = 0;
  for (std::size_t ind = 0; ind < kImageSize; ++ind && ++index_param) {
    if (kStringParameters[index_param] == "weathercode") {
      ++index_param;
    }
    str = kStringParameters[index_param];
    for (std::size_t i = 1; i <= kTimeDay; ++i) {
      std::cout << GetWMOImage(forecast["hourly"]["weathercode"]
                                      [day * kHoursPerDay + kShiftTime[i - 1]])[ind] << " ";
      int length_param = (str != "weathercode") ? 
                  (ProccessParameter(forecast, str, day * kHoursPerDay + kShiftTime[i - 1])) : (0);
      if (i != kTimeDay) {
        PrintCarrySpace(kTimeDaySize - kWeatherImageLen - length_param);
        std::cout << '|';
      }
    }
    std::cout << '\n';
  }
  while (index_param++ < kStringParameters.size()) {
    str = kStringParameters[index_param - 1];
    for (int i = 1; i <= kTimeDay; ++i) {
      FillSpace(13);
      int length_param = 0;
      if (str != "weathercode") {
        length_param = ProccessParameter(forecast, str, day * kHoursPerDay + kShiftTime[i - 1]);
      }
      if (i != kTimeDay) {
        PrintCarrySpace(kTimeDaySize - kWeatherImageLen - length_param);
        std::cout << '|';
      }
    }
  }
}

void FillSpace(const int length) {
  for (std::size_t i = 0; i < length; ++i) {
    std::cout << ' ';
  }
}

void PrintWeatherCode(const nlohmann::json& forecast, const std::size_t day) {
  for (std::size_t i = 1; i <= kTimeDay; ++i) {
    std::string weather_description = 
            GetWMODescription(forecast["hourly"]["weathercode"][day * kHoursPerDay + kShiftTime[i - 1]]);
    std::cout << weather_description;
    int line_len = (i == 1) ? kTimeDaySize : (kTimeDaySize - 1);
    if (i != kTimeDay) {
      FillSpace(line_len - static_cast<int>(weather_description.length()));
      std::cout << "| ";
    }
  }
}

bool BuildForecast(const nlohmann::json& data, const int days, const std::size_t city_index,
                                                               std::map<std::string, City>& cities) {
  if (!cities.contains(data["cities"][city_index]["name"])) {
    cpr::Response request = cpr::Get(cpr::Url{"https://api.api-ninjas.com/v1/city"},
                            cpr::Header{{"X-Api-Key", kMyApiAddress}},
                            cpr::Parameters{{"name", data["cities"][city_index]["name"]}});
    if (request.status_code != kNoErrorCode) {
      std::cerr << request.error.message << '\n';
      return false;
    }
    nlohmann::json p = nlohmann::json::parse(request.text);
    if (!p.size()) {
      std::cerr << "ERROR:\nThere is no such a city(" << data["cities"][city_index]["name"] << ")\n";
      return false;
    }
    cities.insert(std::make_pair(data["cities"][city_index]["name"], 
        City(data["cities"][city_index]["name"], p[0].at("latitude"), p[0].at("longitude"), p[0].at("country"))));
  }
  cpr::Response r = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                              cpr::Parameters{{"latitude",        std::to_string(cities[data["cities"][city_index]["name"]].latitude_)},
                                              {"longitude",       std::to_string(cities[data["cities"][city_index]["name"]].longitude_)},
                                              {"current_weather", "true"},
                                              {"timezone", "auto"},
                                              {"forecast_days",   std::to_string(days)},
                                              {"hourly",          BuildString(kStringParameters)}});
  if (r.status_code != kNoErrorCode) {
    std::cerr << r.error.message << '\n';
    return false;
  }
  MakeConsoleImage(nlohmann::json::parse(r.text), cities[data["cities"][city_index]["name"]]);

  return true;
}

int GetCurrTime(const nlohmann::json& data) {
  std::string curr_time = data["current_weather"]["time"];
  std::tm tm = {};
  std::istringstream ss(curr_time);
  ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M");

  return tm.tm_hour;
}

int GetValueWeekDay() {
  struct tm local_time;
  time_t now = time(0);
  localtime_s(&local_time, &now);

  return local_time.tm_wday;
}

void BuildCurrentForecast(const nlohmann::json& data) {
  int current_time = GetCurrTime(data);
  std::string weather_description = GetWMODescription(data["hourly"]["weathercode"][current_time]);
  std::cout << weather_description << '\n';
  std::size_t index_param = 0;
  for (std::size_t ind = 0; ind < kImageSize; ++ind && ++index_param) {
    if (kStringParameters[index_param] == "weathercode") {
      ++index_param;
    }
    std::cout << GetWMOImage(data["hourly"]["weathercode"]
                                    [current_time])[ind] << " ";
    int length_param = (kStringParameters[index_param] != "weathercode") ?
                       (ProccessParameter(data, kStringParameters[index_param], current_time)) : (0);
    std::cout << '\n';
  }
  std::size_t ind = index_param;
  while (ind++ < kStringParameters.size()) {
    FillSpace(kWeatherImageLen);
    int length_param = (kStringParameters[index_param] != "weathercode") ?
                       (ProccessParameter(data, kStringParameters[index_param], current_time)) : (0);
    std::cout << '\n';
  }
}

void MakeConsoleImage(const nlohmann::json& forecast, const City& city) {
  std::cout << "Weather forecast, " << city.name_ << ", " << city.country_ << "\n\n";
  BuildCurrentForecast(forecast);
  std::cout << '\n';
  for (std::size_t i = 0; i < forecast["hourly"]["weathercode"].size() / kHoursPerDay; ++i) {
    PrintBottomLine();
    std::cout << "\t\t\t\t\t\t\t Day " << i + 1 << ", " << GetWeekDay((GetValueWeekDay() + i) % kDaysPerWeek)
                                                        << ", " << GetCalendarDate(forecast, i) << "\t\t\n";
    PrintLongLine();
    std::cout << "\t\tMorning\t\t\t   |\t\tDay\t\t\t   |\t\tEvening\t\t\t   |\t\tNight\n";
    PrintLongLine();
    PrintWeatherCode(forecast, i);
    std::cout << '\n';
    ViewParameters(forecast, i);
    std::cout << '\n';
  }
}
