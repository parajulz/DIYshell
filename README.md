# 🐚 ANUSKAsShell

Welcome to Anuska's Shell! I've built a fully functional **custom Unix shell** written in **C**, built from scratch. Supports basic command execution, directory navigation, input/output redirection, piping, background processes, and history navigation using arrow keys.

---

## 📦 Features

* ✅ Command execution (e.g. `ls`, `echo hello`)
* ✅ Built-in `cd` to change directories
* ✅ Exit with `exit`
* ✅ Arrow key history (via GNU `readline`)
* ✅ Output redirection: `echo hello > file.txt`
* ✅ Input redirection: `sort < input.txt`
* ✅ Piping: `ls | grep .c`
* ✅ Background execution: `sleep 5 &`

---

## 🧠 How It Works

* **`fork()`** is used to create a new child process
* **`execvp()`** runs external commands
* **`waitpid()`** lets the shell wait for foreground processes
* **`dup2()`** handles I/O redirection
* **`pipe()`** allows for connecting two commands
* **`readline()`** enables arrow-key history
* Built-in `cd` is handled **before forking** because it affects the parent shell

---

## 🛠️ Build Instructions

Make sure `gcc` and `readline` are installed.

### 📥 Install readline (if not already):

```bash
sudo apt install libreadline-dev   # Linux
brew install readline              # macOS
```

### 🔨 Compile the shell:

```bash
gcc main.c -o mysh -lreadline
```

### ▶️ Run it:

```bash
./mysh
```

---

## 🧪 Example Usage

```bash
ANUSKAsShell> ls
ANUSKAsShell> cd ..
ANUSKAsShell> echo "Hello" > out.txt
ANUSKAsShell> cat < out.txt
ANUSKAsShell> ls | grep txt
ANUSKAsShell> sleep 5 &
ANUSKAsShell> exit
```

---

## 📁 File Structure

```
mysh/
├── main.c        # The full shell source code
├── README.md     # Project documentation
└── Makefile      # (optional) for easy build commands
```
