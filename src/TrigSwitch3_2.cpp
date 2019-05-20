#include "ML_modules.hpp"


struct TrigSwitch3_2 : Module {
	enum ParamIds {
		STEP_PARAM,
		NUM_PARAMS = STEP_PARAM+8
	};

	enum InputIds {
		TRIG_INPUT,
		CV1_INPUT = TRIG_INPUT + 8,
		CV2_INPUT,
		CV3_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT = OUT1_OUTPUT + 8,
		OUT3_OUTPUT = OUT2_OUTPUT + 8,
		NUM_OUTPUTS = OUT3_OUTPUT + 8
	};
	enum LightIds {
		STEP_LIGHT,
		NUM_LIGHTS = STEP_LIGHT+8
	};

	TrigSwitch3_2() { 
		config( NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS ); 
		for(int i=0; i<8; i++) configParam(TrigSwitch3_2::STEP_PARAM + i, 0.0, 1.0, 0.0);

		onReset(); 
	};

	void process(const ProcessArgs &args) override;

	int position=0;

	enum OutMode {
		ZERO,
		LAST
	};

	OutMode outMode = ZERO;



	dsp::SchmittTrigger stepTriggers[8];

	float out1[8];
	float out2[8];
	float out3[8];

	void onReset() override {

		position = 0;
		for(int i=0; i<8; i++) {
			lights[i].value = 0.0;
			out1[i] = 0.0f;
			out2[i] = 0.0f;
			out3[i] = 0.0f;
		};
	};

	json_t *dataToJson() override {

		json_t *rootJ = json_object();

		json_object_set_new(rootJ, "outMode", json_integer(outMode));
		json_object_set_new(rootJ, "position", json_integer(position));

		return rootJ;
	};
	
	void dataFromJson(json_t *rootJ) override {

		json_t *outModeJ = json_object_get(rootJ, "outMode");
		if(outModeJ) outMode = (OutMode) json_integer_value(outModeJ);
	
		json_t *positionJ = json_object_get(rootJ, "position");
		if(positionJ) position = json_integer_value(positionJ);

	};

};


void TrigSwitch3_2::process(const ProcessArgs &args) {

	if(outMode==ZERO) { 
		for(int i=0; i<8; i++) {
			out1[i] = 0.0f;
			out2[i] = 0.0f;
			out3[i] = 0.0f;
		}
	}

	for(int i=0; i<8; i++) {
		if( stepTriggers[i].process( inputs[TRIG_INPUT+i].getNormalVoltage(0.0)) + params[STEP_PARAM+i].getValue() ) position = i;
		lights[i].value = (i==position)?1.0:0.0;
	};

	out1[position] = inputs[CV1_INPUT].getNormalVoltage(0.0);
	out2[position] = inputs[CV2_INPUT].getNormalVoltage(0.0);
	out3[position] = inputs[CV3_INPUT].getNormalVoltage(0.0);

	for(int i=0; i<8; i++) {
		outputs[OUT1_OUTPUT+i].setVoltage(out1[i]);
		outputs[OUT2_OUTPUT+i].setVoltage(out2[i]);
		outputs[OUT3_OUTPUT+i].setVoltage(out3[i]);
	}

};


struct TrigSwitch3_2OutModeItem : MenuItem {

	TrigSwitch3_2 *trigSwitch;
	TrigSwitch3_2::OutMode outMode;

	void onAction(const event::Action &e) override {
		trigSwitch->outMode = outMode;
	};


	void step() override {
		rightText = (trigSwitch->outMode == outMode)? "✔" : "";
	};

};



struct TrigSwitch3_2Widget : ModuleWidget {
	TrigSwitch3_2Widget(TrigSwitch3_2 *module);
	json_t *dataToJsonData();
	void dataFromJsonData(json_t *root) ;
	void appendContextMenu(Menu*) override;

};

TrigSwitch3_2Widget::TrigSwitch3_2Widget(TrigSwitch3_2 *module) {
	
	setModule(module);
	box.size = Vec(15*12, 380);

	{
		SvgPanel *panel = new SvgPanel();
		panel->box.size = box.size;
		panel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance,"res/TrigSwitch3_2.svg")));
		addChild(panel);
	}

	addChild(createWidget<MLScrew>(Vec(15, 0)));
	addChild(createWidget<MLScrew>(Vec(box.size.x-30, 0)));
	addChild(createWidget<MLScrew>(Vec(15, 365)));
	addChild(createWidget<MLScrew>(Vec(box.size.x-30, 365)));


	const float offset_y = 60, delta_y = 32, row1=15, row2 = row1+33, row3 = row2 + 25;

	for (int i=0; i<8; i++) {

		addInput(createInput<MLPort>(             Vec(row1, offset_y + i*delta_y), module, TrigSwitch3_2::TRIG_INPUT + i));

		addParam(createParam<ML_MediumLEDButton>(Vec(row2 , offset_y + i*delta_y +3 ), module, TrigSwitch3_2::STEP_PARAM + i));
		addChild(createLight<MLMediumLight<GreenLight>>( Vec(row2 + 4, offset_y + i*delta_y + 7), module, TrigSwitch3_2::STEP_LIGHT+i));


		addOutput(createOutput<MLPort>(             Vec(row3, offset_y + i*delta_y),    module, TrigSwitch3_2::OUT1_OUTPUT + i));
		addOutput(createOutput<MLPort>(             Vec(row3+32, offset_y + i*delta_y), module, TrigSwitch3_2::OUT2_OUTPUT + i));
		addOutput(createOutput<MLPort>(             Vec(row3+64, offset_y + i*delta_y), module, TrigSwitch3_2::OUT3_OUTPUT + i));

	}
	addInput(createInput<MLPort>(Vec(row3,    320), module, TrigSwitch3_2::CV1_INPUT));
	addInput(createInput<MLPort>(Vec(row3+32, 320), module, TrigSwitch3_2::CV2_INPUT));
	addInput(createInput<MLPort>(Vec(row3+64, 320), module, TrigSwitch3_2::CV3_INPUT));

}



void TrigSwitch3_2Widget::appendContextMenu(Menu *menu) {

	TrigSwitch3_2 *trigSwitch = dynamic_cast<TrigSwitch3_2*>(module);
	assert(trigSwitch);

	MenuLabel *modeLabel = new MenuLabel();
	modeLabel->text = "Output Mode";
	menu->addChild(modeLabel);

	TrigSwitch3_2OutModeItem *zeroItem = new TrigSwitch3_2OutModeItem();
	zeroItem->text = "Zero";	
	zeroItem->trigSwitch = trigSwitch;
	zeroItem->outMode = TrigSwitch3_2::ZERO;
	menu->addChild(zeroItem);

	TrigSwitch3_2OutModeItem *lastItem = new TrigSwitch3_2OutModeItem();
	lastItem->text = "Last";	
	lastItem->trigSwitch = trigSwitch;
	lastItem->outMode = TrigSwitch3_2::LAST;
	menu->addChild(lastItem);

};

Model *modelTrigSwitch3_2 = createModel<TrigSwitch3_2, TrigSwitch3_2Widget>("TrigSwitch3_2");
