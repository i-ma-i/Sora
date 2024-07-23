# Sora



## やらないこと
- マルチプラットフォーム
- マルチスレッド
- アニメーション
- ネットワーク通信
- 日本語対応
- 高度なデバイス入力処理
- 高度なオーディオ処理
- 高度なUI



## ライブラリ
- yaml-cpp
- magic-enum
- spdlog
- SDL2
- DirectX Tool Kit
- imgui



## アーキテクチャ

### Application: マルチスレッド非対応
・list<Module>
- Logger
- Keyboard
- Mouse
- Audio
- AssetRegistry
### Engine: マルチスレッド対応（要命名）
・list<Module>
- Graphics
- Physics
- Collision
### Actor
・list<Component>
- Transform
- Renderer
- Rigidbody
- Collider



## TODO
- デバッグカメラ
- シェーダー ライブラリのパッケージ化
- [非同期のアセット読み込み](https://learn.microsoft.com/ja-jp/windows/uwp/gaming/load-a-game-asset)
- プロファイリング
- DirectMesh
- FastNoise


## 
[McGuire](https://casual-effects.com/data/)