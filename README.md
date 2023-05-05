# 📑 ls
As simple as listing the files in a directory.

# 📥 Installation

```bash
# clone the project with
$ git clone https://github.com/42-Ikole-Systems/ls.git --recurse-submodules

# build the project
$ cd ls
$ make

# run the executable
$ ./ft_ls -la -G
```

# 🪧 Usage
```bash
./ft_ls [-alRrtufgdG] [file(s)]
```

# ⛳ Supported flags
- `[a]` include hidden directories
- `[l]` long format
- `[R]` Recusively list subdirectories
- `[r]` reverse lexicographical sort
- `[t]` sort by time modified
- `[u]` Use time of last access, instead of last modification of the file for sorting and in long format.
- `[f]` Output is not sorted.  This option turns on the -a option.
- `[g]` display the group name in the long (-l) format output (the owner name is suppressed and long format is turned on).
- `[d]` Directories are listed as plain files (not searched recursively).
- `[G]` Gives the output ✨Beautiful colors✨ :D
