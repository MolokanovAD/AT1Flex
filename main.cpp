#include "Recognizer.h"
#include <fstream>

int main(int argc, char* argv[]) {
	int retcode = 0;
    if (argc < 2)
    {
        std::cerr << "No file to check." << std::endl;
        retcode = 2;
    }
    else if (argc > 3)
    {
        std::cerr << "Only two arguments accepted." << std::endl;
        retcode = 3;
    }
    else if (argc == 3 && std::string(argv[2]) != "TIMING") {
        std::cerr << "Wrong flag." << std::endl;
        retcode = 3;
    }
    else {
        Recognizer rec("Test.txt");
        if (argc == 2) {
            try {
                rec.run();
            }
            catch (std::runtime_error& a) {
                std::cout << a.what();
            }
        }
        else {
            try {
                rec.timing();
            }
            catch (std::runtime_error& a) {
                std::cout << a.what();
            }
            std::cout << rec.linesChecked() << " strings checked in " << rec.showTime().count() << " ns" << std::endl;
        }
        rec.write_result();
    }
    /*std::string timingF("Output.txt");
    std::ofstream of(timingF);
    for (int strings = 10; strings <= 1000;) {
            for (int len = 50; len <= 10000;) {
                std::string filename("C:\\Users\\molok\\source\\repos\\ProjectAT1Regex\\Timing_" + std::to_string(strings) + "_" + std::to_string(len) + ".txt");
                std::ifstream istr(filename);
                Recognizer thisContext(filename);
                long long count = 0;
                thisContext.timing();
                std::cout << filename << " END " << std::endl;
                of << "FLEX  " << thisContext.linesChecked() << " strings, length" << len << " checked in " << thisContext.showTime().count() << " ns" << std::endl;
                switch (len)
                {
                case 50: {len = 200; break; }
                case 200: {len = 500; break; }
                case 500: {len = 1000; break; }
                case 1000: {len = 10000; break; }
                case 10000: {len = 100000; break; }
                default:
                    len += 1;
                }
            }
            switch (strings)
            {
            case 10: {strings = 200; break; }
            case 200: {strings = 500; break; }
            case 500: {strings = 1000; break; }
            default:
                strings += 1;
            }
        }*/
	return retcode;
}