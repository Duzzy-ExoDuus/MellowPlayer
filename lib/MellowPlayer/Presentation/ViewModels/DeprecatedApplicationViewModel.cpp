#include "DeprecatedApplicationViewModel.hpp"
#include "MellowPlayer/Presentation/IconProvider.hpp"
#include <MellowPlayer/Domain/ListeningHistory/ListeningHistoryEntry.hpp>
#include <MellowPlayer/Domain/Logging/LoggingManager.hpp>
#include <MellowPlayer/Domain/Player/Player.hpp>
#include <MellowPlayer/Domain/StreamingServices/StreamingService.hpp>
#include <MellowPlayer/Presentation/ViewModels/Settings/Types/SettingViewModel.hpp>
#include <QWebEngineProfile>
#include <QtWebEngine>
#include <MellowPlayer/Domain/BuildConfig.hpp>

using namespace MellowPlayer::Domain;
using namespace MellowPlayer::Domain;
using namespace MellowPlayer::Presentation;

DeprecatedApplicationViewModel::DeprecatedApplicationViewModel(int& argc, char** argv, const QString& appName) : qtApp_(argc, argv)
{
    qtApp_.setApplicationDisplayName("MellowPlayer");
    qtApp_.setApplicationName(appName);
    qtApp_.setApplicationVersion(BuildConfig::getVersion());
    qtApp_.setOrganizationDomain("org.mellowplayer");
    qtApp_.setOrganizationName("MellowPlayer");
    qtApp_.setWindowIcon(IconProvider::windowIcon());

    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Black.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Italic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Light.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-MediumItalic.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/fonts/Roboto/Roboto-Thin.ttf");
    QFontDatabase::addApplicationFont(":/MellowPlayer/Presentation/Resources/"
                                      "fonts/Roboto/Roboto-ThinItalic.ttf");

    qRegisterMetaType<Song*>("Domain::Song*");
    qRegisterMetaType<Song*>("Song*");
    qRegisterMetaType<StreamingService*>("Domain::StreamingService*");
    qRegisterMetaType<StreamingService*>("StreamingService*");
    qRegisterMetaType<ListeningHistoryEntry>("Domain::ListeningHistoryEntry");
    qRegisterMetaType<ListeningHistoryEntry>("ListeningHistoryEntry");
    qRegisterMetaType<Player*>("Domain::Player*");
    qRegisterMetaType<Player*>("Player*");
    qRegisterMetaType<SettingViewModel*>("Presentation::SettingViewModel*");
    qRegisterMetaType<SettingViewModel*>("SettingViewModel*");

    connect(&qtApp_, &QApplication::aboutToQuit, this, &DeprecatedApplicationViewModel::onAboutToQuit);
    connect(&qtApp_, &QApplication::commitDataRequest, this, &DeprecatedApplicationViewModel::forceQuitRequested);
}

void DeprecatedApplicationViewModel::initialize()
{
    if (!translator_.load(QLocale(), "MellowPlayer", "_", ":/MellowPlayer/Translations")) {
        qWarning() << "failed to load translation: " << QLocale::system().name();
        qInfo() << "available translations: ";
        QDirIterator it(":/MellowPlayer/Translations", QStringList() << "*.qm", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
            qInfo() << "  - " << it.next();
    } else
        qInfo() << "translation successfully loaded: " << QLocale::system().name();

    qtApp_.installTranslator(&translator_);
}

int DeprecatedApplicationViewModel::run()
{
    return qtApp_.exec();
}

void DeprecatedApplicationViewModel::clearCache() const
{
    QWebEngineProfile profile("Default");
    profile.clearHttpCache();
    QDir cacheDir(QStandardPaths::standardLocations(QStandardPaths::CacheLocation)[0]);
    qDebug() << "removing cache directory: " << cacheDir;
    cacheDir.removeRecursively();
}

void DeprecatedApplicationViewModel::clearCookies() const
{
    QWebEngineProfile profile("Default");
    QDir storageDir(profile.persistentStoragePath());
    qDebug() << "removing persistent storage directory: " << storageDir;
    storageDir.removeRecursively();
}

void DeprecatedApplicationViewModel::restart()
{
    restartRequested_ = true;
    qApp->quit();
}

void DeprecatedApplicationViewModel::requestQuit()
{
    emit quitRequested();
}

void DeprecatedApplicationViewModel::quit()
{
    qtApp_.exit(0);
}

void DeprecatedApplicationViewModel::setAutoQuitDelay(int delay)
{
    if (delay > 0)
        QTimer::singleShot(delay, qtApp_.quit);
}

void DeprecatedApplicationViewModel::onAboutToQuit()
{
    clearCache();
}

static QString compilerString()
{
#if defined(Q_CC_CLANG) // must be before GNU, because clang claims to be GNU too
    QString isAppleString;
#if defined(__apple_build_version__) // Apple clang has other version numbers
    isAppleString = QLatin1String(" (Apple)");
#endif
    return QLatin1String("Clang ") + QString::number(__clang_major__) + QLatin1Char('.') + QString::number(__clang_minor__) + isAppleString;
#elif defined(Q_CC_GNU)
    return QLatin1String("GCC ") + QLatin1String(__VERSION__);
#elif defined(Q_CC_MSVC)
    if (_MSC_VER >= 1800) // 1800: MSVC 2013 (yearly release cycle)
        return QLatin1String("MSVC ") + QString::number(2008 + ((_MSC_VER / 100) - 13));
    if (_MSC_VER >= 1500) // 1500: MSVC 2008, 1600: MSVC 2010, ... (2-year release cycle)
        return QLatin1String("MSVC ") + QString::number(2008 + 2 * ((_MSC_VER / 100) - 15));
#else
    return QObject::tr("<unknown compiler>");
#endif
}

QString DeprecatedApplicationViewModel::buildInfo() const
{
    return AppStrings().builtOnStr().arg(QString(__DATE__), QString(__TIME__), compilerString(), QString::number(QSysInfo::WordSize),
                                         QString(QT_VERSION_STR));
}

bool DeprecatedApplicationViewModel::restartRequested() const
{
    return restartRequested_;
}