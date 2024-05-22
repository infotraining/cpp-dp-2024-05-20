#include "bitmap.hpp"
#include <algorithm>
#include <array>
#include <deque>

using namespace std;

struct Bitmap::Implementation
{
    std::deque<char> image_;

    Implementation(size_t size, char c) : image_(size, c)
    {}
};

Bitmap::Bitmap(size_t size, char fill_char) : pimpl_{std::make_unique<Implementation>(size, fill_char)}
{
}

Bitmap::~Bitmap() = default;

void Bitmap::draw()
{
    cout << "Image: ";
    for (size_t i = 0; i < pimpl_->image_.size(); ++i)
        cout << pimpl_->image_[i];
    cout << endl;
}
