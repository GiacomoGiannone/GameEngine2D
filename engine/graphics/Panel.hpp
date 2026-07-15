#pragma once

#include <string>

namespace GE
{

}

namespace GE
{
    class Panel
    {
        private:
            std::string text;

        public:
            Panel() = default;
            virtual ~Panel() = default;

            virtual void update(float deltaTime) = 0;
            virtual void render() = 0;
    };
}