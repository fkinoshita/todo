todo_sources = [
  'main.c',
  'todo-application.c',
  'todo-window.c',
  'todo-item.c',
]

todo_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1'),
]

todo_sources += gnome.compile_resources('todo-resources',
  'todo.gresource.xml',
  c_name: 'todo'
)

executable('todo', todo_sources,
  dependencies: todo_deps,
       install: true,
)
