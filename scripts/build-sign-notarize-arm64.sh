#!/usr/bin/env bash
# Build traaitt for Apple Silicon (arm64), code-sign, and notarize.
#
# Usage:
#   ./scripts/build-sign-notarize-arm64.sh
#
# Required environment variables (or you will be prompted):
#   APPLE_ID          – your Apple ID email
#   APPLE_ID_PASSWORD – app-specific password from appleid.apple.com
#
# The Developer ID Application certificate and Team ID are auto-detected
# from your login keychain.

set -euo pipefail

# ── Paths ─────────────────────────────────────────────────────────────────────
REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$REPO_ROOT/build-arm64"
DIST_DIR="$REPO_ROOT/dist-arm64"
ZIP_NAME="traaitt-macos-arm64.zip"

# ── Signing identity ──────────────────────────────────────────────────────────
CERT_NAME="Developer ID Application: Christian Benz (MC52ZBYMUT)"
TEAM_ID="MC52ZBYMUT"

# ── Binaries produced by src/CMakeLists.txt ───────────────────────────────────
BINARIES=(
    XTEnetwork
    "traaittwallet++"
    cmdwalletbeta
    XTEservice
    XTEwalletapi
    MINER
    walletupgrader
    cryptotest
)

# ── Helpers ───────────────────────────────────────────────────────────────────
info()  { printf '\033[1;34m==> %s\033[0m\n' "$*"; }
ok()    { printf '\033[1;32m  ✓ %s\033[0m\n' "$*"; }
die()   { printf '\033[1;31mERROR: %s\033[0m\n' "$*" >&2; exit 1; }

# ── Preflight checks ──────────────────────────────────────────────────────────
info "Checking prerequisites"

[[ "$(uname -m)" == "arm64" ]] || die "This script must run on an Apple Silicon Mac (arm64)."

command -v cmake  &>/dev/null || die "cmake not found. Install with: brew install cmake"
command -v xcrun  &>/dev/null || die "xcrun not found – is Xcode installed?"
command -v codesign &>/dev/null || die "codesign not found – is Xcode installed?"

# Verify the signing certificate is present and valid
security find-identity -v -p codesigning | grep -qF "$CERT_NAME" \
    || die "Certificate not found in keychain: $CERT_NAME"

# Install ninja if missing
if ! command -v ninja &>/dev/null; then
    info "Installing ninja via Homebrew"
    brew install ninja
fi

# Collect notarization credentials
if [[ -z "${APPLE_ID:-}" ]]; then
    read -rp "Apple ID (email): " APPLE_ID
fi
if [[ -z "${APPLE_ID_PASSWORD:-}" ]]; then
    read -rsp "App-specific password: " APPLE_ID_PASSWORD
    echo
fi

[[ -n "$APPLE_ID" ]]          || die "APPLE_ID is required."
[[ -n "$APPLE_ID_PASSWORD" ]] || die "APPLE_ID_PASSWORD is required."

# ── Build ─────────────────────────────────────────────────────────────────────
info "Configuring CMake (arm64, Release)"
cmake -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DARCH=default \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
    -S "$REPO_ROOT"

info "Building ($(sysctl -n hw.logicalcpu) cores)"
cmake --build "$BUILD_DIR" --parallel

# ── Verify architecture ───────────────────────────────────────────────────────
info "Verifying arm64 architecture"
for name in "${BINARIES[@]}"; do
    bin="$BUILD_DIR/src/$name"
    if [[ -f "$bin" ]]; then
        arch_out=$(lipo -archs "$bin" 2>/dev/null || file "$bin")
        echo "  $name: $arch_out"
    fi
done

# ── Code sign ─────────────────────────────────────────────────────────────────
info "Code signing with: $CERT_NAME"
for name in "${BINARIES[@]}"; do
    bin="$BUILD_DIR/src/$name"
    if [[ -f "$bin" ]]; then
        codesign \
            --force \
            --options runtime \
            --sign "$CERT_NAME" \
            --timestamp \
            "$bin"
        codesign --verify --verbose=1 "$bin" && ok "Signed: $name"
    else
        echo "  (skipped – not built: $name)"
    fi
done

# ── Package ───────────────────────────────────────────────────────────────────
info "Packaging binaries"
rm -rf "$DIST_DIR"
mkdir -p "$DIST_DIR"

for name in "${BINARIES[@]}"; do
    bin="$BUILD_DIR/src/$name"
    [[ -f "$bin" ]] && cp "$bin" "$DIST_DIR/"
done

cd "$REPO_ROOT"
rm -f "$ZIP_NAME"
ditto -c -k --sequesterRsrc --keepParent "$DIST_DIR" "$ZIP_NAME"
ok "Package: $REPO_ROOT/$ZIP_NAME"

# ── Notarize ──────────────────────────────────────────────────────────────────
info "Submitting to Apple notarization service (this may take a few minutes)"
SUBMIT_OUT=$(xcrun notarytool submit "$ZIP_NAME" \
    --apple-id    "$APPLE_ID" \
    --password    "$APPLE_ID_PASSWORD" \
    --team-id     "$TEAM_ID" \
    --wait \
    2>&1)

echo "$SUBMIT_OUT"

# Check result
if echo "$SUBMIT_OUT" | grep -q "status: Accepted"; then
    ok "Notarization accepted"
elif echo "$SUBMIT_OUT" | grep -q "status: Invalid"; then
    # Print the full log for diagnostics
    SUBMISSION_ID=$(echo "$SUBMIT_OUT" | grep -oE 'id: [0-9a-f-]+' | head -1 | awk '{print $2}')
    if [[ -n "$SUBMISSION_ID" ]]; then
        info "Fetching notarization log for submission $SUBMISSION_ID"
        xcrun notarytool log "$SUBMISSION_ID" \
            --apple-id    "$APPLE_ID" \
            --password    "$APPLE_ID_PASSWORD" \
            --team-id     "$TEAM_ID" || true
    fi
    die "Notarization failed – see log above."
fi

# ── Staple ────────────────────────────────────────────────────────────────────
info "Stapling notarization tickets"
for name in "${BINARIES[@]}"; do
    bin="$DIST_DIR/$name"
    if [[ -f "$bin" ]]; then
        xcrun stapler staple "$bin" && ok "Stapled: $name"
    fi
done

# Re-package with stapled binaries
rm -f "$ZIP_NAME"
ditto -c -k --sequesterRsrc --keepParent "$DIST_DIR" "$ZIP_NAME"

# ── Done ──────────────────────────────────────────────────────────────────────
info "Done"
echo ""
echo "  Signed + notarized package: $REPO_ROOT/$ZIP_NAME"
echo "  Individual binaries:        $DIST_DIR/"
echo ""
echo "  Verify on another Mac with:"
echo "    spctl -a -vvv -t install $DIST_DIR/XTEnetwork"
