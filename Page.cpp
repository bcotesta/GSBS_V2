#include "Page.h"

Page::Page(const string& title)
    : title_(title), widget_(nullptr), mainLayout_(nullptr), 
      visible_(false), loaded_(false)
{
    initializeWidget();
}

Page::~Page() {
    if (widget_) {
        delete widget_;
        widget_ = nullptr;
    }
}

void Page::initializeWidget() {
    // Create the main widget for this page
    widget_ = new QWidget();
    
    // Create main layout
    mainLayout_ = new QVBoxLayout(widget_);
    mainLayout_->setContentsMargins(0, 0, 0, 0);
    
    widget_->setLayout(mainLayout_);
    widget_->hide(); // Start hidden
}

string Page::getTitle() const {
    return title_;
}

QWidget* Page::getWidget() {
    return widget_;
}

const QWidget* Page::getWidget() const {
    return widget_;
}

QWidget* Page::getCentralWidget() {
    return widget_;
}

void Page::show() {
    if (!loaded_) {
        buildUI();      // Build UI on first show
        onLoad();       // Call load hook
        loaded_ = true;
    }
    
    if (widget_) {
        widget_->show();
        visible_ = true;
        onShow();       // Call show hook
    }
}

void Page::hide() {
    if (widget_) {
        widget_->hide();
        visible_ = false;
        onHide();       // Call hide hook
    }
}

bool Page::isVisible() const {
    return visible_;
}

// Default implementations of lifecycle hooks (can be overridden)
void Page::onLoad() {
    // Override in derived classes
}

void Page::onShow() {
    // Override in derived classes
}

void Page::onHide() {
    // Override in derived classes
}

void Page::onUnload() {
    // Override in derived classes
}
