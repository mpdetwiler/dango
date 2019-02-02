AddOption(
  '--target',
  dest='target',
  type='string',
  nargs=1,
  action='store',
  metavar='DIR',
  help='build target'
)

target = GetOption('target')

if(target == 'linux'):
  print 'building for \"' + target + '\"'
elif(target == 'win32'):
  print 'building for \"' + target + '\"'
elif(target == 'win64'):
  print 'building for \"' + target + '\"'
else:
  print 'invalid target'
  Exit(1)

import os;

env = Environment()

env.Append(ENV = {'PATH':os.environ['PATH']})

h_paths = []
prog_name = 'run'

if(target == 'win32'):
  env.Replace(CXX = 'i686-w64-mingw32-g++')
elif(target == 'win64'):
  env.Replace(CXX = 'x86_64-w64-mingw32-g++')

if(target == 'linux'):
  env.Append(LIBS=['pthread'])
elif(target == 'win32' or target == 'win64'):
  prog_name = 'run.exe'
  env.Append(CXXFLAGS = ['-static-libgcc','-static-libstdc++'])
  env.Append(LINKFLAGS = ['-static-libgcc','-static-libstdc++'])
  env.Append(CXXFLAGS = ['-D WIN32_LEAN_AND_MEAN', '-D WINVER=0x0A00', '-D _WIN32_WINNT=0x0A00'])

h_paths += ['include/']

c_flags = [
#	'-S',
#	'-Ofast',
	'-flto',
	'-O2',
	'-std=c++17',
	'-pedantic',
	'-pedantic-errors',
	'-Wall',
	'-Wextra',
	'-Wshadow',
	'-Wfatal-errors'
]

env.Append(CPPPATH = h_paths)
env.Append(CXXFLAGS = c_flags)
env.Append(LINKFLAGS = c_flags)

sources = [
	Glob('src/*.cpp')
]

env.Program(prog_name, sources)

