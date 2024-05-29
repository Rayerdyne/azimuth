# Maintainer: François Straet <francois.straet at gmail dot com>
# Game author: Matthew D. Steele
pkgname=azimuth-git
pkgver=1.0.3
pkgrel=1
pkgdesc="A metroidvania with vector graphics by M. D. Steele"
arch=(i686 x86_64)
url="https://mdsteele.games/azimuth/"
license=('GNU GPL v3')
groups=()
depends=('sdl2' 'glibc')
makedepends=('git')
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("${pkgname%-git}-${pkgver}::git+https://github.com/Rayerdyne/azimuth#tag=v$pkgver")
noextract=()
sha256sums=('2d74d3176d05082ce8a2b5b251bac7db04df6dabe8ee7bcfc62dd748a3729ece')

pkgver() {
    cd "${srcdir}/${pkgname%-git}-${pkgver}"
    git describe --tags | sed 's/^v//;s/-/+/g'
}

prepare() {
    mkdir -p "${srcdir}/${pkgname%-git}-${pkgver}"
}

build() {
    cd "${srcdir}/${pkgname%-git}-${pkgver}"
    make BUILDTYPE=release 
}

check() {
    cd "${srcdir}/${pkgname%-git}-${pkgver}"
    make BUILDTYPE=release test
}

package() {
    cd "${srcdir}/${pkgname%-git}-${pkgver}"
    make BUILDTYPE=release DESTDIR="$pkgdir/" install
}
