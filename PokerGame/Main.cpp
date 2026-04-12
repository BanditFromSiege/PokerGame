#include "Render_Logic/Render_manager.h"
#include <chrono>

int main() {
    std::mt19937_64 rng{ std::random_device{}() };

    auto manager = std::make_unique<Render_manager>(rng, std::pair{ 1920, 1080 }, 8);
    manager->run();

    return 0;
}