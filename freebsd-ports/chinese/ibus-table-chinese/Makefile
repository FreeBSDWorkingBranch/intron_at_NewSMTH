# $FreeBSD: tags/RELEASE_10_3_0/chinese/ibus-table-chinese/Makefile 367888 2014-09-10 20:50:31Z gerald $

PORTNAME=	ibus-table-chinese
PORTVERSION=	1.8.2
PORTREVISION=	2
CATEGORIES=	chinese

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Chinese input tables for IBus

LICENSE=	GPLv3
LICENSE_FILE=	${WRKSRC}/COPYING

BUILD_DEPENDS=	${LOCALBASE}/share/cmake/Modules/ManageEnvironment.cmake:devel/cmake-fedora \
		ibus-daemon:textproc/ibus \
		${LOCALBASE}/libexec/ibus-engine-table:textproc/ibus-table
RUN_DEPENDS=	ibus-daemon:textproc/ibus \
		${LOCALBASE}/libexec/ibus-engine-table:textproc/ibus-table

USES=		cmake:noninja pkgconfig

USE_GITHUB=	yes
GH_ACCOUNT=	definite

INSTALL_TARGET=	install
DATADIR=	${PREFIX}/share/ibus-table
CMAKE_ARGS+=	-DDATA_DIR:PATH="${PREFIX}/share" \
		-DPRJ_DOC_DIR:PATH="${DOCSDIR}" \
		-DCMAKE_FEDORA_TMP_DIR:PATH="${WRKSRC}"
MAKE_ENV+=	PYTHONDONTWRITEBYTECODE=1

.include <bsd.port.mk>
