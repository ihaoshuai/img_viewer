project('img_viewer', 'cpp', 'c')

incl = include_directories('include')

raylib = dependency('raylib')
spdlog = dependency('spdlog')

incls = [ incl ]
deps = [ raylib, spdlog ]

source = []

subdir('src')

executable(
    'iv',
    source,
    include_directories: incls,
    dependencies: deps
)

shared_library(
    'chaos',
    'shared_lib/chaos.cpp',
    dependencies: raylib
)