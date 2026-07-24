#include <gp_communicator.hpp>
#include <stdexcept>

using namespace ovum;

ovum::GP_communicator::GP_communicator()
{
    gnuplot = popen("gnuplot -persist", "w");

    if(!gnuplot)
    {
        throw std::runtime_error{"Failed to open stream with gnuplot,"};
    }
}

void ovum::GP_communicator::Begin_frame()
{
    if(mode == GP_mode::MODE_2D)
    {
        fprintf(gnuplot, "plot '-' with points pointtype 7\n");
    }
    else if(mode == GP_mode::MODE_3D)
    {
        fprintf(gnuplot, "splot '-' with points pointtype 7\n");
    }
    else
    {
        throw(std::runtime_error{"Gnuplot comunicator hasn't set mode of use in moment of use."});
    }
}

void ovum::GP_communicator::End_frame()
{
    fprintf(gnuplot, "e\n");
    fflush(gnuplot);
}

void ovum::GP_communicator::Enable_2d(const std::string & title)
{
    fprintf(gnuplot, "set title '%s'\n",  title.c_str());

    fprintf(gnuplot, "set xrange [-3:3]\n");
    fprintf(gnuplot, "set yrange [-3:3]\n");

    mode = GP_mode::MODE_2D;
}

void ovum::GP_communicator::Enable_3d(const std::string & title)
{
    fprintf(gnuplot, "set title '%s'\n",  title.c_str());

    fprintf(gnuplot, "set xrange [-3:3]\n");
    fprintf(gnuplot, "set yrange [-3:3]\n");
    fprintf(gnuplot, "set zrange [-3:3]\n");

    mode = GP_mode::MODE_3D;
}

void ovum::GP_communicator::Set_title(const std::string & title)
{
    fprintf(gnuplot, "set title '%s'\n",  title.c_str());
}

void ovum::GP_communicator::Set_x_axis_range(float min, float max)
{
    fprintf(gnuplot, "set xrange [%f:%f]\n", min, max);
}

void ovum::GP_communicator::Set_y_axis_range(float min, float max)
{
    fprintf(gnuplot, "set yrange [%f:%f]\n", min, max);
}

void ovum::GP_communicator::Set_z_axis_range(float min, float max)
{
    fprintf(gnuplot, "set zrange [%f:%f]\n", min, max);
}

void ovum::GP_communicator::Set_x_axis_title(const std::string & title)
{
    fprintf(gnuplot, "set xlabel '%s'\n", title.c_str());
}

void ovum::GP_communicator::Set_y_axis_title(const std::string & title)
{
    fprintf(gnuplot, "set ylabel '%s'\n", title.c_str());
}

void ovum::GP_communicator::Set_z_axis_title(const std::string & title)
{
    fprintf(gnuplot, "set zlabel '%s'\n", title.c_str());
}

void ovum::GP_communicator::Stage_data(glm::vec3 vec3)
{
    fprintf(gnuplot, "%f %f %f\n", vec3.x, vec3.y, vec3.z);
}

void ovum::GP_communicator::Stage_data(glm::vec2 vec2)
{
    fprintf(gnuplot, "%f %f\n", vec2.x, vec2.y);
}

void ovum::GP_communicator::Stage_data(float x)
{
    fprintf(gnuplot, "%f\n", x);
}

ovum::GP_communicator::~GP_communicator()
{
    if(gnuplot)
    {
        pclose(gnuplot);
    }
}
