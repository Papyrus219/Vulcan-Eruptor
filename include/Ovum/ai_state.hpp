#ifndef OVUM_AI_STATE_HPP
#define OVUM_AI_STATE_HPP

namespace ovum
{

enum class Ai_state
{
    RESTING,
    HUNTING,
    RETURN,
    DEAD
};

struct Ai_data
{
    float desire_y_rot{};
    float curr_y_rot{};

    float time_elapsed{};

    Ai_state state{};

    bool is_desire_rot{true};
};

}

#endif //OVUM_AI_STATE_HPP
