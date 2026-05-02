# Maintainer: APKTool Dev
pkgname=apktool-toolbox
pkgver=1.0.0
pkgrel=1
pkgdesc="APK全能工具箱 - 解包/打包/签名/查壳/刷机包签名/ADB设备管理"
arch=('x86_64')
url="https://github.com/maoyaotang12/apktool-toolbox"
license='GPL3'
depends=(
    qt6-base
    qt6-widgets
    android-tools
    aapt
    unzip
    java-runtime-common
    android-signapk
    android-apktool
)
makedepends=(
    gcc
    make
    qt6-tools
)
source=(git+$url.git
    apktool-toolbox.png
    apktool-toolbox.desktop
)
sha256sums=(
    'SKIP'
    'SKIP'
    'SKIP'
    'SKIP'
    'SKIP'
    'SKIP'
)

build() {
    cd "$srcdir/$pkgname"
    qmake6 APKTool.pro
    make -j$(nproc)
}

package() {
    install -Dm755 $srcdir/$pkgname/APKTool "$pkgdir/usr/bin/apktool-toolbox"

    install -Dm644 $srcdir/$pkgname/apktool-toolbox.png "$pkgdir/usr/share/icons/hicolor/256x256/apps/apktool-toolbox.png"

    install -Dm644 $srcdir/$pkgname/apktool-toolbox.desktop "$pkgdir/usr/share/applications/apktool-toolbox.desktop"
    install -D -m644 $srcdir/$pkgname/LICENSE -t "${pkgdir}/usr/share/licenses/${pkgname}"
}
