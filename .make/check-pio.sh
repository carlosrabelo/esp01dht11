#!/usr/bin/env bash
set -euo pipefail

if command -v pio &>/dev/null; then
    exit 0
fi

PIPX_PIO="$HOME/.local/pipx/venvs/platformio/bin/pio"
if [ -f "$PIPX_PIO" ]; then
    exit 0
fi

PIO_DEFAULT="$HOME/.platformio/penv/bin/pio"
if [ -f "$PIO_DEFAULT" ]; then
    exit 0
fi

echo "PlatformIO not found. Run: make install-pio"
exit 1
