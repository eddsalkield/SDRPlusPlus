#include <imgui.h>
#include <module.h>
#include <path.h>
#include <watcher.h>
#include <config.h>

#define CONCAT(a, b)    ((std::string(a) + b).c_str())
#define DEEMP_LIST      "50µS\00075µS\000none\000"

MOD_INFO {
    /* Name:        */ "radio",
    /* Description: */ "Radio module for SDR++",
    /* Author:      */ "Ryzerth",
    /* Version:     */ "0.2.5"
};

class RadioModule {
public:
    RadioModule(std::string name) {
        this->name = name;
        demod = 1;
        bandWidth = 200000;
        bandWidthMin = 100000;
        bandWidthMax = 200000;
        sigPath.init(name, 200000, 1000);
        sigPath.start();
        sigPath.setDemodulator(SigPath::DEMOD_FM, bandWidth);
    }

    ~RadioModule() {
        // TODO: Implement destructor
    }

private:
    static void menuHandler(void* ctx) {
        RadioModule* _this = (RadioModule*)ctx;
        float menuColumnWidth = ImGui::GetContentRegionAvailWidth();

        ImGui::BeginGroup();

        // TODO: Change VFO ref in signal path

        ImGui::Columns(4, CONCAT("RadioModeColumns##_", _this->name), false);
        if (ImGui::RadioButton(CONCAT("NFM##_", _this->name), _this->demod == 0) && _this->demod != 0) { 
            _this->demod = 0;
            _this->bandWidth = 16000;
            _this->bandWidthMin = 8000;
            _this->bandWidthMax = 16000;
            _this->sigPath.setDemodulator(SigPath::DEMOD_NFM, _this->bandWidth);
        }
        if (ImGui::RadioButton(CONCAT("WFM##_", _this->name), _this->demod == 1) && _this->demod != 1) {
            _this->demod = 1;
            _this->bandWidth = 200000;
            _this->bandWidthMin = 100000;
            _this->bandWidthMax = 200000;
            _this->sigPath.setDemodulator(SigPath::DEMOD_FM, _this->bandWidth); 
        }
        ImGui::NextColumn();
        if (ImGui::RadioButton(CONCAT("AM##_", _this->name), _this->demod == 2) && _this->demod != 2) {
            _this->demod = 2;
            _this->bandWidth = 12500;
            _this->bandWidthMin = 6250;
            _this->bandWidthMax = 12500;
            _this->sigPath.setDemodulator(SigPath::DEMOD_AM, _this->bandWidth); 
        }
        if (ImGui::RadioButton(CONCAT("DSB##_", _this->name), _this->demod == 3) && _this->demod != 3)  {
            _this->demod = 3;
            _this->bandWidth = 6000;
            _this->bandWidthMin = 3000;
            _this->bandWidthMax = 6000;
            _this->sigPath.setDemodulator(SigPath::DEMOD_DSB, _this->bandWidth); 
        }
        ImGui::NextColumn();
        if (ImGui::RadioButton(CONCAT("USB##_", _this->name), _this->demod == 4) && _this->demod != 4) {
            _this->demod = 4;
            _this->bandWidth = 3000;
            _this->bandWidthMin = 1500;
            _this->bandWidthMax = 3000;
            _this->sigPath.setDemodulator(SigPath::DEMOD_USB, _this->bandWidth); 
        }
        if (ImGui::RadioButton(CONCAT("CW##_", _this->name), _this->demod == 5) && _this->demod != 5) { _this->demod = 5; };
        ImGui::NextColumn();
        if (ImGui::RadioButton(CONCAT("LSB##_", _this->name), _this->demod == 6) && _this->demod != 6) {
            _this->demod = 6;
            _this->bandWidth = 3000;
            _this->bandWidthMin = 1500;
            _this->bandWidthMax = 3000;
            _this->sigPath.setDemodulator(SigPath::DEMOD_LSB, _this->bandWidth);
        }
        if (ImGui::RadioButton(CONCAT("RAW##_", _this->name), _this->demod == 7) && _this->demod != 7) {
            _this->demod = 7;
            _this->bandWidth = 10000;
            _this->bandWidthMin = 3000;
            _this->bandWidthMax = 10000;
            _this->sigPath.setDemodulator(SigPath::DEMOD_RAW, _this->bandWidth);
        };
        ImGui::Columns(1, CONCAT("EndRadioModeColumns##_", _this->name), false);

        ImGui::EndGroup();

        ImGui::Text("WFM Deemphasis");
        ImGui::SameLine();
        ImGui::PushItemWidth(menuColumnWidth - ImGui::GetCursorPosX());
        if (ImGui::Combo(CONCAT("##_deemp_select_", _this->name), &_this->deemp, DEEMP_LIST)) {
            _this->sigPath.setDeemphasis(_this->deemp);
        }
        ImGui::PopItemWidth();

        ImGui::Text("Bandwidth");
        ImGui::SameLine();
        ImGui::PushItemWidth(menuColumnWidth - ImGui::GetCursorPosX());
        if (ImGui::InputInt(CONCAT("##_bw_select_", _this->name), &_this->bandWidth, 100, 1000)) {
            _this->bandWidth = std::clamp<int>(_this->bandWidth, _this->bandWidthMin, _this->bandWidthMax);
            _this->sigPath.setBandwidth(_this->bandWidth);
        }
        ImGui::PopItemWidth();
    }

    std::string name;
    int demod = 1;
    int deemp = 0;
    int bandWidth;
    int bandWidthMin;
    int bandWidthMax;
    SigPath sigPath;

};

MOD_EXPORT void _INIT_() {
   // Do your one time init here
}

MOD_EXPORT void* _CREATE_INSTANCE_(std::string name) {
    return new RadioModule(name);
}

MOD_EXPORT void _DELETE_INSTANCE_(void* instance) {
    delete (RadioModule*)instance;
}

MOD_EXPORT void _STOP_() {
    // Do your one shutdown here
}