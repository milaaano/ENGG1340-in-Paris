#include <chrono>
#include <iostream>
#include <string>
#include <thread>
// Don't forget to run g++ -std=c++11 -O2 testing.cpp -o testing before executing
using namespace std;

class Animation {
public:
    void run() {
        initialize();
        while (frameCount < maxFrames) {
            auto start = chrono::high_resolution_clock::now();

            update();
            render();

            auto end = chrono::high_resolution_clock::now();
            auto elapsed = end - start;
            // frameDuration is a chrono::milliseconds
            auto sleepTime = frameDuration - chrono::duration_cast<chrono::milliseconds>(elapsed);

            if (sleepTime > chrono::milliseconds(0)) {
                this_thread::sleep_for(sleepTime);
            }
            frameCount++;
        }
    }

protected:
    int frameCount = 0;
    const int maxFrames = 100;
    // Explicitly construct a 50ms duration
    const chrono::milliseconds frameDuration{50};

    virtual void initialize() {}
    virtual void update() {}
    virtual void render() {
        // Clear screen (ANSI escape codes)
        cout << "\033[2J\033[H";
    }
};

class BoxAnimation : public Animation {
    int x = 0;
    int direction = 1;

    void update() override {
        x += direction;
        if (x >= 20 || x <= 0)
            direction = -direction;
    }
void printHeader() {
    cout << CYAN;
    cout << "######     ##    ######  ######  ##      ######  ######  ######  ##  ##  ###### ####### ######      \n";
    cout << "##   #   ##  ##    ##      ##    ##      ##      ##      ##      ##  ##    ##   ##   ## ##    \n";
    cout << "######   ######    ##      ##    ##      ######  ######  ######  ######    ##   ####### ######    \n";
    cout << "##   #   ##  ##    ##      ##    ##      ##      ##          ##  ##  ##    ##   ##          ##  \n";
    cout << "######   ##  ##    ##      ##    ######  ######  ######  ######  ##  ##  ###### ##      ###### \n";


        cout << RESET;


}
    void render() override {
        Animation::render();
        // print a '-' at position x
        cout << string(x, ' ') << "-\n";
        cout << "Frame: " << frameCount << "/" << maxFrames << endl;
    }
};

int main() {
    BoxAnimation animation;
    animation.run();
    return 0;
}
