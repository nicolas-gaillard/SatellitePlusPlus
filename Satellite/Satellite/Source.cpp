#include "Judge.h"
#include "TimeMeasure.h"
#include <ctime>
#include <time.h>		// time() == real time, clock() == processing time

// typedef std::chrono::high_resolution_clock Clock;

int main(int argc, char *argv[]) {
	/*
	std::cout << "[I] Measuring execution time test" << std::endl << std::endl;
	
	// Get a number between 1 and 30 :
    // int timeBreak = rand() % 30 + 1;

	std::cout << "Test 1 : ctime" << std::endl;
	std::clock_t start = std::clock();
	sleep(5);
	double duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
	std::cout << "Duration : " << duration << " seconds" << std::endl << std::endl;


	std::cout << "Test 2 : <chrono> nanosec" << std::endl;
	auto t1 = std::chrono::high_resolution_clock::now();;
    sleep(5);
	auto t2 = std::chrono::high_resolution_clock::now();
    std::cout << "Duration : " << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
              << " nanoseconds" << std::endl << std::endl;


	std::cout << "Test 3 : clock()" << std::endl;
	clock_t begin = clock();
	sleep(5);
	clock_t end = clock();
	double time = (double) (end-begin) / CLOCKS_PER_SEC * 1000.0;
	std::cout << "Duration : " << time << std::endl << std::endl;


	std::cout << "Test 4 : <chrono> ms" << std::endl;
	auto before = std::chrono::high_resolution_clock::now();
    sleep(5);
    auto after = std::chrono::high_resolution_clock::now();
	auto result = std::chrono::duration_cast<std::chrono::milliseconds>(after-before).count();
	std::cout << typeid(result).name() << std::endl;
	std::cout << static_cast<long>(result) << std::endl;
	std::cout << static_cast<double>(result) << std::endl;
    std::cout << "chrono 2 : "
              << result
              << " milliseconds" << std::endl;

	*/

	std::cout << "Test GetFilesInDirectory" << std::endl;
	std::vector<std::string> test;	
	
	std::string path("/Users/nicolaz/Desktop/Hashcode/Satellite/Satellite/text");

	TimeMeasure tm(path, "", "");

	tm.getFilesInDirectory(test);
	for(auto &it : test){
		std::cout << it << std::endl;
	}
}