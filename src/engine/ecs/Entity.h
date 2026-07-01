#pragma one

namespace engine
{
    enum class EntityType
    {
        Crop,
        // more later: fence, building, etc
    };

    enum class GrowthStage
    {
        Seed,
        Sprout,
        Grown,
    };

    struct Entity
    {
        EntityType type;
        int tileX;
        int tileZ;

        GrowthStage growthStage = GrowthStage::Seed;
        float growthTimer = 0.0f;
    };
} // namespace engine
