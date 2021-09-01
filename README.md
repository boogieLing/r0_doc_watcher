# DOCS Auto Generator



```shell
git clone 
```

## build

If the distribution is not available, you can choose to build manually.

```shell
g++ "./docs_watcher.cpp" -o "./docs_watcher"
```

## init

First, you need to initialize this folder. Make sure you have a good network.

Here, you need to specify the directory for the files to be monitored.

```shell
./docs_watcher -init -path ./
```

**Be sure to select Yes when asking for "separate source files and build directories"**

```shell
> 独立的源文件和构建目录（y/n） [n]: y
```

After all the action is complete, you can see the generated HTML in the **./watcher/html_build**

## run

```shell
./docs_watcher -path ./
```

## clear

```shell
./docs_watcher -clear
```

