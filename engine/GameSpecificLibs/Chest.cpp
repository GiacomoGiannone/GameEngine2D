#include "Chest.hpp"
#include "core/GameObject.hpp"
#include "graphics/SpriteRenderer.hpp"
#include "graphics/TextureManager.hpp"
#include "core/FontManager.hpp"
#include "graphics/Renderer.hpp"

namespace GE
{
    //constructor with two parameters, lootName is optional
    ChestLogic::ChestLogic(const std::string& openTextureId,
                           const std::filesystem::path& openTexturePath)
        : openTextureId(openTextureId), openTexturePath(openTexturePath), lootText(FontManager::getInstance().loadFont("debug_font", "assets/Fonts/arial.ttf"))
    {
        // Pre-carica subito la texture "aperta" così è già pronta in memoria
        // quando servirà, senza leggere da disco nel momento dell'interazione.
        // loadTexture incrementa il reference count; la scarichiamo nel distruttore.
        TextureManager::getInstance().loadTexture(this->openTextureId, this->openTexturePath);
    }

    ChestLogic::ChestLogic(const std::string& openTextureId,
                           const std::filesystem::path& openTexturePath,
                           const std::string& lootName)
        : openTextureId(openTextureId), openTexturePath(openTexturePath), lootName(lootName), lootText(FontManager::getInstance().loadFont("debug_font", "assets/Fonts/arial.ttf"))
    {
        // Pre-carica subito la texture "aperta" così è già pronta in memoria
        // quando servirà, senza leggere da disco nel momento dell'interazione.
        // loadTexture incrementa il reference count; la scarichiamo nel distruttore.
        TextureManager::getInstance().loadTexture(this->openTextureId, this->openTexturePath);

        lootText.setString(lootName);
        lootText.setCharacterSize(14);
        lootText.setFillColor(sf::Color::White);
        lootText.setOutlineColor(sf::Color::Black);
        lootText.setOutlineThickness(1.0f);

        // Centra l'origine orizzontalmente sul testo, così setPosition lo centra sopra la chest
        const auto bounds = lootText.getLocalBounds();
        lootText.setOrigin({bounds.size.x / 2.0f, bounds.size.y});
    }

    ChestLogic::~ChestLogic()
    {
        // Rilascia il riferimento che abbiamo noi alla texture "aperta".
        // Quella "chiusa" la scarica già lo SpriteRenderer per conto suo.
        TextureManager::getInstance().unloadTexture(openTextureId);
    }

    void ChestLogic::open()
    {
        if (state == ChestState::Open)
            return;

        state = ChestState::Open;

        if (getOwner() != nullptr && getOwner()->hasComponentOfType<SpriteRenderer>())
        {
            auto& sprite = getOwner()->getComponentOfType<SpriteRenderer>();
            sf::Texture& texture = TextureManager::getInstance().getTexture(openTextureId);
            sprite.setTexture(texture);
        }

        // TODO: qui puoi disattivare la CollisionBox, far spawnare il loot, ecc.
        
        //solo per debug, renderizziamo il loot come un testo sopra il chest

    }

    void ChestLogic::update(float deltaTime)
    {
        (void)deltaTime;

        if (state == ChestState::Open || player == nullptr || getOwner() == nullptr)
            return;

        const float dx = player->getX() - getOwner()->getX();
        const float dy = player->getY() - getOwner()->getY();
        const float distSquared = dx * dx + dy * dy;

        const bool inRange = distSquared <= (interactionRange * interactionRange);
        const bool keyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);

        // "appena premuto", non ogni frame in cui resta giù
        if (inRange && keyPressed && !wasKeyPressed)
        {
            open();
        }

        wasKeyPressed = keyPressed;
    }

    void ChestLogic::render(Renderer& renderer)
    {
        if (!isOpen() || getOwner() == nullptr)
            return;

        lootText.setPosition({
            getOwner()->getX(),
            getOwner()->getY() - 40.0f   // offset sopra la chest, regola in base alle dimensioni sprite
        });

        renderer.draw(lootText);
    }

    //set Loot has to repeat what the constructor with 3 parameters does
    void ChestLogic::setLoot(const std::string& lootItem)
    {
        lootName = lootItem;
        lootText.setString(lootName);
        lootText.setCharacterSize(14);
        lootText.setFillColor(sf::Color::White);
        lootText.setOutlineColor(sf::Color::Black);
        lootText.setOutlineThickness(1.0f);

        // Centra l'origine orizzontalmente sul testo, così setPosition lo centra sopra la chest
        const auto bounds = lootText.getLocalBounds();
        lootText.setOrigin({bounds.size.x / 2.0f, bounds.size.y});
    }
}