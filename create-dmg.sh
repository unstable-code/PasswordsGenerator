#!/bin/bash
# macOS DMG 생성 스크립트

APP_NAME="PasswordGenerator"
APP_PATH="$1"
OUTPUT_DMG="$2"
VOLUME_NAME="Password Generator"

if [ ! -d "$APP_PATH" ]; then
    echo "Error: App bundle not found at $APP_PATH"
    exit 1
fi

# 임시 DMG 생성
TMP_DMG="tmp_$APP_NAME.dmg"
rm -f "$TMP_DMG" "$OUTPUT_DMG"

# DMG 크기 계산 (app 크기 + 여유공간)
APP_SIZE=$(du -sm "$APP_PATH" | cut -f1)
DMG_SIZE=$((APP_SIZE + 50))

# 빈 DMG 생성
hdiutil create -size ${DMG_SIZE}m -fs HFS+ -volname "$VOLUME_NAME" "$TMP_DMG"

# DMG 마운트
MOUNT_DIR=$(hdiutil attach "$TMP_DMG" | grep Volumes | awk '{print $3}')

# 앱 복사
cp -R "$APP_PATH" "$MOUNT_DIR/"

# Applications 폴더 심볼릭 링크 생성
ln -s /Applications "$MOUNT_DIR/Applications"

# DMG 언마운트
hdiutil detach "$MOUNT_DIR"

# 최종 압축된 DMG 생성
hdiutil convert "$TMP_DMG" -format UDZO -o "$OUTPUT_DMG"

# 임시 파일 삭제
rm -f "$TMP_DMG"

echo "DMG created: $OUTPUT_DMG"
