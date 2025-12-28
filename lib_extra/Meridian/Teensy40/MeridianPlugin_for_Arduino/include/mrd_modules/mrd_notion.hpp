/**
 * @file mrd_notion.hpp
 * @brief
 * @version 0.1.0
 * @date 2025-11-30
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_MODULE_MRD_NOTION_HPP__
#define __MERIDIAN_MODULE_MRD_NOTION_HPP__
// ヘッダファイルの読み込み
#include <interface/i_mrd_driver.hpp>
// ライブラリ導入
// #include ...

namespace meridian {
namespace modules {

class MrdNotion : public IMrdDriver {
public:
  MrdNotion() : IMrdDriver(true, true) {}
  ~MrdNotion() {}
  /// @brief カテゴリー名
  virtual const char *get_category() { return "Template"; }
  /// @brief 区別させるための名前
  virtual const char *get_name() { return "Notion"; }

public:
  bool setup() override {
    return true;
  }

  bool input(Meridim &a_meridim) override {
    this->info("<Driver>[-> input] MrdNotion called");
    return true;
  }

  bool output(Meridim &a_meridim) override {
    this->info("<Driver>[<- output] MrdNotion called");
    return true;
  }
};

} // namespace modules
} // namespace meridian

#endif // __MERIDIAN_MODULE_MRD_NOTION_HPP__
