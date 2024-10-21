#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

int randomx(int max_value){
	std::srand(std::time(nullptr)); // use current time as seed for random generator
	int value = std::rand() % max_value;
	return value;
}


int score(std::string user, int attemps){
	int result = 0;

	std::ifstream file_r;
	file_r.open("result.txt", std::ifstream::in);
	std::string line;

	std::string temp_file;
	
	while (std::getline(file_r, line))
	{
		if (line.find(user) != std::string::npos)
		{
			
			result = std::stoi(line.substr(line.find(":") + 1, line.find_first_of(";")-line.find(":")));
			if (result > attemps)
			{
				std::string new_line = line.replace(line.find(":") + 1, line.find_first_of(";")-line.find(":") - 1, std::to_string(attemps));
				temp_file.append(new_line).append("\n");
			}
			else
			{
				temp_file.append(line).append("\n");
			}

		}
		if (line.find(user) == std::string::npos)
		{
			temp_file.append(line).append("\n");
		}
	}

	file_r.close();

	std::ofstream file;
	file.open("result.txt", std::ofstream::out);
	file << temp_file;
	file.close();

	if (result == 0)
	{
		std::ofstream file;
		file.open("result.txt", std::ofstream::app);
		file << user << ":" << attemps << ";\n";
		file.close();
	}
	
	return result;
}

int main(int argc, char** argv) {

	int randnumber;
	if (argc == 1)
	{
		randnumber = 100;
	}

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{

			std::string arg_value{ argv[i] };
			if (arg_value == "-table")
			{
				std::ifstream file;
				file.open("result.txt");
				if (!file.is_open()) 
				{	
					std::cout << "Ошибка открытия файла!" << std::endl;
					return -1;
				}

				std::string line;
				while (std::getline(file, line))
				{
					std::string user = line.substr(0, line.find(":"));
					std::string value = line.substr(line.find(":") + 1, line.find_first_of(";")-line.find(":") - 1);
					std::cout << "Игрок: " << user << " Результат: "<< value <<std::endl;

				}
				file.close();
				return 0;
			}
		}

		for (int i = 1; i < argc; i++)
		{
			std::string arg_value{ argv[i] };
			if (arg_value == "-level")
			{
				//check level + max sometime error
				for (int j = 1; j < argc; j++)
				{
					std::string argj_value{ argv[j] };
					if (argj_value == "-max"){
						std::cout << "Ошибка: одновременно использование level и max параметров!" << std::endl;
						return -1;
					}
				}
				if (!argv[i + 1])
				{
					std::cout << "Ошибка: значение level не задано" << std::endl;
					return -1;
				}
				int level_value = std::stoi(argv[i + 1]);

				if (level_value == 1) randnumber = 10;
				if (level_value == 2) randnumber = 50;
				if (level_value == 3) randnumber = 100;
				if ((level_value != 1)&&(level_value != 2)&&(level_value != 3))
				{
					std::cout << "Ошибка: значение level неверно (1,2,3)" << std::endl;
					return -1;
				}
			}
		}

		for (int i = 1; i < argc; i++)
		{
			std::string arg_value{ argv[i] };
			if (arg_value == "-max"){
				if (!argv[i + 1])
				{
					std::cout << "Ошибка: значение max не задано" << std::endl;
					return -1;
				}
				int max_value = std::stoi(argv[i + 1]);
				if ((max_value < 0)||(max_value > 100))
				{
					std::cout << "Ошибка: значение max неверно (1-100)" << std::endl;
					return -1;
				}
				else 
				{
					randnumber = max_value;
				}
			}
		}

	}

	std::cout << "Введите свое имя" << std::endl;
	std::string username;
	std::cin >> username;

	if ((username.find(";") != std::string::npos)||(username.find(":") != std::string::npos))
	{
		std::cout << "Ошибка: запрещенный символ в имени (:,;)" << std::endl;
		return -1;
	}

	int attemps = 0;
	int randvalue = randomx(randnumber);
	int uservalue;

	while (1)
	{
        std::cout << "Введите число от 0 до " << randnumber << std::endl;
        std::cin >> uservalue;

		if ((uservalue >= 0)&&(uservalue <= randnumber))
		{
	    	if (uservalue < randvalue) 
	    	{
		    	std::cout << "Значение меньше загаданного"<< std::endl;
				attemps++;
	    	}
	    	if (uservalue > randvalue)
			{
				std::cout << "Значение больше загаданного"<< std::endl;
				attemps++;
			}
			if (uservalue == randvalue)
			{
				attemps++;
				std::cout << "Ура! Вы угадали! Количество попыток "<< attemps << std::endl;
				int result;
				result = score(username, attemps);
				if (result > 0) 
				{
					std::cout << "Предыдущий результат: "<< result << std::endl;
					break;
				}
				else
				{
					std::cout << "Предыдущий результат не найден" << std::endl;
					break;
				}
				
			}
		}
		else
		{
			std::cout << "Неверный ввод"<< std::endl;
			break;
		}
	}
	return 0;
}
