// Brandon Cotesta || 10/28/2025 | 3:00 PM
// Sahara Rahimani || 11/04/2025 | 8:00 AM
// Dashboard page for displaying user and account information

#include "SettingsPage.h"                       
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <iomanip>
#include <sstream>

SettingsPage::SettingsPage()
    : Page("settings"),
    containerWidget_(nullptr),
    currentUser_(nullptr)
{
}

SettingsPage::~SettingsPage() {
    // Qt's parent-child system handles cleanup
}

void SettingsPage::setUser(User* user) {
    currentUser_ = user;
}

void SettingsPage::buildUI() {
    QWidget* centralWidget = getCentralWidget();

    // Set background color for the page
    centralWidget->setStyleSheet("QWidget { background-color: #f5f5f5; }");

    // Create a centered container
    containerWidget_ = new QWidget(centralWidget);
    containerWidget_->setFixedWidth(400);
    containerWidget_->setStyleSheet(
        "QWidget {"
        "   background-color: white;"
        "   color: black;"
        "   border-radius: 12px;"
        "}"
    );

    QVBoxLayout* containerLayout = new QVBoxLayout(containerWidget_);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(40, 35, 40, 35);

    // Title
    titleLabel_ = new QLabel("Settings", containerWidget_);
    QFont titleFont("Segoe UI", 28, QFont::Bold);
    titleLabel_->setFont(titleFont);
    titleLabel_->setAlignment(Qt::AlignCenter);
    titleLabel_->setStyleSheet("QLabel { color: #2c3e50; }");
    titleLabel_->setMinimumHeight(45);
    containerLayout->addWidget(titleLabel_);

    
    // Add container to main layout with centering
    mainLayout_->addStretch();

    // Horizontal centering
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addStretch();
    hLayout->addWidget(containerWidget_);
    hLayout->addStretch();
    mainLayout_->addLayout(hLayout);

    mainLayout_->addStretch();

    QVBoxLayout* btnLayout = new QVBoxLayout();
    btnLayout->setSpacing(12);

    // Helper to create the settings buttons with optional left icon and right chevron.
    auto createArrowButton = [&](const QString& text, const QString& iconPath = "") -> QPushButton* {
        QPushButton* btn = new QPushButton(containerWidget_);
        btn->setMinimumHeight(45);
        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

        QFont btnFont("Segoe UI", 14, QFont::DemiBold);
        btn->setFont(btnFont);

        // Add right padding in stylesheet so child content never touches the button edge
        btn->setStyleSheet(
            "QPushButton {"
            "   background-color: white;"
            "   border: 1px solid #00cc00;"
            "   border-radius: 8px;"
            "   font-size: 14px;"
            "   padding: 0px 16px 0px 16px;" // left/right padding
            "}"
            "QPushButton:hover {"
            "   background-color: #f4fff4;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #e8ffe8;"
            "}"
        );

        // Build content: optional icon (fixed) -> label (expanding) -> arrow (fixed)
        btn->setText("");
        QWidget* content = new QWidget(btn);
        content->setStyleSheet("QWidget { background-color: transparent; }");
        content->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        QHBoxLayout* contentLayout = new QHBoxLayout(content);
        // increase right margin to ensure arrow never clips
        contentLayout->setContentsMargins(8, 0, 16, 0);
        contentLayout->setSpacing(8);

        // Optional icon on the left (fixed sized)
        if (!iconPath.isEmpty()) {
            const int iconDisplaySize = 28; // tweak if needed
            QPixmap pix;
            QIcon icon(iconPath);
            pix = icon.pixmap(iconDisplaySize, iconDisplaySize);
            if (pix.isNull()) {
                pix.load(iconPath);
            }
            if (!pix.isNull()) {
                QLabel* iconLabel = new QLabel(content);
                iconLabel->setPixmap(pix.scaled(iconDisplaySize, iconDisplaySize,
                                                Qt::KeepAspectRatio, Qt::SmoothTransformation));
                iconLabel->setFixedSize(iconDisplaySize, iconDisplaySize);
                iconLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                iconLabel->setContentsMargins(0, 0, 5, 0);
                iconLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                contentLayout->addWidget(iconLabel);
            } else {
                qDebug() << "SettingsPage: failed to load icon at" << iconPath;
            }
        }

        // Text label: expanding so it doesn't push fixed widgets out
        QLabel* leftLabel = new QLabel(text, content);
        leftLabel->setFont(btnFont);
        leftLabel->setStyleSheet("QLabel { color: #00cc00; }");
        leftLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        leftLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        leftLabel->setWordWrap(false);
        leftLabel->setIndent(0);
        contentLayout->addWidget(leftLabel);

        // small stretch to keep space but avoid pushing arrow off-screen
        contentLayout->addSpacing(6);

        // Arrow on the right: fixed size so it won't be cropped
        const int arrowSize = 16;
        QLabel* arrowLabel = new QLabel(content);
        arrowLabel->setText(QString::fromUtf8("\u276F"));
        arrowLabel->setFont(btnFont);
        arrowLabel->setStyleSheet("QLabel { color: #00cc00; }");
        arrowLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        // slightly larger fixed width so glyph always visible
        arrowLabel->setFixedSize(arrowSize + 12, arrowSize + 4);
        arrowLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        contentLayout->addWidget(arrowLabel);

        // Place content into the button with zero margins
        QVBoxLayout* btnLayoutInner = new QVBoxLayout(btn);
        btnLayoutInner->setContentsMargins(0, 0, 0, 0);
        btnLayoutInner->addWidget(content);

        return btn;
    };

    // Sign out button factory (red)
    auto createSignOutButton = [&]() -> QPushButton* {
        QPushButton* btn = new QPushButton("Sign Out", containerWidget_);
        btn->setMinimumHeight(45);

        QFont btnFont("Segoe UI", 16, QFont::DemiBold);
        btn->setFont(btnFont);

        btn->setStyleSheet(
            "QPushButton {"
            "   background-color: #C73636;"
            "   color: black;"
            "   border: 1px solid #A82F2F;"
            "   border-radius: 8px;"
            "   font-size: 20px;"
            "   padding-left: 0px;"
            "   text-align: center;"
            "}"
            "QPushButton:hover {"
            "   background-color: #B12F2F;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #992222;"
            "}"
        );

        btn->setContentsMargins(0, 0, 0, 0);
        btn->setStyleSheet(btn->styleSheet() + " QPushButton { qproperty-alignment: 'AlignCenter'; }");
        return btn;
    };

    // Use the exact filenames you provided
    QString profileIconPath = "img/128x/userIcon.png";
    QString starIconPath = "img/128x/star.png";
    QString cartIconPath = "img/128x/shopping-cart.png";
    QString contactIconPath = "img/128x/contact-mail.png";
    QString infoIconPath = "img/128x/information.png";

    // Create buttons with those icons  
    QPushButton* profileBtn = createArrowButton("Profile", profileIconPath);
    QPushButton* productsBtn = createArrowButton("Products and Services", cartIconPath);
    QPushButton* appInfoBtn = createArrowButton("Get to Know the App", starIconPath);
    QPushButton* contactBtn = createArrowButton("Contact Us", contactIconPath);
    QPushButton* privacyBtn = createArrowButton("Privacy and Legal", infoIconPath);
    QPushButton* signOutBtn = createSignOutButton();

    // Add buttons to layout
    btnLayout->addWidget(profileBtn);
    btnLayout->addWidget(productsBtn);
    btnLayout->addWidget(appInfoBtn);
    btnLayout->addWidget(contactBtn);
    btnLayout->addWidget(privacyBtn);
    btnLayout->addWidget(signOutBtn);

    containerLayout->addLayout(btnLayout);

}

void SettingsPage::onShow() {
    qDebug() << "SettingsPage::onShow called";
}