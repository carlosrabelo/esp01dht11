#!/usr/bin/env bash
set -euo pipefail

# Add default PlatformIO install path to PATH if pio is not in PATH
PIO_DEFAULT="$HOME/.platformio/penv/bin/pio"
if [ -f "$PIO_DEFAULT" ]; then
    export PATH="$(dirname "$PIO_DEFAULT"):$PATH"
fi

PIPX_PIO="$HOME/.local/pipx/venvs/platformio/bin/pio"
if [ -f "$PIPX_PIO" ]; then
    export PATH="$(dirname "$PIPX_PIO"):$PATH"
fi

exec pio "$@"
