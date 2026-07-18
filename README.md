# meta-licheepizero-voicecommand

Yocto meta-layer for **LicheePi Zero Dock** with voice command processing,
SQL database connectivity, and Qt application support.

---

## 📦 Prerequisites

```bash
sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib \
  build-essential chrpath socat libsdl1.2-dev xterm libmpc-dev libgmp3-dev
```

## ⚙️ Setup

Run these commands from your home directory:

```bash
mkdir yocto
cd yocto
mkdir build
git clone https://git.yoctoproject.org/poky -b kirkstone
git clone https://github.com/alirezarahatloo/meta-licheepizero-voicecommand.git
cd poky
git clone https://github.com/openembedded/meta-openembedded.git -b kirkstone
git clone https://github.com/meta-qt5/meta-qt5.git --depth 1 -b kirkstone
git clone https://github.com/voloviq/meta-licheepizero --depth 1 -b kirkstone
source oe-init-build-env ~/yocto/build-licheepizero
```

## 📁 Layer Contents

| Directory | Description |
|-----------|-------------|
| `recipes-apps/voicecommand/` | C++ MFCC-based voice command |
| `recipes-apps/sqlclient/` | FreeTDS SQL Server client |
| `recipes-myqtapp/myqtapp/` | Qt5 ODBC database app |
| `recipes-kernel/linux/` | Mic & UART2 DTS patches |
| `recipes-core/init-ifupdown/` | Network interfaces config |
| `recipes-database/freetds/` | FreeTDS 1.3.10 recipe |
| `recipes-example/example/` | Bitbake example recipe |

## 🚀 Build

Add the layer and build:

```bash
bitbake-layers add-layer meta-licheepizero-voicecommand
bitbake core-image-minimal
```

---

_Maintainer: [alirezarahatloo](https://github.com/alirezarahatloo)_
