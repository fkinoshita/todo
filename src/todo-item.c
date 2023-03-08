/* todo-item.c
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

#include "todo-item.h"

struct _TodoItem
{
  GtkBox              parent;

  /* Template widgets */
  GtkCheckButton      *check_button;
  GtkLabel            *label;
};

G_DEFINE_FINAL_TYPE (TodoItem, todo_item, GTK_TYPE_BOX)

/* Callbacks */

static void
on_check_button_toggled (GtkCheckButton *check_button,
                         gpointer        userdata)
{
  TodoItem *todo_item;
  gboolean checked;
  PangoAttrList *attributes;
  PangoAttribute *strikethrough;

  todo_item = TODO_ITEM (userdata);
  checked = gtk_check_button_get_active (check_button);

  attributes = pango_attr_list_new();
  strikethrough = pango_attr_strikethrough_new(TRUE);
  pango_attr_list_insert(attributes, strikethrough);

  if (checked) {
    gtk_label_set_attributes (todo_item->label, attributes);
    gtk_widget_set_opacity (GTK_WIDGET (todo_item->label), 0.5);
  }

  if (!checked) {
    gtk_label_set_attributes (todo_item->label, NULL);
    gtk_widget_set_opacity (GTK_WIDGET (todo_item->label), 1.0);
  }
}

/* Overrides */

static void
todo_item_class_init (TodoItemClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/com/github/fkinoshita/todo/todo-item.ui");
  gtk_widget_class_bind_template_child (widget_class, TodoItem, check_button);
  gtk_widget_class_bind_template_child (widget_class, TodoItem, label);

  gtk_widget_class_bind_template_callback (widget_class, on_check_button_toggled);
}

static void
todo_item_init (TodoItem *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

GtkWidget *
todo_item_new (void)
{
  return g_object_new (TODO_TYPE_ITEM, NULL);
}

void
todo_item_set_text (TodoItem *todo_item,
                    const char       *text)
{
  g_return_if_fail (TODO_IS_ITEM (todo_item));

  gtk_label_set_label (todo_item->label, text);
}
