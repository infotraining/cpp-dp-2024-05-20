#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <typeindex>

#include "rectangle.hpp"
#include "shape.hpp"
#include "shape_readers_writers/rectangle_reader_writer.hpp"
#include "shape_readers_writers/square_reader_writer.hpp"
#include "square.hpp"

using namespace std;
using namespace Drawing;
using namespace Drawing::IO;

// unique_ptr<Shape> create_shape(const string& id)
// {
//     if (id == Rectangle::id)
//         return make_unique<Rectangle>();
//     else if (id == Square::id)
//         return make_unique<Square>();

//     throw runtime_error("Unknown shape id");
// }

// unique_ptr<ShapeReaderWriter> create_shape_rw(Shape& shape)
// {
//     if (typeid(shape) == typeid(Rectangle))
//         return make_unique<RectangleReaderWriter>();
//     else if (typeid(shape) == typeid(Square))
//         return make_unique<SquareReaderWriter>();

//     throw runtime_error("Unknown shape id");
// }

//////////////////////////////////////////////////////////////////////
template <
    typename TProduct, 
    typename TId = std::string,
    typename TCreator = std::function<std::unique_ptr<TProduct>()>
>
class GenericFactory
{
    std::unordered_map<TId, TCreator> creators_;
public:
    bool register_creator(TId id, TCreator shape_creator)
    {
        return creators_.emplace(std::move(id), shape_creator).second;
    }

    std::unique_ptr<TProduct> create(const TId& id)
    {
        auto& shape_creator = creators_.at(id);
        return shape_creator();
    }
};

using ShapeFactory = GenericFactory<Shape>;
using ShapeRWFactory = GenericFactory<ShapeReaderWriter, std::type_index>;

class GraphicsDoc
{
    vector<unique_ptr<Shape>> shapes_;
    ShapeFactory& shape_factory_;
    ShapeRWFactory& shape_rw_factory_;

public:
    GraphicsDoc(ShapeFactory& shape_factory, ShapeRWFactory& shape_rw_factory) 
        : shape_factory_{shape_factory}, shape_rw_factory_{shape_rw_factory}

    {}

    void add(unique_ptr<Shape> shp)
    {
        shapes_.push_back(std::move(shp));
    }

    void render()
    {
        for (const auto& shp : shapes_)
            shp->draw();
    }

    void load(const string& filename)
    {
        ifstream file_in{filename};

        if (!file_in)
        {
            cout << "File not found!" << endl;
            exit(1);
        }

        while (file_in)
        {
            string shape_id;
            file_in >> shape_id;

            if (!file_in)
                return;

            cout << "Loading " << shape_id << "..." << endl;

            auto shape = shape_factory_.create(shape_id);
            auto shape_rw = shape_rw_factory_.create(std::type_index{typeid(*shape)});

            shape_rw->read(*shape, file_in);

            shapes_.push_back(std::move(shape));
        }
    }

    void save(const string& filename)
    {
        ofstream file_out{filename};

        for (const auto& shp : shapes_)
        {
            auto shape_rw = shape_rw_factory_.create(std::type_index{typeid(*shp)});
            shape_rw->write(*shp, file_out);
        }
    }
};

int main()
{
    cout << "Start..." << endl;

    // bootstrapping
    ShapeFactory shape_factory;
    shape_factory.register_creator(Rectangle::id, [] { return std::make_unique<Rectangle>();});
    shape_factory.register_creator(Square::id, [] { return std::make_unique<Square>();});

    ShapeRWFactory shape_rw_factory;
    shape_rw_factory.register_creator(std::type_index{typeid(Rectangle)}, [] { return std::make_unique<RectangleReaderWriter>(); });
    shape_rw_factory.register_creator(std::type_index{typeid(Square)}, [] { return std::make_unique<SquareReaderWriter>(); });

    GraphicsDoc doc{shape_factory, shape_rw_factory};

    doc.load("drawing_fm_example.txt");

    cout << "\n";

    doc.render();

    doc.save("new_drawing.txt");
}
