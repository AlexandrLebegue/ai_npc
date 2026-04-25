#!/usr/bin/env bash
# ============================================================
#  Far Cry 1 — PS Vita Port  —  build script
#  Usage:
#    ./build_vita.sh            # Release build
#    ./build_vita.sh debug      # Debug build
#    ./build_vita.sh clean      # Remove build directory
# ============================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build_vita"
BUILD_TYPE="${1:-Release}"

# ── Sanity check ─────────────────────────────────────────────────────────
if [ -z "${VITASDK}" ]; then
    echo "[ERROR] VITASDK environment variable is not set."
    echo "        Install vitaSDK: https://vitasdk.org/"
    echo "        Then: export VITASDK=/usr/local/vitasdk"
    echo "              export PATH=\$VITASDK/bin:\$PATH"
    exit 1
fi

if ! command -v arm-vita-eabi-gcc &>/dev/null; then
    echo "[ERROR] arm-vita-eabi-gcc not found in PATH."
    echo "        Make sure \$VITASDK/bin is in your PATH."
    exit 1
fi

if [ "$1" = "clean" ]; then
    echo "[INFO] Removing ${BUILD_DIR} ..."
    rm -rf "${BUILD_DIR}"
    echo "[INFO] Done."
    exit 0
fi

# ── Apply engine patches if not already patched ───────────────────────────
PATCH_MARKER="${SCRIPT_DIR}/farcry/.vita_patched"
if [ ! -f "${PATCH_MARKER}" ]; then
    echo "[INFO] Applying PS Vita patches to engine source ..."
    bash "${SCRIPT_DIR}/patches/apply_patches.sh"
    touch "${PATCH_MARKER}"
fi

# ── Configure ────────────────────────────────────────────────────────────
echo "[INFO] Configuring  (${BUILD_TYPE}) ..."
cmake -S "${SCRIPT_DIR}" \
      -B "${BUILD_DIR}" \
      -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
      -DCMAKE_TOOLCHAIN_FILE="${VITASDK}/share/vita.toolchain.cmake"

# ── Build ────────────────────────────────────────────────────────────────
echo "[INFO] Building ..."
cmake --build "${BUILD_DIR}" -- -j"$(nproc)"

# ── Report output ────────────────────────────────────────────────────────
VPK="${BUILD_DIR}/FarCry.vpk"
if [ -f "${VPK}" ]; then
    echo ""
    echo "╔══════════════════════════════════════╗"
    echo "║  Build successful!                   ║"
    echo "║  Output: ${VPK}"
    echo "║                                      ║"
    echo "║  Install with VitaShell or Vitá:     ║"
    echo "║    ftp your VPK to the Vita and      ║"
    echo "║    install via VitaShell             ║"
    echo "╚══════════════════════════════════════╝"
else
    echo "[WARNING] Build finished but FarCry.vpk not found."
    echo "          Check cmake output above for errors."
fi
