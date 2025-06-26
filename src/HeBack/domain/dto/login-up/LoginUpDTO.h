#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: Chulan
 @Date: 2025/06/26 13:09:32

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

	  https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#ifndef LOGINUP_DTO_H
#define LOGINUP_DTO_H

#include <domain/GlobalInclude.h>

#include OATPP_CODEGEN_BEGIN(DTO)

/**
 * 1. 发送验证码请求DTO
 */
class SendCodeDTO : public oatpp::DTO {
	DTO_INIT(SendCodeDTO, DTO);

	// 手机号码
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// 验证码类型
	DTO_FIELD(String, codeType);
	DTO_FIELD_INFO(codeType) {
		info->description = ZH_WORDS_GETTER("loginup.field.codeType");
	}
};

/**
 * 2. 用户注册DTO
 */
class RegisterDTO : public oatpp::DTO {
	DTO_INIT(RegisterDTO, DTO);

	// 手机号码
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// 验证码
	DTO_FIELD(String, code);
	DTO_FIELD_INFO(code) {
		info->description = ZH_WORDS_GETTER("loginup.field.code");
	}

	// 密码
	DTO_FIELD(String, password);
	DTO_FIELD_INFO(password) {
		info->description = ZH_WORDS_GETTER("loginup.field.password");
	}

	// 用户昵称（可选）
	DTO_FIELD(String, nickname);
	DTO_FIELD_INFO(nickname) {
		info->description = ZH_WORDS_GETTER("loginup.field.nickname");
	}
};

/**
 * 3.1 用户登录DTO - 验证码登录
 */
class LoginByCodeDTO : public oatpp::DTO {
	DTO_INIT(LoginByCodeDTO, DTO);

	// 手机号码
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// 验证码
	DTO_FIELD(String, code);
	DTO_FIELD_INFO(code) {
		info->description = ZH_WORDS_GETTER("loginup.field.code");
	}
};

/**
 * 3.2 用户登录DTO - 密码登录
 */
class LoginByPasswordDTO : public oatpp::DTO {
	DTO_INIT(LoginByPasswordDTO, DTO);

	// 手机号码
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// 密码
	DTO_FIELD(String, password);
	DTO_FIELD_INFO(password) {
		info->description = ZH_WORDS_GETTER("loginup.field.password");
	}
};

/**
 * 4 用户登出DTO
 */
class LogoutDTO : public oatpp::DTO {
	DTO_INIT(LogoutDTO, DTO);

	// 访问令牌
	DTO_FIELD(String, accessToken);
	DTO_FIELD_INFO(accessToken) {
		info->description = ZH_WORDS_GETTER("loginup.field.accessToken");
	}
};

/**
 * 5 重置密码DTO
 */
class ResetPasswordDTO : public oatpp::DTO {
	DTO_INIT(ResetPasswordDTO, DTO);

	// 手机号码
	DTO_FIELD(String, phone);
	DTO_FIELD_INFO(phone) {
		info->description = ZH_WORDS_GETTER("loginup.field.phone");
	}

	// 验证码
	DTO_FIELD(String, code);
	DTO_FIELD_INFO(code) {
		info->description = ZH_WORDS_GETTER("loginup.field.code");
	}

	// 新密码
	DTO_FIELD(String, newPassword);
	DTO_FIELD_INFO(newPassword) {
		info->description = ZH_WORDS_GETTER("loginup.field.newPassword");
	}
};

/**
 * 6 刷新Token DTO
 */
class RefreshTokenDTO : public oatpp::DTO {
	DTO_INIT(RefreshTokenDTO, DTO);

	// 刷新令牌
	DTO_FIELD(String, refreshToken);
	DTO_FIELD_INFO(refreshToken) {
		info->description = ZH_WORDS_GETTER("loginup.field.refreshToken");
	}
};

#include OATPP_CODEGEN_END(DTO)

#endif // !LOGINUP_DTO_H
