/* todo-window.c
 *
 * Copyright 2023 Felipe Kinoshita
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <string.h>

#include "config.h"
#include "todo-window.h"
#include "todo-item.h"

struct _TodoWindow
{
  AdwApplicationWindow  parent;

  /* Template widgets */
  GtkListBox          *list;
  GtkEntry            *entry;

  /* Variables */
  GListModel          *model;
};

G_DEFINE_FINAL_TYPE (TodoWindow, todo_window, ADW_TYPE_APPLICATION_WINDOW)

/* Callbacks */

static void
on_entry_activate (GtkEntry *entry,
                   gpointer  user_data)
{
  TodoWindow *window;
  TodoItem *todo_item;

  GtkEntryBuffer *buffer;
  const char *text;

  window = TODO_WINDOW (user_data);

  // Get text from GtkEntryBuffer
  buffer = gtk_entry_get_buffer (entry);
  text = gtk_entry_buffer_get_text (buffer);

  if (!text || strlen(text) == 0)
    return;

  // Add item to list
  todo_item = TODO_ITEM (todo_item_new ());
  todo_item_set_text (todo_item, text);
  gtk_list_box_append (window->list, GTK_WIDGET (todo_item));

  // Clear GtkEntryBuffer
  gtk_entry_buffer_delete_text (buffer, 0, -1);
}

/* Overrides */

static void
todo_window_class_init (TodoWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/github/fkinoshita/todo/todo-window.ui");
  gtk_widget_class_bind_template_child (widget_class, TodoWindow, list);
  gtk_widget_class_bind_template_child (widget_class, TodoWindow, entry);

  gtk_widget_class_bind_template_callback (widget_class, on_entry_activate);
}

static void
todo_window_init (TodoWindow *self)
{
  AdwStatusPage *status_page;

  gtk_widget_init_template (GTK_WIDGET (self));

  // Set window minimum size
  gtk_widget_set_size_request (GTK_WIDGET (self), 300, 300);

  // Add placeholder page
  status_page = ADW_STATUS_PAGE (adw_status_page_new ());
  adw_status_page_set_title (status_page, "Add new tasks below");
  adw_status_page_set_icon_name (status_page, "view-list-bullet-symbolic");
  gtk_list_box_set_placeholder (self->list, GTK_WIDGET (status_page));

  // Add model
  gtk_list_box_bind_model (self->list, self->model, NULL, NULL, NULL);

  gtk_widget_grab_focus (GTK_WIDGET (self->entry));
}
