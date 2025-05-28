//taliyariel1@gmail.com
#pragma once
#include <stdio.h>

enum class Role{
    Governor, 
    Spy, 
    Baron,
    General, 
    Judge, 
    Merchant
};

//Cast role enum member to a string
inline std::string to_string(Role role) {
    switch (role) {
        case Role::Baron: return "Baron";
        case Role::Spy: return "Spy";
        case Role::Governor: return "Governor";
        case Role::General: return "General";
        case Role::Merchant: return "Merchant";
        case Role::Judge: return "Judge";
        default: return "Unknown";
    }
}