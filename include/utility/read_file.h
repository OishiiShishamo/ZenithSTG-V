#pragma once
#ifndef ZENITHSTGV_INCLUDE_UTILITY_READ_FILE_H_
#define ZENITHSTGV_INCLUDE_UTILITY_READ_FILE_H_

#include <cstdio>
#include <fstream>
#include <vector>

namespace zenithstgv {
namespace Utility {
std::vector<char> ReadFile(const std::string &filename);
}
} // namespace zenithstgv

#endif
