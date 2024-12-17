#include "lib/weather.h"
#include "parser/ArgParser.h"

int main(int argc, char** argv) {
	ArgumentParser::ArgParser parser("Forecast");
	parser.AddHelp('h', "help", "The program outputs a forecast of selected cities using dll");
	parser.AddStringArgument('f', "file", "Contains the name of json file");
	if (!parser.Parse(argc, argv)) {
		std::cerr << "ERROR\nIncorrect arguments\n";
		std::cout << parser.HelpDescription() << '\n';
		return EXIT_FAILURE;
	}
	if (parser.Help()) {
		std::cout << parser.HelpDescription() << '\n';
		return EXIT_SUCCESS;
	}
	std::ifstream my_file(parser.GetStringValue("file"), std::ios::in);
	if (!my_file) {
		std::cerr << "ERROR:\nUnable to open the file\n";
		return EXIT_FAILURE;
	}
	std::size_t city_index = 0;
	nlohmann::json data = nlohmann::json::parse(my_file, nullptr, false);
	if (data.is_discarded()) {
		std::cerr << "ERROR:\nThe json file is damaged\n";
		return EXIT_FAILURE;
	}
	int days = data["forecast_days"].get<int>();
	int frequency = data["update_frequency"].get<int>();
	std::size_t json_length = data["cities"].size();
	time_t start, end;
	time(&start);
	std::map<std::string, City> cities;
	std::cout << '\n';
	if (!BuildForecast(data, days, city_index, cities)) {
		return EXIT_FAILURE;
	}
	for (;;) {
		if (!_kbhit()) {
			time(&end);
			if (difftime(end, start) >= frequency) {
				start = end;
				system("cls");
				if (!BuildForecast(data, days, city_index, cities)) {
					return EXIT_FAILURE;
				}
			}
			continue;
		}
		switch (_getch()) {
			case 'n':
				city_index = (city_index + 1) % json_length;
				break;
			case 'p':
				city_index = (city_index == 0) ? (json_length - 1) : (city_index - 1) % json_length;
				break;
			case '+':
				days = (days == kMaxForecastDays - 1) ? (days) : (days + 1);
				break;
			case '-':
				days = (days == 1) ? 1 : days - 1;
				break;
			case kEscapeCode:
				my_file.close();
				return EXIT_SUCCESS;
			default:
				continue;
		}
		system("cls");
		if (!BuildForecast(data, days, city_index, cities)) {
			return EXIT_FAILURE;
		}
	}
	my_file.close();

	return EXIT_SUCCESS;
}
