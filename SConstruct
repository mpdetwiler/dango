
AddOption(
  '--target',
  dest='target',
  type='string',
  nargs=1,
  action='store',
  metavar='DIR',
  help='build target'
);

target = GetOption('target');

if(target is None):
  target = 'linux';

if(target == 'linux'):
  print('building for \"' + target + '\"');
elif(target == 'win32'):
  print('building for \"' + target + '\"');
elif(target == 'win64'):
  print('building for \"' + target + '\"');
else:
  print('invalid target \"' + target + '\"');
  Exit(1);

import os;

env = Environment();

env.Append(ENV = {'PATH':os.environ['PATH']});

prog_name = 'run';
header_paths = [];
flags = [
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
];

if(target == 'win32'):
  env.Replace(CXX = 'i686-w64-mingw32-g++');
elif(target == 'win64'):
  env.Replace(CXX = 'x86_64-w64-mingw32-g++');

if(target == 'linux'):
  env.Append(LIBS = ['pthread']);
elif(target == 'win32' or target == 'win64'):
  env.Append(LIBS = ['winmm']);
  prog_name += '.exe';
  flags += ['-static-libgcc', '-static-libstdc++'];
  env.Append(CPPDEFINES = [('WINVER', '0x0601'), ('_WIN32_WINNT', '0x0601')]);

header_paths += ['include/'];

env.Append(CPPPATH = header_paths);
env.Append(CXXFLAGS = flags);
env.Append(LINKFLAGS = flags);

sources = [
	Glob('src/*.cpp')
];

env.Program(prog_name, sources);

