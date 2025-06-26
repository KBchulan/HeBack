#pragma once

/*
 Copyright Zero One Star. All rights reserved.

 @Author: Chulan
 @Date: 2025/06/24 13:04:32

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

#ifndef LOGINUPCONTROLLER_H
#define LOGINUPCONTROLLER_H


#include <domain/vo/BaseJsonVO.h>
#include <domain/dto/login-up/LoginUpDTO.h>
#include <domain/vo/login-up/LoginUpVO.h>

#include OATPP_CODEGEN_BEGIN(ApiController)

/*
 * 登录注册控制器，当前共7个核心接口
 */
class LoginUpController : public oatpp::web::server::api::ApiController {
	API_ACCESS_DECLARE(LoginUpController);

public:
	// ==================== 1. 发送验证码接口 ====================
	ENDPOINT_INFO(sendCode) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.sendcode.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
	}
	ENDPOINT(API_M_POST, "/api/auth/send-code", sendCode, BODY_DTO(SendCodeDTO::Wrapper, dto)) {
		API_HANDLER_RESP_VO(execSendCode(dto));
	}

	// ==================== 2. 用户注册接口 ====================
	ENDPOINT_INFO(registerUser) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.register.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(LoginJsonVO);
	}
	ENDPOINT(API_M_POST, "/api/auth/register", registerUser, BODY_DTO(RegisterDTO::Wrapper, dto)) {
		API_HANDLER_RESP_VO(execRegister(dto));
	}

	// ==================== 3.1 用户登录接口（密码登录） ====================
	ENDPOINT_INFO(loginByPassword) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.login-password.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(LoginJsonVO);
	}
	ENDPOINT(API_M_POST, "/api/auth/login-password", loginByPassword, BODY_DTO(LoginByPasswordDTO::Wrapper, dto)) {
		API_HANDLER_RESP_VO(execLoginByPassword(dto));
	}

	// ==================== 3.2 用户登录接口（验证码登录） ====================
	ENDPOINT_INFO(loginByPassword) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.login-code.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(LoginJsonVO);
	}
	ENDPOINT(API_M_POST, "/api/auth/login-code", loginByPassword, BODY_DTO(LoginByCodeDTO::Wrapper, dto)) {
		API_HANDLER_RESP_VO(execLoginByPassword(dto));
	}

	// ==================== 4. 用户登出接口 ====================
	ENDPOINT_INFO(logoutUser) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.logout.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
	}
	ENDPOINT(API_M_POST, "/api/auth/logout", logoutUser, API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execLogout(authObject->getPayload()));
	}

	// ==================== 5. 重置密码接口 ====================
	ENDPOINT_INFO(resetPassword) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.resetpassword.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
	}
	ENDPOINT(API_M_POST, "/api/auth/reset-password", resetPassword, BODY_DTO(ResetPasswordDTO::Wrapper, dto)) {
		API_HANDLER_RESP_VO(execResetPassword(dto));
	}

	// ==================== 6. 获取用户信息接口 ====================
	ENDPOINT_INFO(getUserProfile) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.profile.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(UserProfileJsonVO);
	}
	ENDPOINT(API_M_GET, "/api/user/profile", getUserProfile, API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execGetUserProfile(authObject->getPayload()));
	}

	// ==================== 7. 刷新Token接口 ====================
	ENDPOINT_INFO(refreshToken) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.refreshtoken.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(RefreshTokenJsonVO);
	}
	ENDPOINT(API_M_POST, "/api/auth/refresh-token", refreshToken, BODY_DTO(RefreshTokenDTO::Wrapper, dto)) {
		API_HANDLER_RESP_VO(execRefreshToken(dto));
	}


private:
	// 执行发送验证码
	StringJsonVO::Wrapper execSendCode(const SendCodeDTO::Wrapper& dto);

	// 执行用户注册
	LoginJsonVO::Wrapper execRegister(const RegisterDTO::Wrapper& dto);
	
	// 执行用户登录
	LoginJsonVO::Wrapper execLoginByPassword(const LoginByPasswordDTO::Wrapper& loginData);
	
	// 执行用户登出
	StringJsonVO::Wrapper execLogout(const PayloadDTO& payload);
	
	// 执行重置密码
	StringJsonVO::Wrapper execResetPassword(const ResetPasswordDTO::Wrapper& dto);
	
	// 执行获取用户信息
	UserProfileJsonVO::Wrapper execGetUserProfile(const PayloadDTO& payload);
	
	// 执行刷新Token
	RefreshTokenJsonVO::Wrapper execRefreshToken(const RefreshTokenDTO::Wrapper& dto);

};

#include OATPP_CODEGEN_END(ApiController)

#endif // !LOGINUPCONTROLLER_H
