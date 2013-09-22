import bb.cascades 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("Settings")
        visibility: ChromeVisibility.Visible
    }
    
    ScrollView {
        scrollViewProperties.scrollMode: ScrollMode.Vertical
        Container {
            topPadding: 20
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                leftPadding: 20
                rightPadding: 20
                
                Label {
                    text: qsTr("Top altitude")
                }
                TextField {
                    id: maxHeight
                    objectName: "maxHeight"
                    inputMode: TextFieldInputMode.NumbersAndPunctuation
                    text: _settings.getValueFor(objectName, AltitudeSettings.maxHeight)
                    onTextChanged: {
                        _settings.saveValueFor(maxHeight.objectName, text);
                    }
                }
            
            } // max height
            
            Divider {}
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                leftPadding: 20
                rightPadding: 20
                
                Label {
                    text: qsTr("Unit")
                }
                DropDown {
                    id: unit
                    objectName: "unit"
                    options: [
                        Option {
                            text: qsTr("Meters")
                        },
                        Option {
                            text: qsTr("Feet")
                        },
                        Option {
                            text: qsTr("Yards")
                        }
                    ]
                    selectedIndex: _settings.getValueFor(objectName, 0)
                    onSelectedIndexChanged: {
                        _settings.saveValueFor(unit.objectName, selectedIndex);
                    }
                }
            } // unit
            
            Divider {}
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                leftPadding: 20
                rightPadding: 20
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: qsTr("Auto-refresh")
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1.0
                        }
                    }
                    ToggleButton {
                        id: autoRefresh
                        objectName: "autoRefresh"
                        checked: _settings.getValueFor(objectName, AltitudeSettings.defaultAutoRefreshState)
                        onCheckedChanged: {
                            _settings.saveValueFor(autoRefresh.objectName, checked);
                        }
                    }
                }
                DateTimePicker {
                    id: refreshDelay
                    objectName: "refreshDelay"
                    value: _app.qdateTimeFromMsecs(_settings.getValueFor(objectName, AltitudeSettings.defaultRefreshDelay))
                    mode: DateTimePickerMode.Timer
                    title: qsTr("Delay")
                    enabled: autoRefresh.checked
                    onValueChanged: {
                        if (_app.msecsFromQDateTime(value) != 0) {
                            _settings.saveValueFor(refreshDelay.objectName, _app.msecsFromQDateTime(value));
                        }
                    }
                }
            } // refresh delay
            
            Divider {}
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                leftPadding: 20
                rightPadding: 20
                
                Label {
                    text: qsTr("Share text")
                }
                Label {
                    text: qsTr("$LATITUDE$ will be replaced by your current latitude, $LONGITUDE$ by your longitude, $ALTITUDE$ by your altitude and $ALTITUDE_UNIT$ by your choosen altitude unit.")
                    multiline: true
                    textStyle.fontSize: FontSize.Small
                    textStyle.fontStyle: FontStyle.Italic
                    textStyle.fontWeight: FontWeight.W100
                }
                TextField {
                    id: shareText
                    objectName: "shareText"
                    text: _settings.getValueFor(objectName, AltitudeSettings.defaultShareText)
                    onTextChanged: {
                        _settings.saveValueFor(shareText.objectName, text);
                    }
                }
            } // share
        }
    }
}
