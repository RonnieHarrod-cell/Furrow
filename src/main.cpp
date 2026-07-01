#include <iostream>
#include "engine/core/Application.h"

int main() {
    try {
        engine::Application app;
        app.Run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
