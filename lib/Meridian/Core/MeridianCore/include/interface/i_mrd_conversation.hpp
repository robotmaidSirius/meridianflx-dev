/**
 * @file i_mrd_conversation.hpp
 * @brief 通信のためのインターフェイスクラス
 * @version 1.0.0
 * @date 2025-04-20
 * @copyright Copyright (c) 2025 by Meridian Team. All rights reserved.
 * @note MIT LICENSE
 */
#ifndef __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_CONVERSATION_HPP__
#define __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_CONVERSATION_HPP__

#include "Meridim.hpp"
#include "mrd_utils/meridian_diagnostic_unit.hpp"

#include <string.h>

namespace meridian {
namespace communication {

/// @brief 通信のためのインターフェイスクラス
/// @note Meridianの通信はこのクラスを継承して実装すること。
class IMeridianConversation : public MeridianDiagnosticUnit {
};

} // namespace communication
} // namespace meridian

#endif // __MERIDIAN_CORE_COMMUNICATION_I_MRD_COM_CONVERSATION_HPP__
