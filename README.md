# DOCS Auto Generator

[![For](https://img.shields.io/badge/for-doc-%2355efc4)]()
[![Autor](https://img.shields.io/badge/Autor-r0-%23f0908d)]()
[![rely on](https://img.shields.io/badge/env-Ubuntu-%230984e3)]()

一个自动监控文件并生成文档的工具，利用Linux的inode通知链和能够通过注释生成文档的sphinx库实现。

A tool to automatically monitor files and generate documents, using Linux's inode notification chain and the Sphinx library that can generate documents through annotations.

```shell
git clone https://code.deeproute.ai/hanyuanling/docs_watcher
```

## build

本质上只需要 <code>docs_watcher</code>就可以完成工作。

Essentially all you need is Docs Watcher to get the job done.

但如果生成文件不可用，您可以选择手动编译。

If the distribution is not available, you can choose to build manually.

```shell
g++ "./docs_watcher.cpp" -o "./docs_watcher"
```

## init

首先，您需要初始化这个文件夹。请确保您有一个良好的网络。

First, you need to initialize this folder. Make sure you have a good network.

在这里，您需要指定目录，该目录下包含需要监控的文件**（目前只监控 .py 文件）**。

Here, you need to specify a directory that contains the files you want to monitor **(currently only .py files )** .

```shell
./docs_watcher -init -path ./
```

当要求“独立的源文件和构建目录”时，务必选择Yes。

**Be sure to select Yes when asking for "separate source files and build directories"**

```shell
> 独立的源文件和构建目录（y/n） [n]: y
```

在所有操作完成后，您可以在./watcher/html_build中看到生成的HTML

After all the action is complete, you can see the generated HTML in the **./watcher/html_build**

## run

```shell
./docs_watcher -path ./
```

## clear

```shell
./docs_watcher -clear
```

## but...

本工具强依赖于Linux的inotify机制，本质上是借用了内核的notify通知链技术。

This tool relies heavily on the **Inotify mechanism of Linux and borrows the notify chain technology in the kernel .**

所以只能在Linux环境下使用，其他系统的版本请等待后续更新。

Therefore, it can be used only in Linux. For other versions, wait for subsequent updates.

并且，所监控的文件请保证它是“可使用”的，否则sphinx不会为其生成docs。

Also, pls make sure the files you monitor are **"usable" **or Sphinx won't generate docs for them.