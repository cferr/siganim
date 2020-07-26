// Siganim - Sign Editor and Animator
// Copyright (C) 2020 Corentin Ferry
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "cells/BlinkAnimation.h"
#include "cells/Compose.h"
#include "cells/Display.h"
#include "cells/Fill.h"
#include "cells/MarqueeAnimation.h"
#include "cells/Split.h"
#include "cells/Text.h"

#include "SiganimJSONSignParser.h"

BlinkAnimation* SiganimJSONSignParser::buildBlinkAnimation(json_object *obj) {
    json_object* subject_obj;
    SignCell* subject;
    json_object* framesOn_obj;
    unsigned int framesOn;
    json_object* framesOff_obj;
    unsigned int framesOff;

    if(!json_object_object_get_ex(obj, "subject", &subject_obj)) {
        subject = NULL;
    } else {
        subject = buildCell(subject_obj);
    }

    if(!json_object_object_get_ex(obj, "framesOn", &framesOn_obj)) {
        throw MissingPropertyException("framesOn");
    } else {
        framesOn = json_object_get_int(framesOn_obj);
    }

    if(!json_object_object_get_ex(obj, "framesOff", &framesOff_obj)) {
        throw MissingPropertyException("framesOff");
    } else {
        framesOff = json_object_get_int(framesOff_obj);
    }

    return new BlinkAnimation(subject, framesOn, framesOff);
}

Compose* SiganimJSONSignParser::buildCompose(json_object *obj) {
    json_object* background_obj;
    SignCell* background;
    json_object* foreground_obj;
    SignCell* foreground;

    if(!json_object_object_get_ex(obj, "background", &background_obj)) {
        background = NULL;
    } else {
        background = buildCell(background_obj);
    }

    if(!json_object_object_get_ex(obj, "foreground", &foreground_obj)) {
        foreground = NULL;
    } else {
        foreground = buildCell(foreground_obj);
    }

    return new Compose(background, foreground);
}

Display* SiganimJSONSignParser::buildDisplay(json_object *obj) {
    json_object* rootCell_obj;
    SignCell* rootCell;
    json_object* width_obj;
    unsigned int width;
    json_object* height_obj;
    unsigned int height;
    json_object* type_obj;
    enum Display::Type type;

    if(!json_object_object_get_ex(obj, "rootCell", &rootCell_obj)) {
        rootCell = NULL;
    } else {
        rootCell = buildCell(rootCell_obj);
    }

    if(!json_object_object_get_ex(obj, "width", &width_obj)) {
        throw MissingPropertyException("width");
    } else {
        width = json_object_get_int(width_obj);
    }

    if(!json_object_object_get_ex(obj, "height", &height_obj)) {
        throw MissingPropertyException("height");
    } else {
        height = json_object_get_int(height_obj);
    }

    if(!json_object_object_get_ex(obj, "displayType", &type_obj)) {
        throw MissingPropertyException("displayType");
    } else {
        const char* typeChar = json_object_get_string(type_obj);
        std::string typeStr = std::string(typeChar);
        if(typeStr == "flipdisc") {
            type = Display::Type::DISPLAY_FLIPDISC;
        } else if(typeStr == "monochrome_led") {
            type = Display::Type::DISPLAY_MONOCHROME_LED;
        } else if(typeStr == "rgb_led") {
            type = Display::Type::DISPLAY_RGB_LED;
        } else {
            throw BadPropertyValueException("type");
        }
    }

    return new Display(width, height, type, rootCell);
}

Fill* SiganimJSONSignParser::buildFill(json_object *obj) {
    SignColor color = SignColor::off();
    json_object* color_obj;

    if(json_object_object_get_ex(obj, "color", &color_obj)) {
        color = buildSignColor(color_obj);
    } else {
        throw MissingPropertyException("color");
    }

    return new Fill(color);
}

MarqueeAnimation* SiganimJSONSignParser::buildMarqueeAnimation(
        json_object *obj) {
    SignCell* subject;
    json_object* subject_obj;
    unsigned int durationFrames;
    json_object* durationFrames_obj;
    enum MarqueeAnimation::Direction direction;
    json_object* direction_obj;
    unsigned int space;
    json_object* space_obj;

    if(!json_object_object_get_ex(obj, "subject", &subject_obj)) {
        subject = NULL;
    } else {
        subject = buildCell(subject_obj);
    }

    if(!json_object_object_get_ex(obj, "space", &space_obj)) {
        return NULL;
    } else {
        space = json_object_get_int(space_obj);
    }

    if(!json_object_object_get_ex(obj, "durationFrames", &durationFrames_obj)) {
        throw MissingPropertyException("durationFrames");
    } else {
        durationFrames = json_object_get_int(durationFrames_obj);
    }

    if(!json_object_object_get_ex(obj, "direction", &direction_obj)) {
        throw MissingPropertyException("direction");
    } else {
        const char* directionCStr = json_object_get_string(direction_obj);
        std::string directionStr = std::string(directionCStr);
        if(directionStr == "left") {
            direction = MarqueeAnimation::Direction::LEFT;
        } else if(directionStr == "right") {
            direction = MarqueeAnimation::Direction::RIGHT;
        } else {
            throw BadPropertyValueException("direction");
        }
    }

    return new MarqueeAnimation(subject, durationFrames, direction, space);
}

Split* SiganimJSONSignParser::buildSplit(json_object *obj) {
    enum Split::SplitDirection splitDirection;
    json_object* splitDirection_obj;
    unsigned int splitPos;
    json_object* splitPos_obj;
    SignCell* topOrLeftChild;
    json_object* topOrLeftChild_obj;
    SignCell* bottomOrRightChild;
    json_object* bottomOrRightChild_obj;

    if(!json_object_object_get_ex(obj, "splitPos", &splitPos_obj)) {
        throw MissingPropertyException("splitPos");
    } else {
        splitPos = json_object_get_int(splitPos_obj);
    }

    if(!json_object_object_get_ex(obj, "splitDirection", &splitDirection_obj)) {
        throw MissingPropertyException("splitDirection");
    } else {
        const char* directionCStr = json_object_get_string(splitDirection_obj);
        std::string directionStr = std::string(directionCStr);
        if(directionStr == "horizontal") {
            splitDirection = Split::SplitDirection::SPLIT_HORIZONTAL;
        } else if(directionStr == "vertical") {
            splitDirection = Split::SplitDirection::SPLIT_VERTICAL;
        } else if(directionStr == "nw_se_diagonal") {
            splitDirection = Split::SplitDirection::SPLIT_NW_SE_DIAGONAL;
        } else if(directionStr == "sw_ne_diagonal") {
            splitDirection = Split::SplitDirection::SPLIT_SW_NE_DIAGONAL;
        } else {
            throw BadPropertyValueException("splitDirection");
        }
    }

    if(!json_object_object_get_ex(obj, "topOrLeftChild", &topOrLeftChild_obj)) {
        topOrLeftChild = NULL;
    } else {
        topOrLeftChild = buildCell(topOrLeftChild_obj);
    }

    if(!json_object_object_get_ex(obj, "bottomOrRightChild",
            &bottomOrRightChild_obj)) {
        bottomOrRightChild = NULL;
    } else {
        bottomOrRightChild = buildCell(bottomOrRightChild_obj);
    }

    return new Split(splitDirection, splitPos, topOrLeftChild,
            bottomOrRightChild);
}

Text* SiganimJSONSignParser::buildText(json_object *obj) {
    std::string fontFamily;
    json_object* fontFamily_obj;
    std::string fontStyle;
    json_object* fontStyle_obj;
    enum Text::HorizontalAlignment hAlign;
    json_object* hAlign_obj;
    enum Text::VerticalAlignment vAlign;
    json_object* vAlign_obj;
    icu::UnicodeString text;
    json_object* text_obj;
    json_object* color_obj;

    if(!json_object_object_get_ex(obj, "fontFamily", &fontFamily_obj)) {
        throw MissingPropertyException("fontFamily");
    } else {
        const char* fontFamilyCStr = json_object_get_string(fontFamily_obj);
        fontFamily = std::string(fontFamilyCStr);
    }

    if(!json_object_object_get_ex(obj, "fontStyle", &fontStyle_obj)) {
        throw MissingPropertyException("fontStyle");
    } else {
        const char* fontStyleCStr = json_object_get_string(fontStyle_obj);
        fontStyle = std::string(fontStyleCStr);
    }

    if(!json_object_object_get_ex(obj, "hAlign", &hAlign_obj)) {
        throw MissingPropertyException("hAlign");
    } else {
        const char* hAlignCStr = json_object_get_string(hAlign_obj);
        std::string hAlignStr = std::string(hAlignCStr);
        if(hAlignStr == "left") {
            hAlign = Text::HorizontalAlignment::HALIGN_LEFT;
        } else if(hAlignStr == "center") {
            hAlign = Text::HorizontalAlignment::HALIGN_CENTER;
        } else if(hAlignStr == "right") {
            hAlign = Text::HorizontalAlignment::HALIGN_RIGHT;
        } else if(hAlignStr == "justify") {
            hAlign = Text::HorizontalAlignment::HALIGN_JUSTIFY;
        } else {
            throw BadPropertyValueException("hAlign");
        }
    }

    if(!json_object_object_get_ex(obj, "vAlign", &vAlign_obj)) {
        throw MissingPropertyException("vAlign");
    } else {
        const char* vAlignCStr = json_object_get_string(vAlign_obj);
        std::string vAlignStr = std::string(vAlignCStr);
        if(vAlignStr == "top_top") {
            vAlign = Text::VerticalAlignment::VALIGN_TOP_TOP;
        } else if(vAlignStr == "top_center") {
            vAlign = Text::VerticalAlignment::VALIGN_TOP_CENTER;
        } else if(vAlignStr == "top_bottom") {
            vAlign = Text::VerticalAlignment::VALIGN_TOP_BOTTOM;
        } else if(vAlignStr == "center_top") {
            vAlign = Text::VerticalAlignment::VALIGN_CENTER_TOP;
        } else if(vAlignStr == "center_center") {
            vAlign = Text::VerticalAlignment::VALIGN_CENTER_CENTER;
        } else if(vAlignStr == "center_bottom") {
            vAlign = Text::VerticalAlignment::VALIGN_CENTER_BOTTOM;
        } else if(vAlignStr == "bottom_top") {
            vAlign = Text::VerticalAlignment::VALIGN_BOTTOM_TOP;
        } else if(vAlignStr == "bottom_center") {
            vAlign = Text::VerticalAlignment::VALIGN_BOTTOM_CENTER;
        } else if(vAlignStr == "bottom_bottom") {
            vAlign = Text::VerticalAlignment::VALIGN_BOTTOM_BOTTOM;
        } else {
            throw BadPropertyValueException("vAlign");
        }
    }

    if(!json_object_object_get_ex(obj, "text", &text_obj)) {
        throw MissingPropertyException("text");
    } else {
        const char* textCStr = json_object_get_string(text_obj);
        text = icu::UnicodeString(textCStr);
    }

    if(json_object_object_get_ex(obj, "color", &color_obj)) {
        SignColor color = buildSignColor(color_obj);
        return new Text(fontFamily, fontStyle, hAlign, vAlign, text, color);
    } else {
        return new Text(fontFamily, fontStyle, hAlign, vAlign, text);
    }

}

SignColor SiganimJSONSignParser::buildSignColor(json_object *obj) {
    enum SignColor::Semantic semantic;
    json_object* semantic_obj;
    json_object* rgba_obj;

    if(!json_object_object_get_ex(obj, "semantic", &semantic_obj)) {
        throw MissingPropertyException("semantic");
    } else {
        const char* semanticCStr = json_object_get_string(semantic_obj);
        std::string semanticStr = std::string(semanticCStr);
        if(semanticStr == "on") {
            semantic = SignColor::Semantic::ON;
        } else if(semanticStr == "off") {
            semantic = SignColor::Semantic::OFF;
        } else {
            return SignColor::off();
        }
    }

    if(json_object_object_get_ex(obj, "rgba", &rgba_obj)) {
        uint32_t rgba_val = json_object_get_int(rgba_obj);
        unsigned char r = (unsigned char)((rgba_val >> 24) & 0xFF);
        unsigned char g = (unsigned char)((rgba_val >> 16) & 0xFF);
        unsigned char b = (unsigned char)((rgba_val >> 8) & 0xFF);
        unsigned char a = (unsigned char)(rgba_val & 0xFF);
        return SignColor(semantic, r, g, b, a);
    } else {
        return SignColor(semantic);
    }

}

SignCell* SiganimJSONSignParser::buildCell(json_object *obj) {
    if(json_object_is_type(obj, json_type::json_type_object)) {
        json_object* type_obj;
        if(!json_object_object_get_ex(obj, "type", &type_obj)
                || !json_object_is_type(type_obj, json_type::json_type_string))
            throw new MissingPropertyException("type");
        const char* type = json_object_get_string(type_obj);
        std::string typeStr = std::string(type);
        if(typeStr == "BlinkAnimation") {
            return buildBlinkAnimation(obj);
        } else if(typeStr == "Compose") {
            return buildCompose(obj);
        } else if(typeStr == "Display") {
            return buildDisplay(obj);
        } else if(typeStr == "Fill") {
            return buildFill(obj);
        } else if(typeStr == "MarqueeAnimation") {
            return buildMarqueeAnimation(obj);
        } else if(typeStr == "Split") {
            return buildSplit(obj);
        } else if(typeStr == "Text") {
            return buildText(obj);
        } else {
            throw BadPropertyValueException("type");
        }
    } else return NULL;

}

Sign* SiganimJSONSignParser::parseJSON(json_object* obj) {
    if(json_object_is_type(obj, json_type::json_type_array)) {
        Sign* ret = new Sign();
        int nbDisplays = json_object_array_length(obj);
        for(int i = 0; i < nbDisplays; ++i) {
            json_object* cell_obj = json_object_array_get_idx(obj, i);
            Display* cell = buildDisplay(cell_obj);
            ret->addDisplay(cell);
        }
        return ret;
    } else throw UnsupportedPropertyTypeException("sign",
            "not array", "array");
}
