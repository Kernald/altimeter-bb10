APP_NAME = Altimeter

CONFIG += qt warn_on cascades10

LIBS += -lQtLocationSubset -lbbdata -lbbdevice

include(config.pri)

device {
    CONFIG(debug, debug|release) {
        # Device-Debug custom configuration
    }

    CONFIG(release, debug|release) {
        # Device-Release custom configuration
    }
}

simulator {
    CONFIG(debug, debug|release) {
        # Simulator-Debug custom configuration
    }
}
