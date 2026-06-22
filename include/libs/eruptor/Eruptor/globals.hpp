#ifndef ERUPTOR_GLOBALS_HPP
#define ERUPTOR_GLOBALS_HPP

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

#endif //ERUPTOR_GLOBALS_HPP
