#include "ML_components.hpp"


#include <functional>

using namespace rack;

extern Plugin *pluginInstance;



MLSVGSwitch::MLSVGSwitch() {

    shadow = new CircularShadow();
    addChild(shadow);
    shadow->box.size = Vec();

	sw = new SvgWidget();
	addChild(sw);
}

void MLSVGSwitch::addFrame(std::shared_ptr<Svg> svg) {
	frames.push_back(svg);
	// If this is our first frame, automatically set SVG and size
	if (!sw->svg) {
		sw->setSvg(svg);
		box.size = sw->box.size;
	}
}

/*
void MLSVGSwitch::onChange(const event::Change &e) {
	assert(frames.size() > 0);
	float valueScaled = rescale(value, minValue, maxValue, 0, frames.size() - 1);
	int index = clamp((int) roundf(valueScaled), 0, (int) frames.size() - 1);
	sw->setSVG(frames[index]);
	dirty = true;
	ParamWidget::onChange(e);
}
*/



WhiteLight::WhiteLight() {
	addBaseColor(color::WHITE);
}

BlueMLKnob::BlueMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/Knob.svg")));
};

SmallBlueMLKnob::SmallBlueMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/Knob_28.svg")));
};

BlueSnapMLKnob::BlueSnapMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/Knob.svg")));
	snap = true;
};

SmallBlueSnapMLKnob::SmallBlueSnapMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/Knob_28.svg")));
	snap = true;
};


RedMLKnob::RedMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/RedKnob.svg")));
};

SmallRedMLKnob::SmallRedMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/RedKnob_28.svg")));
};

RedSnapMLKnob::RedSnapMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/RedKnob.svg")));
	snap = true;
};

SmallRedSnapMLKnob::SmallRedSnapMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/RedKnob_28.svg")));
	snap = true;
};



GreyMLKnob::GreyMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/GreyKnob.svg")));
};

SmallGreyMLKnob::SmallGreyMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/GreyKnob_28.svg")));
};


GreySnapMLKnob::GreySnapMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/GreyKnob.svg")));
	snap = true;
};

SmallGreySnapMLKnob::SmallGreySnapMLKnob() {
    setSvg(APP->window->loadSvg(asset::plugin(pluginInstance,"res/GreyKnob_28.svg")));
	snap = true;
};



MLPort::MLPort() {
	setSvg(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Jack.svg")));
};


MLButton::MLButton() {
	momentary = true;
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MLButton_0.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/MLButton_1.svg")));
	sw->wrap();
	box.size = sw->box.size;
};


MLSmallButton::MLSmallButton() {
	momentary = true;
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SmallButton_0.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/SmallButton_1.svg")));
	sw->wrap();
	box.size = sw->box.size;
};


ML_LEDButton::ML_LEDButton() {
	momentary = true;

	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LEDButton.svg")));
	sw->wrap();
	box.size = sw->box.size;

	shadow->box.size = box.size;
	shadow->blurRadius = 0.0f;
	shadow->box.pos = Vec(0, sw->box.size.y * 0.1);
	
};

ML_MediumLEDButton::ML_MediumLEDButton() {

	momentary = true;
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LEDButton_medium.svg")));
	sw->wrap();
	box.size = sw->box.size;

	shadow->box.size = box.size;
	shadow->blurRadius = 0.0f;
	shadow->box.pos = Vec(0, sw->box.size.y * 0.1);
	
};


ML_SmallLEDButton::ML_SmallLEDButton() {

	momentary = true;
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/LEDButton_small.svg")));
	sw->wrap();
	box.size = sw->box.size;

	shadow->box.size = box.size;
	shadow->blurRadius = 0.0f;
	shadow->box.pos = Vec(0, sw->box.size.y * 0.1);
	
};




ML_ResetButton::ML_ResetButton() {

	momentary = true;
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ResetButton_0.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/ResetButton_1.svg")));

	sw->wrap();
	box.size = sw->box.size;
}

MLSwitch::MLSwitch() {

	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_0.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_1.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_2.svg")));

	shadow->box.size = box.size;
	shadow->blurRadius = 0.0f;
	shadow->box.pos = Vec(0, sw->box.size.y * 0.1);

};

MLSwitch2::MLSwitch2() {
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_0.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/Switch_2.svg")));

	shadow->box.size = box.size;
	shadow->blurRadius = 0.0f;
	shadow->box.pos = Vec(0, sw->box.size.y * 0.1);
    
};

BlueMLSwitch::BlueMLSwitch() {
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BlueSwitch_0.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BlueSwitch_1.svg")));
	addFrame(APP->window->loadSvg(asset::plugin(pluginInstance, "res/BlueSwitch_2.svg")));

   	shadow->box.size = box.size;
	shadow->blurRadius = 0.0f;
	shadow->box.pos = Vec(0, sw->box.size.y * 0.1);

};
