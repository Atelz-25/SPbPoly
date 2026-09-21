#!/bin/bash
# Собирает переносимую папку с игрой для Windows: exe + все DLL + картинки.
# Запускать в MSYS2 MinGW 64-bit из каталога проекта:
#   ./collect-dlls.sh "Upgrade Original.exe" dist
set -e
EXE="${1:-Upgrade Original.exe}"
OUT="${2:-dist}"
MINGW="${MINGW_PREFIX:-/mingw64}"

[ -f "$EXE" ] || { echo "Не найден $EXE — сначала соберите игру"; exit 1; }
mkdir -p "$OUT"
cp "$EXE" "$OUT/"
cp -r Streets Chances "$OUT/"

echo "Копирую DLL..."
copy_deps() {
    ldd "$1" 2>/dev/null | grep -i "$MINGW" | awk '{print $3}' | while read -r dll; do
        [ -f "$dll" ] || continue
        name=$(basename "$dll")
        [ -f "$OUT/$name" ] && continue
        cp "$dll" "$OUT/"
        copy_deps "$dll"
    done
}
copy_deps "$EXE"

# загрузчики изображений и тема иконок — без них GTK не покажет картинки
if [ -d "$MINGW/lib/gdk-pixbuf-2.0" ]; then
    mkdir -p "$OUT/lib"
    cp -r "$MINGW/lib/gdk-pixbuf-2.0" "$OUT/lib/"
fi
if [ -d "$MINGW/share/icons/Adwaita" ]; then
    mkdir -p "$OUT/share/icons"
    cp -r "$MINGW/share/icons/Adwaita" "$OUT/share/icons/"
fi
if [ -d "$MINGW/share/glib-2.0/schemas" ]; then
    mkdir -p "$OUT/share/glib-2.0"
    cp -r "$MINGW/share/glib-2.0/schemas" "$OUT/share/glib-2.0/"
fi

echo "Готово. Переносимая папка: $OUT/ ($(du -sh "$OUT" | cut -f1))"
