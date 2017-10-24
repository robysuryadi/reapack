/* ReaPack: Package manager for REAPER
 * Copyright (C) 2015-2017  Christian Fillion
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef REAPACK_BROWSER_HPP
#define REAPACK_BROWSER_HPP

#include "dialog.hpp"

#include "package.hpp"

#include <boost/optional.hpp>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <vector>

class Index;
class ListView;
class Menu;
class Registry;
class Version;

typedef std::shared_ptr<const Index> IndexPtr;

class Browser : public Dialog {
public:
  enum Action {
    ACTION_VERSION = 80,
    ACTION_FILTERTYPE,
    ACTION_LATEST = 300,
    ACTION_LATEST_ALL,
    ACTION_REINSTALL,
    ACTION_REINSTALL_ALL,
    ACTION_UNINSTALL,
    ACTION_UNINSTALL_ALL,
    ACTION_PIN,
    ACTION_ABOUT_PKG,
    ACTION_ABOUT_REMOTE,
    ACTION_RESET_ALL,
    ACTION_COPY,
    ACTION_REFRESH,
    ACTION_MANAGE,
  };

  Browser();
  void refresh(bool stale = false);
  void setFilter(const std::string &);

protected:
  void onInit() override;
  void onCommand(int, int) override;
  void onTimer(int) override;
  bool onKeyDown(int, int) override;
  void onClose() override;

private:
  class Entry; // browser_entry.hpp

  enum View {
    AllView,
    QueuedView,
    InstalledView,
    OutOfDateView,
    ObsoleteView,
    UninstalledView,
  };

  enum LoadState {
    Init,
    Loading,
    Loaded,
    DeferredLoaded,
  };

  void onSelection();
  bool fillContextMenu(Menu &, int index);
  void populate(const std::vector<IndexPtr> &, const Registry *);
  void transferActions();
  bool match(const Entry &) const;
  void updateFilter();
  void updateAbout();
  void fillList();
  Entry *getEntry(int);
  void updateDisplayLabel();
  void displayButton();
  void actionsButton();
  void fillMenu(Menu &);
  bool isFiltered(Package::Type) const;
  bool hasAction(const Entry *) const;
  void setTarget(const int index, const Version *, bool toggle = true);
  void resetTarget(int index);
  void resetActions(int index);
  void updateAction(const int index);
  void selectionDo(const std::function<void (int)> &);
  void listDo(const std::function<void (int)> &, const std::vector<int> &);
  View currentView() const;
  void copy();
  bool confirm() const;
  bool apply();

  void installLatest(int index, bool toggle = true);
  void installLatestAll();
  void reinstall(int index, bool toggle = true);
  void installVersion(int index, size_t verIndex);
  void uninstall(int index, bool toggle = true);
  void togglePin(int index);
  void aboutRemote(int index, bool focus = true);
  void aboutPackage(int index, bool focus = true);

  LoadState m_loadState;
  int m_currentIndex;

  boost::optional<Package::Type> m_typeFilter;
  std::vector<Entry> m_entries;
  std::list<Entry *> m_actions;

  HWND m_filter;
  HWND m_view;
  HWND m_displayBtn;
  HWND m_actionsBtn;
  ListView *m_list;
  HWND m_applyBtn;
  Serializer m_serializer;
};

#endif
