#include "PageManager.h"
#include <algorithm>
#include <iostream>
#include <string>
using namespace std;

PageManager::PageManager()
    : currentPageId_("")
{
}

PageManager::~PageManager() {
    closeAllPages();
}

void PageManager::addPage(const string& pageId, Page* page) {
    // Check if page already exists
    if (hasPage(pageId)) {
        cerr << "Page with ID '" << pageId << "' already exists!" << endl;
        delete page; // Clean up
        return;
    }

    // Wrap in unique_ptr and add to list
    pages_.emplace_back(pageId, unique_ptr<Page>(page));

    cout << "Page '" << page->getTitle() << "' (ID: " << pageId << ") added to PageManager" << endl;
}

void PageManager::removePage(const string& pageId) {
    // If trying to remove current page, close it first
    if (currentPageId_ == pageId) {
        closeCurrentPage();
    }
    
    // Remove from list
    pages_.remove_if([&pageId](const PageEntry& entry) {
        return entry.id == pageId;
    });
    
    cout << "Page with ID '" << pageId << "' removed from PageManager" << endl;
}

bool PageManager::openPage(const string& pageId) {
    // Find the page
    PageEntry* entry = findPageEntry(pageId);
    if (!entry) {
        cerr << "Page with ID '" << pageId << "' not found!" << endl;
        return false;
    }
    
    // Close current page if there is one
    closeCurrentPage();
    
    // Open the new page
    entry->page->show();
    
    // Update current page
    string previousPageId = currentPageId_;
    currentPageId_ = pageId;
    
    // Add to navigation history
    if (!previousPageId.empty() && previousPageId != pageId) {
        pushToHistory(previousPageId);
    }
    
    cout << "Opened page: " << entry->page->getTitle() << " (ID: " << pageId << ")" << endl;
    return true;
}

void PageManager::closeCurrentPage() {
    if (currentPageId_.empty()) {
        return; // No page is currently open
    }
    
    PageEntry* entry = findPageEntry(currentPageId_);
    if (entry) {
        entry->page->hide();
        cout << "Closed page: " << entry->page->getTitle() << endl;
    }
    
    currentPageId_ = "";
}

void PageManager::closeAllPages() {
    for (auto& entry : pages_) {
        if (entry.page) {
            entry.page->hide();
        }
    }
    currentPageId_ = "";
    navigationHistory_.clear();
    
    cout << "All pages closed" << endl;
}

Page* PageManager::getCurrentPage() const {
    if (currentPageId_.empty()) {
        return nullptr;
    }
    
    const PageEntry* entry = findPageEntry(currentPageId_);
    return entry ? entry->page.get() : nullptr;
}

Page* PageManager::getPage(const string& pageId) const {
    const PageEntry* entry = findPageEntry(pageId);
    return entry ? entry->page.get() : nullptr;
}

string PageManager::getCurrentPageId() const {
    return currentPageId_;
}

bool PageManager::hasPage(const string& pageId) const {
    return findPageEntry(pageId) != nullptr;
}

bool PageManager::goBack() {
    if (navigationHistory_.empty()) {
        return false;
    }
    
    // Get previous page
    string previousPageId = navigationHistory_.back();
    navigationHistory_.pop_back();
    
    // Open it (without adding to history again)
    PageEntry* entry = findPageEntry(previousPageId);
    if (!entry) {
        return false;
    }
    
    // Close current page
    if (!currentPageId_.empty()) {
        PageEntry* current = findPageEntry(currentPageId_);
        if (current) {
            current->page->hide();
        }
    }
    
    // Open previous page
    entry->page->show();
    currentPageId_ = previousPageId;
    
    cout << "Navigated back to: " << entry->page->getTitle() << endl;
    return true;
}

bool PageManager::canGoBack() const {
    return !navigationHistory_.empty();
}

void PageManager::clearHistory() {
    navigationHistory_.clear();
}

PageManager::PageEntry* PageManager::findPageEntry(const string& pageId) {
    for (auto& entry : pages_) {
        if (entry.id == pageId) {
            return &entry;
        }
    }
    return nullptr;
}

const PageManager::PageEntry* PageManager::findPageEntry(const string& pageId) const {
    for (const auto& entry : pages_) {
        if (entry.id == pageId) {
            return &entry;
        }
    }
    return nullptr;
}

void PageManager::pushToHistory(const string& pageId) {
    // Limit history size (optional)
    const size_t MAX_HISTORY_SIZE = 50;
    
    if (navigationHistory_.size() >= MAX_HISTORY_SIZE) {
        navigationHistory_.pop_front();
    }
    
    navigationHistory_.push_back(pageId);
}
