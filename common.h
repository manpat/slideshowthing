#ifndef COMMON_H
#define COMMON_H

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <string>
#include <memory>
#include "utilities/debug.h"

using std::string;
using std::wstring;
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;
using std::make_shared;

typedef shared_ptr<sf::RenderWindow> Window;

#endif