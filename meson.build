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
slotname = name + '-' + slot

pkg = import('pkgconfig')

attributes = ['cold', 'noreturn', 'unused']
foreach attribute : attributes
  if cc.has_function_attribute(attribute)
    value = '__attribute__((' + attribute + '))'
  else
    value = ''
  endif
  define = attribute.to_upper()
  add_global_arguments('-D' + define + '=' + value, language: 'c')
endforeach

prefix = '''
#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
'''
functions = ['reallocarray', 'strdup', 'strndup']
have_any_function = false
foreach function : functions
  if cc.has_function(function, prefix: prefix)
    have_any_function = true
    value = '1'
  else
    value = '0'
  endif
  arg = '-DHAVE_' + function.to_upper() + '=' + value
  add_global_arguments(arg, language: 'c')
endforeach
if have_any_function
  add_global_arguments('-D_GNU_SOURCE', language: 'c')
endif

install_headers(
  [
    'include/dima/derived.h',
    'include/dima/dima.h',
    'include/dima/exiting_on_failure.h',
    'include/dima/failing.h',
    'include/dima/failure_hook.h',
    'include/dima/system.h',

    'include/dima/proxy/invocation.h',
  ],
  subdir: join_paths(slotname, name))

incdirs = include_directories('include')

lib = library(
  slotname,
  [
    'src/derived.c',
    'src/dima.c',
    'src/exiting_on_failure.c',
    'src/failing.c',
    'src/failure_hook.c',
    'src/system.c',
  ],
  include_directories: incdirs,
  install: true)

dima = declare_dependency(
  link_with: lib,
  include_directories: incdirs)

url = 'https://github.com/karllinden/dima'
pkg.generate(
  lib,
  name: slotname,
  url: url,
  subdirs: slotname,
  description: 'Dependency injectable memory allocator')

proxy_slotname = name + '-proxy-' + slot
proxy_lib = library(
  proxy_slotname,
  [
    'src/proxy/invocation.c'
  ],
  include_directories: incdirs,
  install: true,
  dependencies: dima)

dima_proxy = declare_dependency(
  link_with: proxy_lib,
  include_directories: incdirs,
  dependencies: dima)

pkg.generate(
  proxy_lib,
  name: proxy_slotname,
  url: url,
  subdirs: slotname,
  requires: slotname,
  description: 'Proxy DIMA implementations')

check = dependency('check', required: get_option('test'))
if check.found()
  testlib = static_library(
    'testlib',
    [
      'tests/forwarding_tests.c',
      'tests/invocations.c',
      'tests/powz.c',
      'tests/standard_tests.c',
      'tests/test.c'
    ],
    dependencies: [check, dima])

  test_names = [
    'derived',
    'dima',
    'exiting_on_failure',
    'failure_hook',
    'powz',
    'system',
  ]
  foreach name : test_names
    test_source = join_paths('tests', name + '_test.c')
    exe = executable(
      name + '_test',
      test_source,
      dependencies: [check, dima_proxy],
      link_with: testlib)
    test(name, exe)
  endforeach
endif
