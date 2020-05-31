#include "SignColor.h"

SignColor SignColor::defaultFlipDiscBG =
        SignColor(Type::BACKGROUND, Status::DEFAULT, { 100, 100, 100 });
SignColor SignColor::defaultFlipDiscFG =
        SignColor(Type::FOREGROUND, Status::DEFAULT, { 255, 255, 0 });
SignColor SignColor::defaultMonoLEDBG =
        SignColor(Type::BACKGROUND, Status::DEFAULT, { 100, 100, 100 });
SignColor SignColor::defaultMonoLEDFG =
        SignColor(Type::FOREGROUND, Status::DEFAULT, { 255, 127, 0 });
SignColor SignColor::defaultRGBLEDBG =
        SignColor(Type::BACKGROUND, Status::DEFAULT, { 0, 0, 0 });
SignColor SignColor::defaultRGBLEDFG =
        SignColor(Type::FOREGROUND, Status::DEFAULT, { 255, 255, 255 });

SignColor SignColor::defaultFG =
        SignColor(Type::FOREGROUND, Status::DEFAULT);
SignColor SignColor::defaultBG =
        SignColor(Type::BACKGROUND, Status::DEFAULT);


SignColor::~SignColor() {

}

SignColor::SignColor(enum Type type, enum Status status, struct RGB value) :
    type(type), status(status), value(value) {
}

SignColor SignColor::defaultColor(enum Type type) {
    switch(type) {
    case FOREGROUND:
        return defaultFG;
        break;
    case BACKGROUND:
        return defaultBG;
        break;
    default:
        return defaultBG;
        break;
    }
}

SignColor SignColor::RGB(enum Type type, unsigned char r, unsigned char g,
        unsigned char b) {
    SignColor c(type, Status::CUSTOM, {r, g, b});
    return c;
}

enum SignColor::Type SignColor::getType() const {
    return this->type;
}

bool SignColor::isDefault() const {
    return this->status == Status::DEFAULT;
}

struct SignColor::RGB SignColor::getValue(
        DisplayType displayType) const {

    struct RGB ret;

    switch(displayType) {
    case DISPLAY_FLIPDISC:
        if(this->type == Type::BACKGROUND)
            ret = SignColor::defaultFlipDiscBG.value;
        else
            ret = SignColor::defaultFlipDiscFG.value;
        break;
    case DISPLAY_MONOCHROME_LED:
        if(this->type == Type::BACKGROUND)
            ret = SignColor::defaultMonoLEDBG.value;
        else
            ret = SignColor::defaultMonoLEDFG.value;
        break;
    case DISPLAY_RGB_LED:
        if(this->status == CUSTOM)
            ret = this->value;
        else if(this->type == Type::BACKGROUND)
            ret = SignColor::defaultRGBLEDBG.value;
        else
            ret = SignColor::defaultRGBLEDFG.value;
        break;
    }

    return ret;
}
