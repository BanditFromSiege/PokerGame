#include "Render_Logic/Render_manager.h"
#include <chrono>

int main() {
    auto manager = std::make_unique<Render_manager>(std::pair{ 1920, 1080 });
    manager->run();
    
    return 0;
}