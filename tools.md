# The coolest tools around

## Generic Essentials

- **[Git](https://git-scm.com/)**: The version control system we use
  - macOS: `brew install git`
  - Linux: it's best to [install git](https://git-scm.com/download/linux) using your OS package manager
- **[Slack](https://slack.com/downloads/)**: We chat on here :yay:
  - macOS: `brew cask install slack`
  - Linux: [check the installation instructions](https://get.slack.help/hc/en-us/articles/212924728-Slack-for-Linux-beta-)
- **[Docker for Mac](https://www.docker.com/)**: We run almost everything on Docker
  - macOS: `brew cask install docker`
  - Linux: [follow the docs](https://docs.docker.com/install/linux/docker-ce/ubuntu/) for your distribution (select the distribution in the left menu)
- **[docker-compose](https://docs.docker.com/compose/install/)**: Manages Docker containers, many projects have their containers defined in docker-compose
  - macOS: `brew install docker-compose`
  - Linux: see the installation instruction, on Ubuntu you can run `sudo apt install docker-compose` if you have installed Docker from a PPA repository

### macOS Essentials

- **[Homebrew](https://brew.sh/)**: Package manager for macOS (has `git`, `docker`, etc.)
  - `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
- **[iTerm 2](https://www.iterm2.com/)**: A macOS terminal emulator with [awesome features](https://www.iterm2.com/features.html)
  - `brew cask install iterm2`

## Python Essentials

- **[Python 3](https://www.python.org/)**: Obviously :D
  - macOS: `brew install python3`
  - Linux: chances are you already have Python 3 pre-installed, if not, use your distribution's package manager, or you can always install Python from source
- **[PyCharm](https://www.jetbrains.com/pycharm/)**: Our heavily-recommended code editor for Python (of course, you can use whichever editor anyway)
  - macOS: `brew cask install pycharm`
  - Linux: you can install PyCharm with snap (on Ubuntu) - `sudo snap install [pycharm-professional|pycharm-community] --classic`, check your package manager for other distributions or [download PyCharm](https://www.jetbrains.com/pycharm/download/#section=linux) from the official site
- **[Setuptools](https://pypi.org/project/setuptools/)** & **[Pip](https://pypi.org/project/pip/)**: Two most important Python packages
  - you should already have pip and setuptools if you installed Python from source or via Homebrew (for macOS), however, it may not be available on Linux if installed using your OS package manager and needs to be installed separately
- **[pip-tools](https://github.com/jazzband/pip-tools)**: We use the `pip-compile` command from this project to lock Python dependencies, more info in [How to Manage Python Dependencies](/how-to/manage-python-dependencies)
  - `pip3 install pip-tools`
- **[tox](https://tox.readthedocs.io/en/latest/)**: Many projects use tox to easily run CI checks locally
  - `pip3 install tox`

## CLI

- **[ag (the silver searcher)](https://github.com/ggreer/the_silver_searcher)**: A code searching tool that is crazy fast
  - macOS: `brew install the_silver_searcher`
- **[tmuxinator](https://github.com/tmuxinator/tmuxinator)**: Creates/resumes terminal sessions based on yaml config files, written in Ruby
  - `gem install tmuxinator`
- **[tmuxp](https://github.com/tmux-python/tmuxp)**: The same as `tmuxinator`, but written in python - compatible config structure + per pane startup script and more
  - `pip install --user tmuxp`
- **[joe](https://github.com/karan/joe)**: The easiest way to create gitignore files
  - `pip3 install joe`
- **[cowsay](https://github.com/schacon/cowsay)**: Like echo, but with more cows
  - macOS: `brew install cowsay`
- **[fasd](https://github.com/clvv/fasd)**: Keeps track of most used files/directories for easily finding paths (type `z tra` to change into the `Trash` directory for instance)
  - macOS: `brew install fasd`
- **[jq](https://stedolan.github.io/jq/)**: Parse and query JSON, such as `.locations[].id` to get a list of location IDs from a locations API response
  - macOS: `brew install jq`
- **[HTTPie](https://httpie.org)**: A better client for making web requests (instead of curl)
  - macOS: `brew install httpie`
- **[colordiff](https://www.colordiff.org/)**: Syntax highlighting for diff output
  - macOS: `brew install colordiff`
- **[fast-cli](https://github.com/sindresorhus/fast-cli)**: Simple tool for checking bandwidth
  - `yarn global add fast-cli`
- **[git-cal](https://github.com/k4rthik/git-cal)**: git-cal is a simple script to view commits calendar (similar to github contributions calendar) on command line with vairous filter options
- **[exa](https://github.com/ogham/exa)**: `ls` with more features and better defaults written in Rust.
  - macOS: `brew install exa`
- **[fzf](https://github.com/junegunn/fzf)**: general-purpose command-line fuzzy finder.
  - macOS: `brew install fzf`
  - example usage for search in history and directories (with zsh):
    ```bash
    # fd - cd to selected directory
    fd() {
      local dir
      dir=$(find ${1:-.} -path '*/\.*' -prune \
                      -o -type d -print 2> /dev/null | fzf +m) &&
      cd "$dir"
    }
    
    # fh - search in your command history and execute selected command
    fh() {
      eval $( ([ -n "$ZSH_NAME" ] && fc -l 1 || history) | fzf +s --tac | sed 's/ *[0-9]* *//')
    }
    ```
- **[grpcurl](https://github.com/fullstorydev/grpcurl)**: `curl` but for GRPC
  - macOS: `brew install grpcurl`
- **[noti](https://github.com/variadico/noti)**: Monitor a process and trigger a notification when it finishes (good for long running tests)
  - macOS: `brew install noti`
- **[tldr](https://tldr.sh/)**: Simple and short `man` pages
  - macOS: `brew install tldr`
- **[Mackup](https://github.com/lra/mackup)**: Keep your macOS application settings (read dotfiles) in sync.
  - macOS: `brew install mackup`
  
## GUI / Web

- **[Dash](https://kapeli.com/dash)**: Documentation browser
  - macOS: `brew cask install dash`
- **[Alfred](https://www.alfredapp.com)**: macOS Spotlight alternative, customizable and powerful (does Google search, etc.)
- **[Numi](https://numi.io/)**: Calculator alternative, also does conversions such as currency or weight
  - macOS: `brew cask install numi`
- **[Spectacle](https://www.spectacleapp.com)**: Resizing/moving windows with hotkeys
- **[Monosnap](https://monosnap.com/welcome)**: Screenshot app that uploads to imgur/SFTP/etc.
- **[Join](https://joaoapps.com/join/)**: Notification syncing between your computer and your phone
- **[Unclutter](https://unclutterapp.com/)**: "A place on your desktop for storing notes, files and pasteboard clips"
- **[Bartender](https://www.macbartender.com/)**: Bartender helps you organize your macOS menu bar icons
- **[Caffeine](https://lightheadsw.com/caffeine/)**: A simple button in your menu bar that keeps your Mac from going to sleep
  - macOS: `brew cask install caffeine`
- **[keybase.io](https://keybase.io)**: Encryption made easy
  - macOS: `brew cask install keybase`
- **[Airflow](https://github.com/apache/incubator-airflow)**: A better cron daemon, with a web UI
  - `pip3 install airflow`
- **[Wakatime](https://wakatime.com/)**: Time tracking for developers, can track time spent per commit, per file, per editor, etc.
- **[reveal.js](https://revealjs.com/)**: Framework for nice presentations that you can write in HTML
  - `yarn add reveal.js`
- **[Insomnia](https://insomnia.rest/)**: API requests client you will love. ✨ (instead of Postman)
  - macOS: `brew cask install insomnia`
- **[Itsycal](https://www.mowglii.com/itsycal/)**: Tiny menu bar calendar. 📅
  - macOS: Download from https://www.mowglii.com/itsycal/ or run `curl https://itsycal.s3.amazonaws.com/Itsycal.zip -o Itsycal.zip && unzip Itsycal.zip -d ~/Applications/`

## Python libraries

- **[arrow](https://arrow.readthedocs.io/en/latest/)**: Better dates and times
  - `pip3 install arrow`
- **[addict](https://github.com/mewwts/addict)**: Easier handling of nested dictionaries
  - `pip3 install addict`
- **[tqdm](https://github.com/tqdm/tqdm)**: Awesome progress bars with one line of code
  - `pip3 install tqdm`
- **[hypothesis](https://github.com/HypothesisWorks/hypothesis-python)**: Automatically parametrizes tests to find edge cases that break
  - `pip3 install hypothesis`
- **[laboratory](https://github.com/joealcorn/laboratory)**: Deploy two versions of the same code, this library runs both and reports on the runtime differences
  - `pip3 install laboratory`
- **[boltons](https://boltons.readthedocs.io/en/latest/)**: A really useful extension of the standard library with tons of utility functions
  - `pip3 install boltons`
- **[structlog](https://structlog.readthedocs.io/en/stable/)**: A logging replacement that can keep track of context and give you JSON output to be indexed in ELK
  - `pip3 install structlog`
- **[zeep](https://python-zeep.readthedocs.io/en/master/)**: The best SOAP client, hands down. Has asyncio support too, and very active development even in 2017.

## Vim plugins

- **[pathogen](https://github.com/tpope/vim-pathogen.git)**: Makes it super easy to install plugins and runtime files in their own private directories
- **[ale](https://github.com/w0rp/ale.git)**: Plugin for providing linting in NeoVim and Vim 8
- **[ctrlp](https://github.com/ctrlpvim/ctrlp.vim.git)**: Full path fuzzy file, buffer, mru, tag, ... finder for Vim.
- **[fugitive](https://github.com/tpope/vim-fugitive.git)**: a Git wrapper so awesome, it should be illegal
- **[gundo](https://github.com/sjl/gundo.vim.git)**: Visualize your Vim undo tree.
- **[airline](https://github.com/bling/vim-airline)**: Nice status/tabline
- **[commentary](https://github.com/tpope/vim-commentary.git)**: Comment stuff out

## Chrome extensions

- **[Vimium](https://github.com/philc/vimium/blob/master/README.md)**: Vimium is a Chrome extension that provides keyboard-based navigation and control of the web in the spirit of the Vim editor
- **[JSON Editor](https://chrome.google.com/webstore/detail/json-editor/lhkmoheomjbkfloacpgllgjcamhihfaj?hl=en)**: A tool to view, edit, format, and validate JSON

## Setup

- **[@strajk's setup](https://github.com/Strajk/setup)**: Whole setup of everything ;)

## Useful aliases and `.zshrc`/`.bashrc` config

- **weather info in terminal**:
    ```bash
    weather(){
            if [ $# -eq 0 ]; then http wttr.in; else http "wttr.in/$1"; fi
    }
    ```
    Then you can use `weather` for weather in your current location or `weather brno` or any other location for weather elsewhere
