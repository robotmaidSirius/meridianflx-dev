# MeridianCore

MeridianCoreは、MeridianFLXフレームワークの中心的なコンポーネントであり、ロボット制御システムの基盤を提供します。
このライブラリは、仮想クラスやインターフェースを通じて、さまざまなハードウェアプラットフォームに対応するための抽象化レイヤーを提供します。

## インターフェイス

### 1. IMrdApp

Meridianアプリケーションを作成するための基本的なインターフェースを提供します。

| --        | --                                       |
| --------- | ---------------------------------------- |
| #include  | ```#include "interface/i_mrd_app.hpp"``` |
| namespace | ```namespace meridian:app;"```           |
| クラス名  | IMrdApp                                  |

継承したクラスは、アプリケーションの基本的なライフサイクルを管理するために、
下記の関数を実装する必要があります。

| 関数名         | 説明                                       |
| -------------- | ------------------------------------------ |
| `getAppName()` | アプリケーションの名前を取得する           |
| `setup()`      | アプリケーションの初期化処理を行う         |
| `loop()`       | メインループで繰り返し実行される処理を行う |
| `closing()`    | アプリケーションの終了処理を行う           |

```cpp
class TestApp : public meridian::app::IMrdApp {
public:
  /// @brief アプリケーション名前
   const char *get_name() override { return "Unknown"; }

protected:
  /// @brief アプリケーションの初期化
   bool setup(Meridim &a_meridim) override{return true;};
  /// @brief アプリケーションの実処理
   bool loop(Meridim &a_meridim) override{return true;};
  /// @brief アプリケーションの終了処理
   bool closing(Meridim &a_meridim) override { return true; };
};
```

### 2. IMrdBoard

### 3. IMrdDriver

### 4. IMrdConversation

### 5. IMrdDiagnostic
