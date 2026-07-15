#pragma once 

namespace GE
{
    class Transform
    {
    private:
        float x_position, y_position;
        float rotation;
        float scale_x, scale_y;
        float width, height;
    public:
        Transform() : x_position(0.0f), y_position(0.0f), rotation(0.0f), scale_x(1.0f), scale_y(1.0f), width(0.0f), height(0.0f) {}
        virtual ~Transform() = default;

        void setPosition(float x, float y) { x_position = x; y_position = y; }
        void setRotation(float rot) { rotation = rot; }
        void setScale(float x, float y) { scale_x = x; scale_y = y; }
        float getX() const { return x_position; }
        float getY() const { return y_position; }
        float getRotation() const { return rotation; }
        float getScaleX() const { return scale_x; }
        float getScaleY() const { return scale_y; }
        float getWidth() const { return width; }
        float getHeight() const { return height; }
        void setWidth(float w) { width = w; }
        void setHeight(float h) { height = h; }
    };
}