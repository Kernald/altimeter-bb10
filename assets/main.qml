import bb.cascades 1.0

NavigationPane {
    id: nav
    
    Menu.definition: MenuDefinition {
        settingsAction: SettingsActionItem {
            onTriggered: {
                displaySettings();
            }
        }
    }
   	Page {
	    Container {
	        layout: AbsoluteLayout {}
	        
	        ImageView {
	            imageSource: "asset:///images/background.png"
	            loadEffect: ImageViewLoadEffect.None
	            
	            verticalAlignment: VerticalAlignment.Fill
	            horizontalAlignment: HorizontalAlignment.Fill
	        }
	        
	        Container {
	            id: altitudeLine
	            objectName: "altitudeLine"
	            background: Color.create("#FFFCCA")
                minWidth: DisplayInfo.width
                minHeight: 2.0
                visible: _app.valid
                layoutProperties: AbsoluteLayoutProperties {}
            }
	        
	        Container {
	            Label {
	                text: _app.altitude
	                textStyle {
	                    base: status.style
	                    fontWeight: FontWeight.Bold
	                }
	            }
	            Label {
	                text: _app.latitude
	                visible: _app.valid
	                textStyle {
	                    base: status.style
	                }
	            }
	            Label {
	                text: _app.longitude
	                visible: _app.valid
	                textStyle {
	                    base: status.style
	                }
	            }
	            
	            attachedObjects: [
	                TextStyleDefinition {
	                    id: status
	                    color: Color.create("#FFFCCA")
	                    fontSize: FontSize.XLarge
	                }
	            ]
	        }
	    }
	}
   	
   	attachedObjects: [
   	    ComponentDefinition {
            id: settingsPageDefinition
            source: "settings.qml"
        }
   	]
   	
   	function displaySettings() {
   	    var page = settingsPageDefinition.createObject();
   	    nav.push(page);
   	}
}
