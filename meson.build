project(
  'dima',
  'c',
  version: '0.1',
  license: 'Apache-2.0',
  default_options: [
    'c_std=c11',
    'warning_level=3',
    'werror=true'
  ])

cc_flags = [
  '-Wformat-security',
  '-Wold-style-declaration',
  '-Wmissing-declarations',
]
cc = meson.get_compiler('c')
add_global_arguments(cc.get_supported_arguments(cc_flags), language: 'c')

name = 'dima'
slot = meson.project_version().split('.')[0]
slotname = name + '-'+ slot
libslotname = meson.project_name() + '-' + slot

pkg = import('pkgconfig')

cc = meson.get_compiler('c')
host_system = host_machine.system()

header_names = [
  'dima',
]
headers = []
foreach header_name : header_names
  headers += join_paths('include', name, header_name + '.h')
endforeach
install_headers(headers, subdir: join_paths(slotname, name))

incdirs = include_directories('include')

sources = []
source_names = [
  'dima',
]
foreach source_name : source_names
  sources += join_paths('src', source_name + '.c')
endforeach
lib = library(
  slotname,
  sources,
  include_directories: incdirs,
  install: true)

dima = declare_dependency(
  link_with: lib,
  include_directories: incdirs)

pkg.generate(
  lib,
  name: libslotname,
  subdirs: slotname,
  description: meson.project_name())

check = dependency('check', required: get_option('test'))
if check.found()
  test_names = [
    'dima',
  ]
  foreach name : test_names
    test_source = join_paths('tests', name + '_test.c')
    exe = executable(
      name + '_test',
      [
        'tests/test.c',
        test_source,
      ],
      dependencies: [check, dima])
    test(name, exe)
  endforeach
endif
