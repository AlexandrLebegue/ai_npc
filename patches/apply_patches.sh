#!/usr/bin/env bash
# ============================================================
#  Apply PS Vita patches to the OpenFarCry1 engine source.
#  Called once by build_vita.sh before the first cmake run.
# ============================================================
set -e
FARCRY_ROOT="$(cd "$(dirname "$0")/../farcry" && pwd)"
PATCHES_DIR="$(cd "$(dirname "$0")" && pwd)"

apply() {
    local patch="$1"
    echo "[PATCH] Applying ${patch} ..."
    patch -d "${FARCRY_ROOT}" -p1 < "${PATCHES_DIR}/${patch}"
}

apply "platform_vita.patch"
apply "system_win32_stub.patch"

echo "[PATCH] All patches applied."
