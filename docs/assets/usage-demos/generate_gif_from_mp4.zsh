#!/bin/env zsh

# --- Argument checks ---------------------------------------------------------

if [[ $# -ne 3 ]]; then
    echo "Usage: $0 SOURCE_VIDEO_FILE START_TIME END_TIME"
    echo "Example: $0 input.mp4 00:00:03 00:00:08"
    exit 1
fi

SOURCE_VIDEO_FILE=$1
EXT_START_TIME=$2
EXT_END_TIME=$3

if [[ ! -f "$SOURCE_VIDEO_FILE" ]]; then
    echo "Error: File not found: $SOURCE_VIDEO_FILE"
    exit 1
fi

# ------------------------------------------------------------------------------

SOURCE_BASENAME=$(basename "$SOURCE_VIDEO_FILE" .mp4)
PALETTE_FILE="${SOURCE_BASENAME}.png"
GIF_FILE="${SOURCE_BASENAME}.gif"

echo "------------------------------------------"
echo "Generating GIF from:"
echo "  File : $SOURCE_VIDEO_FILE"
echo "  Slice: $EXT_START_TIME → $EXT_END_TIME"
echo "------------------------------------------"
echo ""

# --- Step 1: Generate palette --------------------------------------------------

echo "[1/2] Creating palette..."
ffmpeg -y -ss "$EXT_START_TIME" -to "$EXT_END_TIME" \
  -i "$SOURCE_VIDEO_FILE" \
  -vf "fps=15,scale=960:-1:flags=lanczos,palettegen" \
  "$PALETTE_FILE"

if [[ $? -ne 0 ]]; then
    echo "Error: Failed to generate palette."
    exit 1
fi

# --- Step 2: Generate GIF ------------------------------------------------------

echo "[2/2] Creating GIF..."
ffmpeg -y -ss "$EXT_START_TIME" -to "$EXT_END_TIME" \
  -i "$SOURCE_VIDEO_FILE" -i "$PALETTE_FILE" \
  -filter_complex "fps=15,scale=960:-1:flags=lanczos[x];[x][1:v]paletteuse" \
  "$GIF_FILE"

if [[ $? -ne 0 ]]; then
    echo "Error: Failed to generate GIF."
    exit 1
fi

echo ""
echo "------------------------------------------"
echo "Done!"
echo "Generated:"
echo "  Palette : $PALETTE_FILE"
echo "  GIF     : $GIF_FILE"
echo "------------------------------------------"
