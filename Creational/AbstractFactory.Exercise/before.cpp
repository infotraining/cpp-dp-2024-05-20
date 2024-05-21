#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

// #define MOTIF

enum class IconType
{
    none,
    ok,
    cancel,
    warning,
    error
};

class Widget
{
public:
    virtual void draw() = 0;
    virtual ~Widget() = default;
};

class Button : public Widget
{
    std::string caption_;
    IconType icon_type_;

public:
    Button(const std::string& caption, IconType icon_type)
        : caption_{caption}
        , icon_type_{icon_type}
    {
    }

    std::string caption() const
    {
        return caption_;
    }

    IconType icon() const
    {
        return icon_type_;
    }
};

class Menu : public Widget
{
    std::string text_;

public:
    Menu(const std::string& text)
        : text_{text}
    {
    }

    std::string text() const
    {
        return text_;
    }
};

class MotifButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "MotifButton [ " << caption() << " ]\n";
    }
};

class MotifMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "MotifMenu { " << text() << " }\n";
    }
};

class WindowsButton : public Button
{
public:
    using Button::Button;

    void draw() override
    {
        cout << "WindowsButton [ " << caption() << " ]\n";
    }
};

class WindowsMenu : public Menu
{
public:
    using Menu::Menu;

    void draw() override
    {
        cout << "WindowsMenu { " << text() << " }\n";
    }
};

/////////////////////////////////////////////////

class SystemFactory
{
public:
    virtual ~SystemFactory() = default;
    virtual std::unique_ptr<Menu> createMenu(const std::string&) = 0;
    virtual std::unique_ptr<Button> createButton(const std::string&, IconType) = 0;
};

class WindowsFactory : public SystemFactory
{
public:
    std::unique_ptr<Menu> createMenu(const std::string& text) override
    {
        return std::make_unique<WindowsMenu>(text);
    }

    std::unique_ptr<Button> createButton(const std::string& caption, IconType icon_type)
    {
        return std::make_unique<WindowsButton>(caption, icon_type);
    }
};

class MotifFactory : public SystemFactory
{
public:
    std::unique_ptr<Menu> createMenu(const std::string& text) override
    {
        return std::make_unique<MotifMenu>(text);
    }

    std::unique_ptr<Button> createButton(const std::string& caption, IconType icon_type)
    {
        return std::make_unique<MotifButton>(caption, icon_type);
    }
};

//////////////////////////////////////

class Window
{
    std::vector<std::unique_ptr<Widget>> widgets;

public:
    void display() const
    {
        std::cout << "######################\n";
        for (const auto& w : widgets)
            w->draw();
        
        std::cout << "######################\n\n";
    }

    void add_widget(std::unique_ptr<Widget> widget)
    {
        widgets.push_back(std::move(widget));
    }
};

class WindowOne : public Window
{
public:
    WindowOne(SystemFactory& widget_factory)
        : widget_factory{widget_factory}
    {
        add_widget(widget_factory.createMenu("File"));
        add_widget(widget_factory.createButton("OK", IconType::ok));
    }

    SystemFactory& widget_factory;
};

class WindowTwo : public Window
{
public:
    WindowTwo(SystemFactory& widget_factory)
        : widget_factory{widget_factory}
    {
        add_widget(widget_factory.createMenu("Edit"));
        add_widget(widget_factory.createButton("OK", IconType::ok));
        add_widget(widget_factory.createButton("Cancel", IconType::cancel));
    }

    SystemFactory& widget_factory;
};

int main(void)
{
#ifdef MOTIF
    MotifFactory widget_factory;
#else // WINDOWS
    WindowsFactory widget_factory;
#endif

    WindowOne w1(widget_factory);
    w1.display();

    WindowTwo w2(widget_factory);
    w2.display();
}
