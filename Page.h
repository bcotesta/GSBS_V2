// Brandon Cotesta || 10/28/2025 | 11:45 AM
// I accidentally fell asleep last night, jays lost in the 18th inning.

#pragma once
//QT
#include <QtWidgets>

#include <string>
using namespace std;

class Page
{
public:
    Page(const string& title);
    virtual ~Page();
    
    // Getters
    string getTitle() const;
    QWidget* getWidget();
    const QWidget* getWidget() const;
    
    // Page visibility control
    virtual void show();
    virtual void hide();
    bool isVisible() const;
    
    // Lifecycle hooks - override in derived classes
    virtual void onLoad();      // Called when page is first loaded
    virtual void onShow();      // Called when page becomes visible
    virtual void onHide();      // Called when page is hidden
    virtual void onUnload();    // Called when page is removed
    
protected:
    // Build the page UI - must be implemented by derived classes
    virtual void buildUI() = 0;
    
    // Get the central widget to add layouts/widgets to
    QWidget* getCentralWidget();
    
    string title_;
    QWidget* widget_;          // The main widget for this page
    QVBoxLayout* mainLayout_;  // Main layout for the page
    bool visible_;
    bool loaded_;
    
private:
    void initializeWidget();
};

