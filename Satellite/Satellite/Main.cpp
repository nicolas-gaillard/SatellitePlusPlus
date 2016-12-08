#include "JudgeOutput.h"

int main() {
    DataReceiver rc =  DataReceiver("text/overlap.in");
    SimulationData data = rc.extractData();
    JudgeOutput jo("text/example.out");
    Collection * arrayCol = data.getArrayCol();
    if (jo.isValidOutput()) {
        std::cout << "Score final : " << jo.getScore(arrayCol, data.getNbCollection()) << std::endl;
    }
}
