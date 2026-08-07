#ifndef OVUM_AI_STATE_HPP
#define OVUM_AI_STATE_HPP

namespace ovum
{

struct Ai_state
{
    float desire_y_rot{};
    float curr_y_rot{};

    float time_elapsed{};

    bool is_desire_rot{true};
};

}

#endif //OVUM_AI_STATE_HPP
