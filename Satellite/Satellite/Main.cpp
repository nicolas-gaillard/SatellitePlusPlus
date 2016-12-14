#include "JudgeOutput.h"

int main() {
    DataReceiver rc =  DataReceiver("text/overlap.in");
    SimulationData data = rc.extractData();
    JudgeOutput jo("text/example.out", &data);
    if (jo.isValidOutput()) {
        std::cout << "Score final : " << jo.getScore() << std::endl;
    }
    else {
        std::cout << "invalid ouput" << std::endl;
    }
}
