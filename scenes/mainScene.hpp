#pragma once

// Inherits from Scene and defines a main scene with
// some GameObjects that have a rectangle renderer component.

#include "core/Scene.hpp"
#include "core/GameObject.hpp"
#include "graphics/Color.hpp"
#include "graphics/RectangleRenderer.hpp"
#include "core/CharacterController.hpp"
#include "core/CollisionBox.hpp"
#include "graphics/SpriteRenderer.hpp"

class MainScene : public GE::Scene
{
private:
    GE::GameObject* rectangleObject;
    GE::GameObject* rectangleObject2;
    GE::GameObject* rectangleObject3;
    GE::GameObject* rectangleObject4;

public:
    MainScene() : GE::Scene("MainScene")
    {
        // Player-controlled rectangle (camera will follow this).
        rectangleObject = new GE::GameObject();
        rectangleObject->getTransform().setPosition(250.0f, 180.0f);

        auto& characterController = rectangleObject->addComponent<GE::CharacterController>(
            200.0f, // speed
            20.0f, // width of the collision box
            40.0f  // height of the collision box
        );
        //get the character controller to print debug info
        rectangleObject->getComponentOfType<GE::CharacterController>().setDebugPrint(true);
        //set this variable to true to enable Y movement with W and S keys
        rectangleObject->getComponentOfType<GE::CharacterController>().setYMovementEnabled(true);

        //add sprite renderer to rectangleObject
        rectangleObject->addComponent<GE::SpriteRenderer>(
            "player_texture",
            "assets/player.png",
            rectangleObject->getTransform()
        );
        
        rectangleObject2 = new GE::GameObject();
        rectangleObject2->getTransform().setPosition(100.0f, 400.0f);
        rectangleObject2->addComponent<GE::RectangleRenderer>(
            200.0f,
            100.0f,
            GE::Color(255, 100, 100),
            rectangleObject2->getTransform()
        );
        //add collision box to rectangleObject2
        rectangleObject2->addComponent<GE::CollisionBox>(200.0f, 100.0f);
        addGameObject(rectangleObject2);

        rectangleObject3 = new GE::GameObject();
        rectangleObject3->getTransform().setPosition(550.0f, 100.0f);
        rectangleObject3->addComponent<GE::RectangleRenderer>(
            150.0f,
            250.0f,
            GE::Color(100, 255, 100),
            rectangleObject3->getTransform()
        );
        //add collision box to rectangleObject3
        rectangleObject3->addComponent<GE::CollisionBox>(150.0f, 250.0f);
        addGameObject(rectangleObject3);

        rectangleObject4 = new GE::GameObject();
        rectangleObject4->getTransform().setPosition(400.0f, 350.0f);
        rectangleObject4->addComponent<GE::RectangleRenderer>(
            120.0f,
            120.0f,
            GE::Color(255, 255, 100),
            rectangleObject4->getTransform()
        );
        //add collision box to rectangleObject4
        rectangleObject4->addComponent<GE::CollisionBox>(120.0f, 120.0f);
        rectangleObject4->setRenderOrder(100); // Set a higher render order for this object
        rectangleObject3->setRenderOrder(100); // Set a lower render order for this object
        rectangleObject2->setRenderOrder(100); // Set a lower render order for this object
        rectangleObject->setRenderOrder(100); // Set a lower render order for this object

        addGameObject(rectangleObject4);

        addGameObject(rectangleObject);
        characterController.setWorldObjects(&getGameObjects());

        setCameraTarget(rectangleObject);

        //get the camera and set a larger viewport size to see more of the scene
        getCamera().setViewportSize(800.0f, 600.0f);
    }

    virtual ~MainScene()
    {
        removeGameObject(rectangleObject);
        delete rectangleObject;

        removeGameObject(rectangleObject2);
        delete rectangleObject2;

        removeGameObject(rectangleObject3);
        delete rectangleObject3;

        removeGameObject(rectangleObject4);
        delete rectangleObject4;
    }

    // Used by the engine/camera to follow the player rectangle.
    GE::GameObject* getPlayer() const { return rectangleObject; }
};
