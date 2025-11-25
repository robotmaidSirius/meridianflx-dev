/**
 * @file i_mrd_driver.hpp
 * @brief ドライバのためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_MODULES_I_MRD_DRIVER_HPP__
#define __MERIDIAN_MODULES_I_MRD_DRIVER_HPP__

#include "Meridim.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"

namespace meridian {
namespace modules {

/// @brief プラグインのためのインターフェイスクラス
class IMeridianDriver : public meridian::communication::MeridianDiagnosticUnit {
};

} // namespace modules
} // namespace meridian

using namespace meridian::modules;

#endif // __MERIDIAN_MODULES_I_MRD_DRIVER_HPP__
