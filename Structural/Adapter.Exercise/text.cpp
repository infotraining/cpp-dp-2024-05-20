#include "text.hpp"

#include "shape_factories.hpp"

using Drawing::Text;

namespace
{
    bool is_registered = Drawing::SingletonShapeFactory::instance()
                             .register_creator(Text::id, []
                                 { return std::make_unique<Text>(); });
}

void Text::draw() const
{
    render_at(coord().x, coord().y);
}

void Text::set_text(const std::string& text)
{
    set_paragraph(text.c_str());
}

const std::string Text::get_text() const
{
    return std::string(get_paragraph());
}