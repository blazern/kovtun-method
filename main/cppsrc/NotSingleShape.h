#ifndef NOTSINGLESHAPE_H
#define NOTSINGLESHAPE_H

#include <stdexcept>

class NotSingleShape : public std::invalid_argument
{
public:
    explicit NotSingleShape(const std::string & what) : std::invalid_argument(what) {}
    virtual ~NotSingleShape() noexcept {}
};

#endif // NOTSINGLESHAPE_H
