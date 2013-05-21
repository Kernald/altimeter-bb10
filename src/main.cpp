#include <bb/cascades/Application>

#include <QtCore/QLocale>
#include <QtCore/QTranslator>
#include <Qt/qdeclarativedebug.h>

#include "applicationui.hpp"

#ifdef QT_DEBUG
void standardOutput(QtMsgType /*type*/, const char* msg) {
	fprintf(stdout, "%s\n", msg);
	fflush(stdout);
}
#endif // QT_DEBUG

Q_DECL_EXPORT int main(int argc, char **argv) {
    bb::cascades::Application app(argc, argv);

#ifdef QT_DEBUG
    qInstallMsgHandler(standardOutput);
#endif // QT_DEBUG

    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString( "Altimeter_%1" ).arg( locale_string );
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator( &translator );
    }

    new ApplicationUI(&app);

    return bb::cascades::Application::exec();
}
