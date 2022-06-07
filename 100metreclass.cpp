#include "cuda_runtime.h"
#include"device_launch_parameters.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include<stdio.h>

#define INITIAL 0
#define INITIAL2 100

using namespace std;


__global__ void kernel(Runner* runner, Runner* runner_pos);
    

class Runners {

public:

    Runners() {

        cudaMalloc((void**)&pos, sizeof(int) * INITIAL);
        cudaMalloc((void**)&speed, sizeof(int) * INITIAL);
        cudaMalloc((void**)&totalRoad, sizeof(int) * INITIAL2);

        cudaMemset((void*)pos, 0, sizeof(int) * INITIAL);
        cudaMemset((void*)speed, 0, sizeof(int) * INITIAL);
        cudaMemset((void*)totalRoad, 0, sizeof(int) * INITIAL2);
    }

    ~Runners() {
        cudaFree((void*)pos);
        cudaFree((void*)speed);
        cudaFree((void*)totalRoad);

    }

    void position(int *pos, int *speed, int *totalRoad) {

        srand(time(0));
        speed = (rand() % 5) + 1;

        std::cout << "Starting Speed: " << speed << " " << "m/sn" << std::endl;

        pos = pos + speed;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "Instant Location(m): " << pos << std::endl;

        if (pos > (totalRoad - 1)) {
            std::cout << "Runner finished the race" << std::endl;
            std::cout << "----------------------" << std::endl;
            pos = totalRoad;
        }
        else {
            std::cout << "Runner continues the race" << std::endl;
            std::cout << "----------------------" << std::endl;
        }
    }

private:
    int *pos;
    int *speed;
    int *totalRoad;

};


__global__ void kernel(Runner* runner, Runner* runner_pos) {

  
    int i = threadIdx.x;

    runner_pos[i] = runner[i];

}

int main()
{

        Runners runner[99] = {};
        Runners runner_pos[99] = {};

        for (int i; i < sizeof(runner_pos); i++) {

            runner_pos[i] = runner[i];
        }

        //create pointer into the gpu
        Runners* cudaR = 0;
        Runners* cudaR2 = 0;

        //allocate memory in the gpu
        cudaMalloc(&cudaR, sizeof(runner));
        cudaMalloc(&cudaR2, sizeof(runner_pos));

        //copy the vectors into the gpu
        cudaMemcpy(cudaR, runner, sizeof(runner), cudaMemcpyHostToDevice);

        position <<<1, sizeof(runner) >>> (cudaR);

        cudaMemcpy(cudaR, runner, sizeof(runner_pos), cudaMemcpyDeviceToHost);

    return 0;
}
