#include "SignColor.h"

SignColor::~SignColor() {

}

SignColor::SignColor(enum Semantic type) :
    semantic(type), isCustom(false) {

}

SignColor::SignColor(enum Semantic semantic, const unsigned char r,
        const unsigned char g, const unsigned char b, const unsigned char a) :
    semantic(semantic), isCustom(true) {
    this->value = { r, g, b, a };
}

SignColor::SignColor(const SignColor &that) {
    this->semantic = that.semantic;
    this->isCustom = that.isCustom;
    this->value = that.value;
}

enum SignColor::Semantic SignColor::getSemantic() const {
    return this->semantic;
}

bool SignColor::hasRGBValue() const {
    return this->isCustom;
}

struct SignColor::RGBA SignColor::getValue() const {
    if(this->isCustom)
        return this->value;
    else
        throw NoRGBValueException();
}

std::ostream& operator <<(std::ostream &strm, const SignColor &s) {
    return s.serialize(strm);

}

std::ostream& SignColor::serialize(std::ostream &strm) const {
    strm << "Color { semantic = " <<
            ((this->semantic == Semantic::ON)?"on":"off");
    if(this->isCustom)
        strm <<
            ", RGBA(" << this->value.r << ", "
            << this->value.g << ", "
            << this->value.b << ", "
            << this->value.a  <<  ")";

    strm << " }";

    return strm;
}

// Non-commutative OR ! This has priority over that.
SignColor SignColor::Or(const SignColor &that) const {
    // ON takes precedence.
    if(this->semantic == ON)
        return *this;
    if(that.semantic == ON)
        return that;
    // Both colors are OFF, so if we're custom, then this wins
    // otherwise, that wins
    if(this->isCustom)
        return *this;
    return that;

}

SignColor SignColor::on() {
    SignColor c(SignColor::ON);
    return c;
}

SignColor SignColor::off() {
    SignColor c(SignColor::OFF);
    return c;
}

json_object* SignColor::toJSON() const {
    json_object* ret = json_object_new_object();
    std::string semanticStr;
    switch(this->semantic) {
    case SignColor::Semantic::ON:
        semanticStr = "on";
        break;
    case SignColor::Semantic::OFF:
        semanticStr = "off";
        break;
    }
    json_object_object_add(ret, "semantic",
            json_object_new_string(semanticStr.c_str()));

    if(this->isCustom) {
        uint32_t rgba = (((uint32_t)this->value.r) << 24)
                | (((uint32_t)this->value.g) << 16)
                | (((uint32_t)this->value.b) << 8)
                | (((uint32_t)this->value.a));
        json_object_object_add(ret, "rgba",
                    json_object_new_int(rgba));
    }

    return ret;
}
