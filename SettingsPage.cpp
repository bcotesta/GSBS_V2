// Brandon Cotesta || 10/28/2025 | 3:00 PM
// Sahara Rahimani || 11/04/2025 | 8:00 AM
// Dashboard page for displaying user and account information

#include "SettingsPage.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QScrollArea>
#include <QtGui/QPixmap>
#include <QtCore/QDebug>
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

    auto createArrowButton = [&](const QString& text, const QString& iconPath = "") -> QPushButton* {
        QPushButton* btn = new QPushButton(containerWidget_);
        btn->setMinimumHeight(45);

        QFont btnFont("Segoe UI", 14, QFont::DemiBold);
        btn->setFont(btnFont);

        // Button base style (background, border). Text & arrow will be child labels.
        btn->setStyleSheet(
            "QPushButton {"
            "   background-color: white;"
            "   border: 1px solid #00cc00;"
            "   border-radius: 8px;"
            "   font-size: 14px;"
            "   padding: 0px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #f4fff4;"
            "}"
            "QPushButton:pressed {"
            "   background-color: #e8ffe8;"
            "}"
        );

        // Clear native text (we'll add child widgets for precise layout)
        btn->setText("");
        // Content widget inside the button
        QWidget* content = new QWidget(btn);
        content->setStyleSheet("QWidget { background-color: transparent; }");
        QHBoxLayout* contentLayout = new QHBoxLayout(content);
        contentLayout->setContentsMargins(12, 0, 12, 0);
        contentLayout->setSpacing(8);

        // Optional icon on the left: load pixmap and only add the widget when loaded successfully.
        bool addedIcon = false;
        if (!iconPath.isEmpty()) {
            QPixmap iconPixmap;
            if (iconPath.startsWith(":/")) {
                // resource path
                iconPixmap.load(iconPath);
            } else {
                // filesystem path
                iconPixmap.load(iconPath);
            }

            if (!iconPixmap.isNull()) {
                QLabel* iconLabel = new QLabel(content);
                iconLabel->setPixmap(iconPixmap.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                iconLabel->setFixedSize(24, 24);
                iconLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
                contentLayout->addWidget(iconLabel);
                contentLayout->addSpacing(8);
                addedIcon = true;
            } else {
                qDebug() << "SettingsPage: failed to load icon at" << iconPath;
            }
        }

        // Left label (the button text)
        QLabel* leftLabel = new QLabel(text, content);
        leftLabel->setFont(btnFont);
        leftLabel->setStyleSheet("QLabel { color: #00cc00; }");
        leftLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

        // Add the text after the icon (if any)
        contentLayout->addWidget(leftLabel);

        // Arrow label on the right
        QLabel* arrowLabel = new QLabel(QString::fromUtf8("\u276F"), content); // chevron
        arrowLabel->setFont(btnFont);
        arrowLabel->setStyleSheet("QLabel { color: #00cc00; }");
        arrowLabel->setAlignment(Qt::AlignVCenter | Qt::AlignRight);

        contentLayout->addStretch();
        contentLayout->addWidget(arrowLabel);

        // Place content into the button
        QVBoxLayout* btnLayoutInner = new QVBoxLayout(btn);
        btnLayoutInner->setContentsMargins(0, 0, 0, 0);
        btnLayoutInner->addWidget(content);

        return btn;
        };
    // Helper to create the Sign Out button (red background, black centered text, no arrow)
    auto createSignOutButton = [&]() -> QPushButton* {
        QPushButton* btn = new QPushButton("Sign Out", containerWidget_);
        btn->setMinimumHeight(45);

        QFont btnFont("Segoe UI", 14, QFont::DemiBold);
        btn->setFont(btnFont);

        btn->setStyleSheet(
            "QPushButton {"
            "   background-color: #C73636;"
            "   color: black;"
            "   border: 1px solid #A82F2F;"
            "   border-radius: 8px;"
            "   font-size: 14px;"
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

        // center native text (some platforms/styles may ignore text-align; ensure alignment)
        btn->setContentsMargins(0, 0, 0, 0);
        btn->setStyleSheet(btn->styleSheet() + " QPushButton { qproperty-alignment: 'AlignCenter'; }");
        return btn;
        };


    // Use a resource path (recommended) or absolute filesystem path.
    // If using a .qrc, ensure the image is listed there and you rebuilt the project.
    QPushButton* profileBtn = createArrowButton("Profile", ":/icons/userIcon.png");
    QPushButton* productsBtn = createArrowButton("Products and Services");
    QPushButton* appInfoBtn = createArrowButton("Get to Know the App");
    QPushButton* contactBtn = createArrowButton("Contact Us");
    QPushButton* privacyBtn = createArrowButton("Privacy and Legal");
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
	cout << "SettingsPage::onShow called" << endl;
}