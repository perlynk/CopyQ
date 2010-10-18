/*
    Copyright (c) 2009, Lukas Holecek <hluk@email.cz>

    This file is part of CopyQ.

    CopyQ is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CopyQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CopyQ.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CLIPBOARDBROWSER_H
#define CLIPBOARDBROWSER_H

#include <QListView>
#include <QRegExp>
#include <QClipboard>
#include <QTimer>
#include <QSettings>
#include <QMimeData>
#include <QMap>
#include "qeditor.h"

class ClipboardModel;
class ItemDelegate;
class ClipboardMonitor;

class ClipboardBrowser : public QListView
{
    Q_OBJECT

    struct command_t {
        QRegExp re;
        QString cmd;
        QString sep;
        bool input;
        bool output;
        bool wait;
        QIcon icon;
        QString shortcut;
    };

    public:
        ClipboardBrowser(QWidget *parent = 0);
        ~ClipboardBrowser();
        void readSettings(const QString &css);
        void writeSettings();
        bool add(const QString &txt, bool ignore_empty = true);
        bool add(QMimeData *item, bool ignore_empty = true);
        void remove();
        int length() const { return model()->rowCount(); }
        QString itemText(int i = -1) const;
        QString itemText(QModelIndex ind) const;
        void sync(bool list_to_clipboard = true, QClipboard::Mode mode = QClipboard::Clipboard);
        QModelIndex index(int i) const {
            return model()->index(i,0);
        }
        void setCurrent(int row, bool cycle = false, bool selection = false);

        // if items selected: return concatenation of selected items
        // else: return text of first item
        const QString selectedText() const;

        void addPreferredCommand(const QString &name,
                                 const QString &cmd,
                                 const QString &re,
                                 const QString &sep,
                                 bool input, bool output,
                                 bool wait, QIcon icon,
                                 QString shortcut);

        void runCallback() const;

        void setMenu(QMenu *menu);

        void startMonitoring();

    private:
        int m_maxitems;
        QString m_callback;
        QStringList m_callback_args;
        QString m_editor;
        ClipboardMonitor *m_monitor;
        QTimer timer_save;
        ClipboardModel *m;
        ItemDelegate *d;

        QMenu *menu;
        QMap<QString, command_t> commands;

        const QString dataFilename() const;
        void updateClipboard();

    protected:
        void keyPressEvent(QKeyEvent *event);
        void contextMenuEvent(QContextMenuEvent *);
        void selectionChanged ( const QItemSelection & selected,
                                const QItemSelection & deselected );

    signals:
        void requestSearch(const QString &txt);
        void requestActionDialog(int row, const QString &cmd = QString(),
                                 const QString &sep = QString("\\n"),
                                 bool input = false, bool output = false,
                                 bool wait = true);
        void hideSearch();
        void escapePressed();
        void closeAllEditors();

    public slots:
        void keyEvent(QKeyEvent *event) { keyPressEvent(event); }
        void moveToClipboard(const QModelIndex &ind);
        void moveToClipboard(int i);
        void filterItems(const QString &str);
        void clearFilter() { filterItems( QString() ); }
        void itemModified(const QString &str);
        void closeEditor(QEditor *editor);
        void openEditor();
        void saveItems(int msec=0);

        void contextMenuAction(QAction *act);
        void updateMenuItems();
        void newItem();

        void checkClipboard(QClipboard::Mode mode, QMimeData *data);
};

#endif // CLIPBOARDBROWSER_H
