#include <QObject>
#include <QCursor>
#include <QPoint>

class NeospoorklAPI : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint globalPosition READ globalPosition NOTIFY globalPositionChanged)

public:
    explicit NeospoorklAPI(QObject *parent = nullptr) : QObject(parent) {}

    QPoint globalPosition() const {
        return QCursor::pos();
    }

signals:
    void globalPositionChanged();

public slots:
    void updateCursorPosition() {
        emit globalPositionChanged();
    }
};

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QFile>
#include <QCommandLineParser>

static const QString DEFAULT_QML_URL = QString("qrc:/default.qml");
static const QString OVERRIDE_FILE   = QString("select.url");

QUrl readOverrideUrl()
{
    QFile file(OVERRIDE_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return {};

    const QString line = QString::fromUtf8(file.readAll()).trimmed();
    return QUrl::fromUserInput(line);
}

void writeOverrideUrl(const QUrl &url)
{
    QFile file(OVERRIDE_FILE);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        file.write(url.toString().toUtf8());
        file.write("\n");
    }
}

bool copyDefaultQml(const QString &destPath = "default.qml")
{
    QFile src(DEFAULT_QML_URL);
    if (!src.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QFile dst(destPath);
    if (!dst.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    dst.write(src.readAll());
    return true;
}


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("neospoorkl");
    QApplication::setApplicationVersion("1.0");
    QQmlApplicationEngine engine;
    QCommandLineParser parser;
    QUrl qmlUrl;

    qmlRegisterType<NeospoorklAPI>("Neospoorkl", 1, 0, "NeospoorklAPI");

    parser.setApplicationDescription("mouse particle trail thing");
    parser.addHelpOption();
    parser.addVersionOption();

    const QCommandLineOption urlOption(QStringList() << "u" << "url", "source qml file link (accepts files via file:///)", "url");
    parser.addOption(urlOption);

    const QCommandLineOption overrideOption(QStringList() << "o" << "override", "override select link with url");
    parser.addOption(overrideOption);

    const QCommandLineOption writeDefaultOption(QStringList() << "d" << "destination", "write default qml to destination", "destination");
    parser.addOption(writeDefaultOption);

    parser.process(app);

    if (parser.isSet(urlOption)) {
        qmlUrl = parser.value(urlOption);
        if (parser.isSet(overrideOption))
            writeOverrideUrl(qmlUrl);
    } else {
        qmlUrl = readOverrideUrl();
        if (!qmlUrl.isValid()) {
            qmlUrl = QUrl(DEFAULT_QML_URL);
            writeOverrideUrl(qmlUrl);
        }
    }

    if (parser.isSet(writeDefaultOption)) copyDefaultQml();
    if (!qmlUrl.isValid()) {
        qCritical() << "invalid url, defaulting to " << DEFAULT_QML_URL;
        qmlUrl = QUrl(DEFAULT_QML_URL);
    }
    engine.load(qmlUrl);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}


// #include <QApplication>
// #include <QWidget>
// #include <QQuickWidget>
// #include <QQmlEngine>
// #include <QQmlContext>
// #include <QVBoxLayout>


// class Neospoorkl : public QWidget {
//     Q_OBJECT

// public:
//     Neospoorkl(QWidget *parent = nullptr) : QWidget(parent) {
//         setAttribute(Qt::WA_TranslucentBackground);
//         setAttribute(Qt::WA_OpaquePaintEvent, false);
//         setAttribute(Qt::WA_NoSystemBackground);
//         setAttribute(Qt::WA_Hover, true);
//         setAttribute(Qt::WA_TransparentForMouseEvents, true);
//         setWindowFlags(Qt::FramelessWindowHint | Qt::WindowTransparentForInput | Qt::WindowStaysOnTopHint);

//         QQuickWidget *quickWidget = new QQuickWidget(this);
//         // quickWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
//         quickWidget->setSource(QUrl(QStringLiteral("Main.qml")));

//         QVBoxLayout *layout = new QVBoxLayout(this);
//         layout->addWidget(quickWidget);
//         setLayout(layout);
//         setMouseTracking(true);
//     }
// };

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     Neospoorkl window;
//     window.show();

//     return app.exec();
// }

#include "main.moc"
