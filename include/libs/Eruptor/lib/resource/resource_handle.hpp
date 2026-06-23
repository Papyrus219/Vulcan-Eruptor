#ifndef ERUPTOR_RESOURCE_RESOURCE_HANDLE_HPP
#define ERUPTOR_RESOURCE_RESOURCE_HANDLE_HPP

#include <cstdint>

namespace eruptor::resource
{

class Resource_manager;

template<typename T>
class Resource_handle
{
public:
    Resource_handle() = default;
    Resource_handle(const uint32_t & id, Resource_manager * manager);

    T* Get() const;

    bool Is_valid() const;

    const uint32_t & Get_id() const;

    T* operator->() const;

    T& operator*() const;

    operator bool() const;

private:
    uint32_t resource_id{};
    Resource_manager * resource_manager{};
};

}

#endif // ERUPTOR_RESOURCE_RESOURCE_HANDLE_HPP
