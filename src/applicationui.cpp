#include "applicationui.hpp"

#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/Application>
#include <bb/cascades/Container>
#include <bb/cascades/QmlDocument>

#include <bb/data/JsonDataAccess>

#include <bb/device/DisplayInfo>

#include <QtCore/QDebug>

#include <QtLocationSubset/QGeoPositionInfoSource>

ApplicationUI::ApplicationUI(bb::cascades::Application *app): QObject(app), _latitude(0), _longitude(0), _altitude(0), _valid(false) {
	DEFAULT_SHARE_TEXT = trUtf8("My current position is $LATITUDE$%1-$LONGITUDE$%1 - $ALTITUDE$ $ALTITUDE_UNIT$!").arg(QChar(0xB0));

	bb::cascades::QmlDocument *qml = bb::cascades::QmlDocument::create("asset:///main.qml").parent(this);
	qml->setContextProperty("_app", this);
	qml->setContextProperty("_settings", &_settings);

	bb::device::DisplayInfo display;
	const int width = display.pixelSize().width();
	const int height = display.pixelSize().height();

	QDeclarativePropertyMap* displayProperties = new QDeclarativePropertyMap;
	displayProperties->insert("width", QVariant(width));
	displayProperties->insert("height", QVariant(height));
	qml->setContextProperty("DisplayInfo", displayProperties);

	QDeclarativePropertyMap* altitudeProperties = new QDeclarativePropertyMap;
	altitudeProperties->insert("maxHeight", QVariant(MAX_ALTITUDE));
	altitudeProperties->insert("defaultShareText", QVariant(DEFAULT_SHARE_TEXT));
	altitudeProperties->insert("defaultAutoRefreshState", QVariant(DEFAULT_AUTO_REFRESH_STATE));
	altitudeProperties->insert("defaultRefreshDelay", QVariant(DEFAULT_REFRESH_DELAY));
	qml->setContextProperty("AltitudeSettings", altitudeProperties);

	bb::cascades::AbstractPane *root = qml->createRootObject<bb::cascades::AbstractPane>();
    app->setScene(root);
	_altitudeLine = root->findChild<bb::cascades::Container*>("altitudeLine");

    QGeoPositionInfoSource *src = QGeoPositionInfoSource::createDefaultSource(this);
    connect(src, SIGNAL(updateTimeout()), this, SLOT(positionUpdateTimeout()));
    connect(src, SIGNAL(positionUpdated(const QGeoPositionInfo&)), this, SLOT(positionUpdated(const QGeoPositionInfo&)));

    if (Settings::getValueFor("autoRefresh", DEFAULT_AUTO_REFRESH_STATE).toBool()) {
    	src->startUpdates();
    	src->setUpdateInterval(Settings::getValueFor("refreshDelay", DEFAULT_REFRESH_DELAY).toInt());
    } else
    	src->requestUpdate(0);
}

QString ApplicationUI::getLatitudeString() const {
	return trUtf8("Latitude: %1°").arg(_latitude);
}

QString ApplicationUI::getLongitudeString() const {
	return trUtf8("Longitude: %1°").arg(_longitude);
}

QString ApplicationUI::getAltitudeString() const {
	E_Unit tunit = static_cast<E_Unit>(Settings::getValueFor("unit", METERS).toInt());
	return _valid ? trUtf8("Altitude: %1 %2").arg(convertAltitude(_altitude, tunit)).arg(getUnitString(tunit)) : tr("Waiting for GPS...");
}

QString ApplicationUI::getUnitString(E_Unit unit) {
	switch (unit) {
	case METERS:
		return tr("meters");

	case FEET:
		return tr("feet");

	case YARDS:
		return tr("yards");

	default:
		return tr("meters");
	}
}

QByteArray ApplicationUI::formatForShare() const {
	E_Unit tunit = static_cast<E_Unit>(Settings::getValueFor("unit", METERS).toInt());
	double altitude = convertAltitude(_altitude, tunit);
	QString unit = getUnitString(tunit);
	QString result = Settings::getValueFor("shareText", DEFAULT_SHARE_TEXT).toString();
	result.replace("$LATITUDE$", QString::number(_latitude));
	result.replace("$LONGITUDE$", QString::number(_longitude));
	result.replace("$ALTITUDE$", QString::number(altitude));
	result.replace("$ALTITUDE_UNIT$", unit);
	return result.toUtf8();
}

QDateTime ApplicationUI::qdateTimeFromMsecs(int msecs) {
	return QDateTime(QDate::currentDate()).addMSecs(msecs);
}

int ApplicationUI::msecsFromQDateTime(QDateTime date) {
	return QDateTime(date.date()).msecsTo(date);
}

void ApplicationUI::positionUpdated(const QGeoPositionInfo& pos) {
	qDebug() << "Position updated";
	setLatitude(pos.coordinate().latitude());
	setLongitude(pos.coordinate().longitude());
	refreshAltitude(_latitude, _longitude);
	_valid = true;
	emit validChanged();
}

void ApplicationUI::positionUpdateTimeout() {
    qDebug() << "Position update timed out";
	_valid = false;
	emit validChanged();
}

void ApplicationUI::refreshAltitude(double latitude, double longitude) {
	QNetworkRequest request = QNetworkRequest();
	QUrl url;
    url.setScheme(QLatin1String("http"));
    url.setHost(QLatin1String("maps.googleapis.com"));
    url.setPath(QLatin1String("/maps/api/elevation/json"));
	url.addQueryItem(QLatin1String("locations"), QString("%1,%2").arg(QString::number(latitude, '.')).arg(QString::number(longitude, '.')));
	url.addQueryItem(QLatin1String("sensor"), QLatin1String("true"));
	request.setUrl(url);
	QNetworkAccessManager *networkAccessManager = new QNetworkAccessManager(this);
	connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(requestFinished(QNetworkReply*)));
	networkAccessManager->get(request);
}

void ApplicationUI::requestFinished(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
    	QByteArray buffer = reply->readAll();
    	bb::data::JsonDataAccess jda;
    	QString resBuffer = QString::fromLatin1(buffer.data(), buffer.size());
    	QVariantMap res = jda.loadFromBuffer(resBuffer).value<QVariantMap>();
    	setAltitude(res["results"].toList().first().toMap()["elevation"].toDouble());
    } else {
        qDebug() << "Network error";
    }

    reply->deleteLater();
}

void ApplicationUI::setLatitude(double latitude) {
	if (latitude != _latitude) {
		_latitude = latitude;
		emit latitudeChanged();
	}
}

void ApplicationUI::setLongitude(double longitude) {
	if (longitude != _longitude) {
		_longitude = longitude;
		emit longitudeChanged();
	}
}

void ApplicationUI::setAltitude(double altitude) {
	if (altitude != _altitude) {
		_altitude = altitude;
		emit altitudeChanged();
		bb::device::DisplayInfo display;
		const int displayHeight = display.pixelSize().height();
		bb::cascades::AbsoluteLayoutProperties* props = dynamic_cast<bb::cascades::AbsoluteLayoutProperties*>(_altitudeLine->layoutProperties());
		props->setPositionY(displayHeight - ((((altitude * MAX_ALTITUDE / _settings.getValueFor("maxHeight", QVariant(MAX_ALTITUDE)).toInt()) * displayHeight) / ALTITUDE_SUM) + ((OFFSET * displayHeight) / ALTITUDE_SUM)));
		_altitudeLine->setLayoutProperties(props);
	}
}

double ApplicationUI::convertAltitude(double altitude, E_Unit unit) {
	switch (unit) {
	case METERS:
		return altitude;

	case FEET:
		return altitude * METRIC_TO_FEET;

	case YARDS:
		return altitude * METRIC_TO_IMP;

	default:
		return altitude;
	}
}
