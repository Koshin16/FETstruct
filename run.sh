#!/bin/bash
set -e

echo "=== FETstruct ビルドスクリプト ==="

# OS検出とパッケージマネージャの判定
if command -v apt-get &> /dev/null; then
    echo "Debian/Ubuntu系を検出しました"
    sudo apt-get update
    sudo apt-get install -y build-essential freeglut3-dev mesa-common-dev wget
elif command -v yum &> /dev/null; then
    echo "RedHat/CentOS系を検出しました"
    sudo yum install -y gcc-c++ freeglut-devel mesa-libGL-devel wget
elif command -v brew &> /dev/null; then
    echo "macOS (Homebrew) を検出しました"
    brew install freeglut
else
    echo "エラー: サポートされていないOS、または必要なパッケージマネージャがみつかりません"
    echo "手動で以下をインストールしてください:"
    echo "  - build-essential (または gcc/g++)"
    echo "  - freeglut3-dev (または freeglut)"
    echo "  - mesa-common-dev (または libgl-dev)"
    exit 1
fi

# stb_image_write.h のダウンロード
if [ ! -f include/stb_image_write.h ]; then
    echo "include/stb_image_write.h をダウンロード中..."
    mkdir -p include
    if wget -O include/stb_image_write.h https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h; then
        echo "ダウンロード成功"
    else
        echo "警告: stb_image_write.h のダウンロードに失敗しました"
        echo "インターネット接続を確認してください"
        exit 1
    fi
fi

# ビルド
echo "ビルド中..."
make clean
if make; then
    echo "ビルド成功"
else
    echo "エラー: ビルドに失敗しました"
    exit 1
fi

# 実行
echo "プログラム実行中..."
./ofet
