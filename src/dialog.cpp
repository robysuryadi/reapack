/* ReaPack: Package manager for REAPER
 * Copyright (C) 2015-2016  Christian Fillion
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

#include "dialog.hpp"

DialogMap Dialog::s_instances;

WDL_DLGRET Dialog::Proc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
  Dialog *dlg = nullptr;

  const auto it = Dialog::s_instances.find(handle);
  if(it != Dialog::s_instances.end())
    dlg = it->second;

  switch(msg) {
  case WM_INITDIALOG:
    dlg = (Dialog *)lParam;
    dlg->m_handle = handle;

    // necessary if something in onInit triggers another event
    // otherwhise dlg would be null then
    Dialog::s_instances[handle] = dlg;

    dlg->onInit();
    break;
  case WM_SHOWWINDOW:
    if(wParam) {
      // this makes possible to call onHide when destroying the window
      // but only if was visible before the destruction request
      // (destruction might be caused by the application exiting,
      // in which case IsWindowVisible would be false but m_isVisible == true)
      dlg->m_isVisible = true;
      dlg->onShow();
    }
    else {
      dlg->m_isVisible = false;
      dlg->onHide();
    }
    break;
  case WM_TIMER:
    dlg->onTimer();
    break;
  case WM_COMMAND:
    dlg->onCommand(wParam, lParam);
    break;
  case WM_NOTIFY:
    dlg->onNotify((LPNMHDR)lParam, lParam);
    break;
  case WM_CONTEXTMENU:
    dlg->onContextMenu((HWND)wParam, lParam);
    break;
  case WM_DESTROY:
    dlg->onDestroy();
    break;
  };

  return false;
}

Dialog::Dialog(const int templateId)
  : m_template(templateId), m_isVisible(false),
    m_parent(nullptr), m_handle(nullptr)
{
  // can't call reimplemented virtual methods here during object construction
}

Dialog::~Dialog()
{
  DestroyWindow(m_handle);
  s_instances.erase(m_handle);
}

INT_PTR Dialog::init(REAPER_PLUGIN_HINSTANCE inst, HWND parent, Modality mode)
{
  m_parent = parent;

  switch(mode) {
  case Modeless:
    CreateDialogParam(inst, MAKEINTRESOURCE(m_template),
      m_parent, Proc, (LPARAM)this);
    return true;
  case Modal:
    return DialogBoxParam(inst, MAKEINTRESOURCE(m_template),
      m_parent, Proc, (LPARAM)this);
  }

  return false; // makes MSVC happy.
}

void Dialog::Destroy(Dialog *dlg)
{
  if(dlg->isVisible())
    dlg->onHide();

  delete dlg;
}

void Dialog::show()
{
  center();

  ShowWindow(m_handle, SW_SHOW);
}

void Dialog::hide()
{
  ShowWindow(m_handle, SW_HIDE);
}

void Dialog::close(const INT_PTR result)
{
  EndDialog(m_handle, result);
}

void Dialog::center()
{
  RECT dialogRect, parentRect;

  GetWindowRect(m_handle, &dialogRect);
  GetWindowRect(m_parent, &parentRect);

  int left = parentRect.right / 2;
  left -= (dialogRect.right - dialogRect.left) / 2;

  int top = parentRect.bottom / 2;
  top -= (dialogRect.bottom - dialogRect.top) / 2;

  SetWindowPos(m_handle, HWND_TOP, left, top, 0, 0, SWP_NOSIZE);
}

void Dialog::setFocus()
{
  SetFocus(m_handle);
}

void Dialog::enable(HWND handle)
{
  setEnabled(true, handle);
}

void Dialog::disable(HWND handle)
{
  setEnabled(false, handle);
}

void Dialog::setEnabled(const bool enabled, HWND handle)
{
  EnableWindow(handle ? handle : m_handle, enabled);
}

HWND Dialog::getItem(const int idc)
{
  return GetDlgItem(m_handle, idc);
}

void Dialog::onShow()
{
}

void Dialog::onHide()
{
}

void Dialog::onTimer()
{
}

void Dialog::onCommand(WPARAM, LPARAM)
{
}

void Dialog::onNotify(LPNMHDR, LPARAM)
{
}

void Dialog::onContextMenu(HWND, LPARAM)
{
}

void Dialog::onDestroy()
{
}
