#pragma once

#include <functional>

#include "Widget.hpp"

class Button : public Widget {
public:
    explicit Button(const uf::vec2i &size = {});
    Button(const sf::String &, const uf::vec2i &size, std::function<void()>);

    void Update(sf::Time timeElapsed) override final;
    bool HandleEvent(sf::Event event) override final;

    void SetString(const sf::String &string);
    void SetUnderCursorStyle(const Style &);
    void SetUnderCursorStyle(Style &&);

protected:
    void draw() const override final;

private:
    sf::Text text;

	// underCursor options
    Style underCursor_style;
    bool underCursor;
    //bool cursorDetected;

    std::function<void()> onPressFunc;
};