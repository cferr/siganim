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
    return strm << "Color { semantic = " <<
            ((this->semantic == Semantic::ON)?"on":"off") <<
            ", " <<
            ((this->isCustom)?"RGB()":"") << " }";
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
