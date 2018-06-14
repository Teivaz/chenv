# chenv
Windows environment management utility

Copyright (c) 2018: Oleksandr Kuznietsov <github@teivaz.com>
Distributed under the terms of the GNU GPLv3 License.The full license
is in the file COPYING, distributed as part of this software.

## Roadmap

### Version 0.1
 - [x] command which
 - [x] command listpaths
 - [X] command version

### Version 0.2
 - [] transactional registry modification
 - [] saving original environment
 - [] system environment backup
 - [] user environment backup
 - [] manual environment presets
 - [] loading environment preset (command push)
 - [] unloading environment preset (command pop)

### Version 1.0
 - [] installer/uninstaller
 - [] command help
 - [] tests in the container


## Commands

### version
> `chenv version`

Prints the current version of the program.

### which
> `chenv which <file>`

Promts the path to the file that will be executed
> `chenv which a`
>> cwd/a

> `chenv which b`
>> path1/b

### listpaths
`chenv listpaths <file>`

Promts a list of files that might be executed in the order of priority. The number is the order and the letter is the part of the path responsible for path element. The meaning of letters: `c` - current working directory, `l` - local changes to the `PATH` variable, `u` - user value of the `PATH` variable, `s` - system value of the `PATH` variable

> `chenv listpaths a`
>> 0	c	cwd/a
>> 1	l	path3/a
>> 2	s	path1/a
>> 3	u	path2/a

> `chenv listpaths b`
>> 0	s path1/b
>> 1	l path4/b

#### Having:
 - System `PATH=path1`
 - User `PATH=path2`
 - Local `PATH=path3;%PATH%;path4`
 - Current directory at `cwd`

```
|---path1
|   |---a
|   \---b
|
|---path2
|   \---a
|
|---path3
|   \---a
|
|---path4
|   \---b
|
\---cwd
    \---a
```

### use
> `chenv use <preset>`

Applies preset of the saved environment setup.

### save
> `chenv save
