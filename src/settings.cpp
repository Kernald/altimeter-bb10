#include "settings.hpp"

#include <QtCore/QSettings>

QVariant Settings::getValueFor(const QString &objectName, const QVariant &defaultValue) {
	QSettings settings;

	// If no value has been saved, return the default value
	if (settings.value(objectName).isNull()) {
		return defaultValue;
	}

	// Otherwise, return the value stored in the settings object
	return settings.value(objectName);
}

void Settings::saveValueFor(const QString &objectName, const QVariant &inputValue) {
	// A new value is saved to the application settings object
	QSettings settings;
	settings.setValue(objectName, inputValue);
}
