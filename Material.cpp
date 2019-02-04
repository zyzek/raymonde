#include <iostream>

#include "Material.hpp"

std::ostream &operator<<(std::ostream &out, const Material &material) {
    out << "Material(" << material.diffuse_colour << ")";
    return out;
}