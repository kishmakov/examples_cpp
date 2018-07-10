#include <chrono>
#include <fstream>
#include <iomanip>

#include <unistd.h>

struct Logger 
{
    const std::string FILE_NAME = "log.txt";

    Logger() : start_(std::chrono::high_resolution_clock::now()) 
    { 
    	std::ofstream fout(FILE_NAME, std::ios::out);
    };

    template<typename T> 
    Logger& operator<<(const T& t) 
    {
    	std::ofstream fout(FILE_NAME, std::ios::app);
    	auto now = std::chrono::high_resolution_clock::now();
        fout
        	<< std::setw(10) 
        	<< std::chrono::duration_cast<std::chrono::milliseconds>(now - start_).count() 
        	<< ": " << t << std::endl;
        return *this;
    }

private:
	const std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

int main(int /*argc*/, char** /*argv*/) 
{
    pid_t pid = 0;

    Logger logger;

    if ((pid = fork()) != 0)  {
        _exit(0);
    }

    setsid();

    if ((pid = fork()) != 0) {    	
    	_exit(0);    
    }         

    for (int i = 0; i < 10; ++i) {
    	usleep(10 * 1000 * 1000);
    	logger << std::to_string(i + 1) + " time intervals passed";
    }

    return 0;
}