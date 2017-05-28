#ifdef USE_LIBNOTIFY

#include <catch.hpp>
#include <MellowPlayer/Entities/Notifications.hpp>
#include <MellowPlayer/Presentation/Notifications/Presenters/LibnotifyPresenter.hpp>
#include "Mocks/MainWindowMock.hpp"

USE_MELLOWPLAYER_NAMESPACE(Entities)
USE_MELLOWPLAYER_NAMESPACE(UseCases)
USE_MELLOWPLAYER_NAMESPACE(Presentation)

TEST_CASE("LibnotifyPresenterTests") {
    auto mainWindowMock = MainWindowMock::get();
    LibnotifyPresenter presenter(mainWindowMock.get());
    presenter.initialize();

    SECTION("display test") {
        Notification notif{"title", "message", "", NotificationType::NewSong};
        presenter.display(notif);
        presenter.display(notif);
    }

    SECTION("action callback show main window") {
        presenter.onActionCallback();
        Verify(Method(mainWindowMock, show)).Once();
    }
}

#endif