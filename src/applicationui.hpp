#ifndef __APPLICATION_UI_HPP__
#define __APPLICATION_UI_HPP__

namespace bb {
	namespace cascades {
		class Application;
		class Container;
	}
}

#include "settings.hpp"

#include <QtCore/QObject>
#include <QtNetwork/QNetworkReply>

#include <QtLocationSubset/QGeoPositionInfo>

using namespace QtMobilitySubset;

class ApplicationUI : public QObject {

    Q_OBJECT

    Q_PROPERTY(QString latitude READ getLatitudeString NOTIFY latitudeChanged);
    Q_PROPERTY(QString longitude READ getLongitudeString NOTIFY longitudeChanged);
    Q_PROPERTY(QString altitude READ getAltitudeString NOTIFY altitudeChanged);
    Q_PROPERTY(bool valid READ isValid NOTIFY validChanged);

public:

    enum E_Unit {
    	METERS,
    	FEET,
    	YARDS
    };

    static const double METRIC_TO_IMP = 1.0936133;
    static const double METRIC_TO_FEET = 3.28083989501312;

    ApplicationUI(bb::cascades::Application *app);
    virtual ~ApplicationUI() {}

    QString getLatitudeString() const;
    QString getLongitudeString() const;
    QString getAltitudeString() const;

    bool isValid() const;

    static const int MAX_ALTITUDE = 9000;
    static const int OFFSET = 4947;
    static const int ALTITUDE_SUM = MAX_ALTITUDE + OFFSET;

public Q_SLOTS:
    void positionUpdated(const QGeoPositionInfo& pos);
    void positionUpdateTimeout();

    void refreshAltitude(double latitude, double longitude);

	void requestFinished(QNetworkReply*);

Q_SIGNALS:
	void latitudeChanged();
	void longitudeChanged();
	void altitudeChanged();

	void validChanged();

private:
	void setLatitude(double latitude);
	void setLongitude(double longitude);
	void setAltitude(double altitude);

	static double convertAltitude(double altitude, E_Unit unit);

    double						_latitude;
    double						_longitude;
    double						_altitude;
    bool						_valid;

    Settings					_settings;

    bb::cascades::Container*	_altitudeLine;
};

inline bool ApplicationUI::isValid() const {
	return _valid;
}

#endif // __APPLICATION_UI_HPP__
