#include "ML_modules.hpp"


struct OctaTimes : Module {
	enum ParamIds {
		MULT_PARAM,
		NUM_PARAMS
	};
	enum InputIds {
		IN1_INPUT,
		IN2_INPUT,
		IN3_INPUT,
		IN4_INPUT,
		IN5_INPUT,
		IN6_INPUT,
		IN7_INPUT,
		IN8_INPUT,
		IN_B_1_INPUT,
		IN_B_2_INPUT,
		IN_B_3_INPUT,
		IN_B_4_INPUT,
		IN_B_5_INPUT,
		IN_B_6_INPUT,
		IN_B_7_INPUT,
		IN_B_8_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		OUT1_OUTPUT,
		OUT2_OUTPUT,
		OUT3_OUTPUT,
		OUT4_OUTPUT,
		OUT5_OUTPUT,
		OUT6_OUTPUT,
		OUT7_OUTPUT,
		OUT8_OUTPUT,
		SUM_OUTPUT,
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	dsp::SchmittTrigger trigger[8];
	float out[8];



	OctaTimes() {
		config( NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS ) ;
	    configParam(OctaTimes::MULT_PARAM , 0.0, 1.0, 0.0);
		onReset();
	};

	void process(const ProcessArgs &args) override;

	void onReset() override {
		for(int i=0; i<8; i++) out[i] = 0.0; 
	};

};



void OctaTimes::process(const ProcessArgs &args) {

	float in_A[8], in_B[8];

	float normal = params[MULT_PARAM].getValue()==1?1.0f:10.f;


	float multiplier = params[MULT_PARAM].getValue()==1?1.f:0.1f;

	in_A[0] = inputs[IN1_INPUT].getNormalVoltage(0.f);
//	for(int i=1; i<8; i++) in_A[i] = inputs[IN1_INPUT+i].normalize(in_A[i-1]);
	for(int i=1; i<8; i++) in_A[i] = inputs[IN1_INPUT+i].getNormalVoltage(0.f);

	in_B[0] = inputs[IN_B_1_INPUT].getNormalVoltage(normal);
	for(int i=1; i<8; i++) in_B[i] = inputs[IN_B_1_INPUT+i].getNormalVoltage(in_B[i-1]);

	float tmp;
	float sum = 0.0f;
	for(int i=0; i<8; i++) {
		tmp = clamp(in_A[i] * in_B[i] * multiplier , -12.f, 12.f);
		outputs[OUT1_OUTPUT+i].setVoltage(tmp);
		sum += tmp;
	};

	outputs[SUM_OUTPUT].setVoltage(clamp(sum,-12.f,12.f));
};



struct OctaTimesWidget : ModuleWidget {
	OctaTimesWidget(OctaTimes *module);
};

OctaTimesWidget::OctaTimesWidget(OctaTimes *module) {
		setModule(module);

	box.size = Vec(15*8, 380);

	{
		SvgPanel *panel = new SvgPanel();
		panel->box.size = box.size;
		panel->setBackground(APP->window->loadSvg(asset::plugin(pluginInstance,"res/OctaTimes.svg")));

		addChild(panel);
	}

	addChild(createWidget<MLScrew>(Vec(15, 0)));
	addChild(createWidget<MLScrew>(Vec(box.size.x-30, 0)));
	addChild(createWidget<MLScrew>(Vec(15, 365)));
	addChild(createWidget<MLScrew>(Vec(box.size.x-30, 365)));




	const float offset_y = 60, delta_y = 32, row1=15, row2 = 48, row3 = 80;

	for( int i=0; i<8; i++) {
		addInput(createInput<MLPort>(Vec(row1, offset_y + i*delta_y  ), module, OctaTimes::IN1_INPUT+i));
		addInput(createInput<MLPort>(Vec(row2, offset_y + i*delta_y  ), module, OctaTimes::IN_B_1_INPUT+i));
		addOutput(createOutput<MLPort>(Vec(row3, offset_y + i*delta_y ), module, OctaTimes::OUT1_OUTPUT+i));
	};
	
	addOutput(createOutput<MLPort>(Vec(row3, 330 ), module, OctaTimes::SUM_OUTPUT));
	addParam(createParam<CKSS>( Vec(row1 + 5, 330 ), module, OctaTimes::MULT_PARAM ));
}

Model *modelOctaTimes = createModel<OctaTimes, OctaTimesWidget>("OctaTimes");
