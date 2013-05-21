import bb.cascades 1.0

Page {
    titleBar: TitleBar {
        title: qsTr("Settings")
        visibility: ChromeVisibility.Visible
    }
    
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
    }
}
