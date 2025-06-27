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
 * ��¼ע�����������ǰ��7�����Ľӿ�
 */
class LoginUpController : public oatpp::web::server::api::ApiController {
	API_ACCESS_DECLARE(LoginUpController);

public:
	// ==================== 1. ������֤��ӿ� ====================
	ENDPOINT_INFO(sendCode) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.sendcode.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		API_DEF_ADD_QUERY_PARAMS(String, "phone", ZH_WORDS_GETTER("loginup.field.phone"), "18737519552", true);
		API_DEF_ADD_QUERY_PARAMS(String, "codeType", ZH_WORDS_GETTER("loginup.field.codeType"), "LOGIN", true);
		API_DEF_ADD_QUERY_PARAMS(String, "ipAddress", ZH_WORDS_GETTER("loginup.field.ipAddress"), "127.0.0.1", true);
	}
	ENDPOINT(API_M_POST, "/api/auth/send-code", sendCode, QUERY(String, phone, "phone"), QUERY(String, codeType, "codeType"), QUERY(String, ipAddress, "ipAddress")) {
		auto dto = SendCodeDTO::createShared();
		dto->phone = phone;
		dto->codeType = codeType;
		dto->ipAddress = ipAddress;
		API_HANDLER_RESP_VO(execSendCode(dto));
	}

	// ==================== 2. �û�ע��ӿ� ====================
	ENDPOINT_INFO(registerUser) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.register.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(LoginJsonVO);
		API_DEF_ADD_QUERY_PARAMS(String, "phone", ZH_WORDS_GETTER("loginup.field.phone"), "18737519552", true);
		API_DEF_ADD_QUERY_PARAMS(String, "code", ZH_WORDS_GETTER("loginup.field.code"), "123456", true);
		API_DEF_ADD_QUERY_PARAMS(String, "password", ZH_WORDS_GETTER("loginup.field.password"), "123456", true);
		API_DEF_ADD_QUERY_PARAMS(String, "nickname", ZH_WORDS_GETTER("loginup.field.nickname"), "Chulan", true);
	}
	ENDPOINT(API_M_POST, "/api/auth/register", registerUser, QUERY(String, phone, "phone"), QUERY(String, code, "code"), QUERY(String, password, "password"), QUERY(String, nickname, "nickname")) {
		auto dto = RegisterDTO::createShared();
		dto->phone = phone;
		dto->code = code;
		dto->password = password;
		dto->nickname = nickname;
		API_HANDLER_RESP_VO(execRegister(dto));
	}

	// ==================== 3.1 �û���¼�ӿڣ���֤���¼�� ====================
	ENDPOINT_INFO(loginByCode) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.login-code.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(LoginJsonVO);
		API_DEF_ADD_QUERY_PARAMS(String, "phone", ZH_WORDS_GETTER("logiup.field.phone"), "18737519552", true);
		API_DEF_ADD_QUERY_PARAMS(String, "code", ZH_WORDS_GETTER("logiup.field.code"), "123456", true);
	}
	ENDPOINT(API_M_POST, "/api/auth/login-code", loginByCode, QUERY(String, phone, "phone"), QUERY(String, code, "code")) {
		auto dto = LoginByCodeDTO::createShared();
		dto->phone = phone;
		dto->code = code;
		API_HANDLER_RESP_VO(execLoginByCode(dto));
	}

	// ==================== 3.2 �û���¼�ӿڣ������¼�� ====================
	ENDPOINT_INFO(loginByPassword) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.login-password.summary"));
		API_DEF_ADD_RSP_JSON_WRAPPER(LoginJsonVO);
		API_DEF_ADD_QUERY_PARAMS(String, "phone", ZH_WORDS_GETTER("logiup.field.phone"), "18737519552", true);
		API_DEF_ADD_QUERY_PARAMS(String, "password", ZH_WORDS_GETTER("logiup.field.password"), "whx051021", true);
	}
	ENDPOINT(API_M_POST, "/api/auth/login-password", loginByPassword, QUERY(String, phone, "phone"), QUERY(String, password, "password")) {
		auto dto = LoginByPasswordDTO::createShared();
		dto->phone = phone;
		dto->password = password;
		API_HANDLER_RESP_VO(execLoginByPassword(dto));
	}

	// ==================== 4. �û��ǳ��ӿ� ====================
	ENDPOINT_INFO(logoutUser) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.logout.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
	}
	ENDPOINT(API_M_POST, "/api/auth/logout", logoutUser, API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execLogout(authObject->getPayload()));
	}

	// ==================== 5. ��������ӿ� ====================
	ENDPOINT_INFO(resetPassword) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.resetpassword.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(StringJsonVO);
		API_DEF_ADD_QUERY_PARAMS(String, "phone", ZH_WORDS_GETTER("logiup.field.phone"), "18737519552", true);
		API_DEF_ADD_QUERY_PARAMS(String, "code", ZH_WORDS_GETTER("logiup.field.code"), "123456", true);
		API_DEF_ADD_QUERY_PARAMS(String, "newPassword", ZH_WORDS_GETTER("loginup.field.newPassword"), "new123456", true);
	}
	ENDPOINT(API_M_POST, "/api/auth/reset-password", resetPassword, QUERY(String, phone, "phone"), QUERY(String, code, "code"), QUERY(String, newPassword, "newPassword"), API_HANDLER_AUTH_PARAME) {
		auto dto = ResetPasswordDTO::createShared();
		dto->phone = phone;
		dto->code = code;
		dto->newPassword = newPassword;
		API_HANDLER_RESP_VO(execResetPassword(dto));
	}

	// ==================== 6. ��ȡ�û���Ϣ�ӿ� ====================
	ENDPOINT_INFO(getUserProfile) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.profile.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(UserProfileJsonVO);
	}
	ENDPOINT(API_M_GET, "/api/user/profile", getUserProfile, API_HANDLER_AUTH_PARAME) {
		API_HANDLER_RESP_VO(execGetUserProfile(authObject->getPayload()));
	}

	// ==================== 7. ˢ��Token�ӿ� ====================
	ENDPOINT_INFO(refreshToken) {
		API_DEF_ADD_TITLE(ZH_WORDS_GETTER("loginup.refreshtoken.summary"));
		API_DEF_ADD_AUTH();
		API_DEF_ADD_RSP_JSON_WRAPPER(RefreshTokenJsonVO);
		API_DEF_ADD_QUERY_PARAMS(String, "refreshToken", ZH_WORDS_GETTER("loginup.field.refreshToken"), "aaaa", true);
	}
	ENDPOINT(API_M_POST, "/api/auth/refresh-token", refreshToken, QUERY(String, refreshTokenParam, "refreshToken"), API_HANDLER_AUTH_PARAME) {
		auto dto = RefreshTokenDTO::createShared();
		dto->refreshToken = refreshTokenParam;
		API_HANDLER_RESP_VO(execRefreshToken(dto));
	}


private:
	// ִ�з�����֤��
	StringJsonVO::Wrapper execSendCode(const SendCodeDTO::Wrapper& dto);

	// ִ���û�ע��
	LoginJsonVO::Wrapper execRegister(const RegisterDTO::Wrapper& dto);
	
	// ִ���û���¼ - ʹ����֤��
	LoginJsonVO::Wrapper execLoginByCode(const LoginByCodeDTO::Wrapper& loginData);

	// ִ���û���¼ - ʹ������
	LoginJsonVO::Wrapper execLoginByPassword(const LoginByPasswordDTO::Wrapper& loginData);
	
	// ִ���û��ǳ�
	StringJsonVO::Wrapper execLogout(const PayloadDTO& payload);
	
	// ִ����������
	StringJsonVO::Wrapper execResetPassword(const ResetPasswordDTO::Wrapper& dto);
	
	// ִ�л�ȡ�û���Ϣ
	UserProfileJsonVO::Wrapper execGetUserProfile(const PayloadDTO& payload);
	
	// ִ��ˢ��Token
	RefreshTokenJsonVO::Wrapper execRefreshToken(const RefreshTokenDTO::Wrapper& dto);

};

#include OATPP_CODEGEN_END(ApiController)

#endif // !LOGINUPCONTROLLER_H
