#ifndef TEXT_HPP
#define TEXT_HPP

#include "paragraph.hpp"
#include "shape.hpp"

#include <string>

namespace Drawing
{
    class Text : public ShapeBase, private LegacyCode::Paragraph
    {
    public:
        static constexpr const char* id = "Text";

        void draw() const override;

        void set_text(const std::string& text);
        const std::string get_text() const;
    };
}

#endif
