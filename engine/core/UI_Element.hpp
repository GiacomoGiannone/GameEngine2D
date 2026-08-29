#pragma once

namespace GE
{
    class Renderer;
}

namespace GE
{
    class UI_Element
    {
    public:
        virtual void Update(float deltaTime) = 0;
        virtual void Render(GE::Renderer& renderer) = 0;
        virtual ~UI_Element() = default;
    };
}