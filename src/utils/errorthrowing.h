#pragma once

#ifndef ERRORTHROWING_H
#define ERRORTHROWING_H

#include <string>
#include <stdexcept>

namespace ErrorThrowing {
inline void uniformNotFound(int variableLocation, std::string variableName) {
    if (variableLocation == -1) {
        throw std::runtime_error("Uniform variable " + variableName + " not found");
    }
}
}

#endif // ERRORTHROWING_H
