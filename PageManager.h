#pragma once

#include "Page.h"

#include <list>
#include <memory>
#include <string>

using namespace std;

class PageManager {
public:
  PageManager();
  ~PageManager();

  // Page management
  void addPage(const string &pageId, Page* page);
  void removePage(const string &pageId);

  // Page navigation
  bool openPage(const string &pageId);
  void closeCurrentPage();
  void closeAllPages();

  // Page queries
  Page *getCurrentPage() const;
  Page *getPage(const string &pageId) const;
  string getCurrentPageId() const;
  bool hasPage(const string &pageId) const;

  // Navigation history
  bool goBack();
  bool canGoBack() const;
  void clearHistory();

private:
  // Internal storage
  struct PageEntry {
    string id;
    unique_ptr<Page> page;

    PageEntry(const string &pageId, unique_ptr<Page> p)
        : id(pageId), page(std::move(p)) {}
  };

  list<PageEntry> pages_;
  string currentPageId_;
  list<string> navigationHistory_;

  // Helper methods
  PageEntry *findPageEntry(const string &pageId);
  const PageEntry *findPageEntry(const string &pageId) const;
  void pushToHistory(const string &pageId);
};

