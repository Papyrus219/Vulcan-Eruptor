#ifndef ERUPTOR_EVENT_EVENT_HPP
#define ERUPTOR_EVENT_EVENT_HPP

#include <Eruptor/lib/event/keys.hpp>

namespace eruptor::event
{

enum class Event_type
{
    CLOSE_WINDOW,
    KEY_PRESSED,
    KEY_RELEASE,
    MOUSE_MOVED,
    MOUSE_SCROLL
};

struct Event
{
    Key key_type{};
    float mouse_move_x_offset{};
    float mouse_move_y_offset{};
    float mouse_scroll_offset{};

    Event_type type{};
    bool procesed{};
};

}

#endif //ERUPTOR_EVENT_EVENT_HPP
