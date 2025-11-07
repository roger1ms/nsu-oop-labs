#include "StrategyFactory.h"
#include "Strategy.cpp"

StrategyFactory::StrategyFactory() {
    add<AllCStrategy>("allc");
    add<AllDStrategy>("alld");
    add<RandomStrategy>("random");
    add<RevengeStrategy>("revenge");
    add<RememberFirstEnemyStrategy>("youmyenemy");
    add<AnalyzerStrategy>("analyzer");
    add<CopyBestPlayerStrategy>("copybest");
    add<MixOfStrategies>("strategymix");
    add<FindBestStrategy>("findbest");
    add<ProvocationStrategy>("provocate");
    add<CycleAgressionStrategy>("cycled");
}

Strategy* StrategyFactory::create(const std::string& name) const {
    auto it = creators.find(name);
    if (it != creators.end()) {
        return it->second();
    }
    std::cout << "Unknown strategy: " << name << std::endl;
    return nullptr;
}

void StrategyFactory::printAvailableStrategies() const {
    std::cout << "Available strategies:" << std::endl;
    std::cout << "  allc         - Always cooperate" << std::endl;
    std::cout << "  alld         - Always defect" << std::endl;
    std::cout << "  random       - Random moves" << std::endl;
    std::cout << "  revenge      - Eye for eye" << std::endl;
    std::cout << "  provocate    - Byte enemies to defect" << std::endl;
    std::cout << "  cycled       - Cycle defects" << std::endl;
    std::cout << "  youmyenemy   - Remember first enemy" << std::endl;
    std::cout << "  analyzer     - Analyze opponents behavior" << std::endl;
    std::cout << "  copybest     - Copy the best player" << std::endl;
    std::cout << "  strategymix  - Mix of strategies" << std::endl;
    std::cout << "  findbest     - Learning strategy" << std::endl;
}