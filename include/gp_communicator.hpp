#ifndef OVUM_GP_COMMUNICATOR_HPP
#define OVUM_GP_COMMUNICATOR_HPP

#include <glm/glm.hpp>
#include <string>
#include <cstdio>


namespace ovum
{

enum class GP_mode
{
    NONE,
    MODE_2D,
    MODE_3D
};

class GP_communicator
{
public:
    GP_communicator();

    void Begin_frame();
    void End_frame();

    void Enable_2d(const std::string & title);
    void Enable_3d(const std::string & title);

    void Set_title(const std::string & title);

    void Set_x_axis_range(float min, float max);
    void Set_y_axis_range(float min, float max);
    void Set_z_axis_range(float min, float max);

    void Set_x_axis_title(const std::string & title);
    void Set_y_axis_title(const std::string & title);
    void Set_z_axis_title(const std::string & title);

    void Stage_data(glm::vec3 vec3);
    void Stage_data(glm::vec2 vec2);
    void Stage_data(float x);

    ~GP_communicator();

private:
    FILE * gnuplot{};
    GP_mode mode{};
};

}

#endif // OVUM_GP_COMMUNICATOR_HPP
