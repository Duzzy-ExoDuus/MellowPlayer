#include "StringSettingViewModel.hpp"

USING_MELLOWPLAYER_NAMESPACE(Presentation)
USING_MELLOWPLAYER_NAMESPACE(Application)

StringSettingViewModel::StringSettingViewModel(Setting& setting, QObject* parent) : SettingViewModel(setting, parent) {

}

QString StringSettingViewModel::getValue() const {
    return setting.getValue().toString();
}

void StringSettingViewModel::setValue(QString value) {
    setting.setValue(value);
}

void StringSettingViewModel::onValueChanged() {
    emit valueChanged();
}

QString StringSettingViewModel::getQmlComponent() {
    return "qrc:/MellowPlayer/Presentation/Views/MellowPlayer/Delegates/StringSettingDelegate.qml";
}