project('Voiture', 'cpp',
  default_options: ['cpp_std=c++17', 'warning_level=3'],
)

subdir('ImGui')

cxx = meson.get_compiler('cpp')

sfml_dep = [
  cxx.find_library('sfml-graphics-s', required: true),
  cxx.find_library('sfml-window-s', required: true),
  cxx.find_library('sfml-system-s', required: true),
  cxx.find_library('udev', required: true),
  dependency('gl'),
  dependency('x11'),
  dependency('xrandr'),
  dependency('threads'),
]

extra_args = []
extra_dep = []
linker_args = ['-static-libstdc++', '-static-libgcc']
static_lib = []

# disgusting hack to statically link bfd
# if only we could cxx.find_library('bfd', static:true)
# but nooooo, meson needs more than a year to merge a pull request
bfd_dep = cxx.find_library('bfd', required: true)
if bfd_dep.found()
  z_dep = cxx.find_library('z', required: true)
  liberty_dep = cxx.find_library('iberty', required: true)
  if z_dep.found() and liberty_dep.found()
	static_lib += ['-lbfd', '-lz', '-liberty']
	extra_dep += cxx.find_library('dl', required: true)
	extra_args += '-DBACKWARD_HAS_BFD=1'
  endif
endif

linker_args += ['-Wl,-Bstatic', static_lib, '-Wl,-Bdynamic']

src = [
  'Application.cpp',
  'NeuralNetwork.cpp',
  'Plot.cpp',
  'Renderer.cpp',
  'Simulator.cpp',
  'StackTrace.cpp',
  'test.cpp',
  'World.cpp'
]

executable('Voiture', src,
  dependencies: [sfml_dep, imgui_dep, extra_dep],
  include_directories: include_directories('.'),
  cpp_args: extra_args,
  link_args: linker_args
)
