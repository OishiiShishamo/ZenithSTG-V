# ZenithSTG-V

[English](README.md) | **日本語**

弾幕STGエンジンの実験的実装。C++20 / Vulkan / SDL3 製。

**16,384発の弾を同時展開してもほぼ処理落ちなし。** GPU側で弾の座標計算・描画を一括処理するVulkanネイティブアーキテクチャにより、CPU負荷を最小限に抑えながら超高密度な弾幕を実現しています。

---

## 特徴

- **超高密度弾幕** — 16,384発同時展開時もフレームレートへの影響は軽微
- **Vulkanネイティブ** — 低レベルGraphics APIを直接利用し、ドライバオーバーヘッドを排除
- **C++20** — コンセプト・レンジなどモダンな言語機能を活用したクリーンな実装
- **GLSL シェーダ** — 弾の描画処理をGPU側に委譲し、大量スプライトを効率的にバッチ処理
- **SDL3** — クロスプラットフォームなウィンドウ管理・入力処理

---

## 依存関係

| ライブラリ | 用途 |
|---|---|
| Vulkan SDK | グラフィックスバックエンド |
| SDL3 | ウィンドウ / 入力 |
| GLM | 数学ライブラリ (ベクトル・行列) |
| glslc (Vulkan SDK同梱) | GLSLシェーダのコンパイル |

---

## ビルド方法

### 必要なもの

- CMake 3.20 以上
- C++20 対応コンパイラ (GCC 12+ / Clang 14+ / MSVC 2022+)
- Vulkan SDK
- SDL3
- GLM

### Linux / macOS

```sh
# シェーダのコンパイル
./shader_compile.sh

# ビルド
./build.sh
```

または手動で:

```sh
cmake -B build
cmake --build build
```

### Windows (MSVC)

```sh
cmake -B build
cmake --build build --config Release
```

ビルド後、`shaders/` と `textures/` が実行ファイルと同じディレクトリに自動でコピーされます。

---

## ディレクトリ構成

```
ZenithSTG-V/
├── src/          # C++ ソースコード
├── include/      # ヘッダファイル
├── shaders/      # GLSL シェーダ
├── textures/     # テクスチャアセット
├── CMakeLists.txt
├── build.sh      # ビルドスクリプト
└── shader_compile.sh  # シェーダコンパイルスクリプト
```

---

## ライセンス

MIT License — 詳細は [LICENSE](LICENSE) を参照。
