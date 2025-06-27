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

#include <stdafx.h>
#include "LoginUpController.h"
#include <service/login-up/LoginUpService.h>

StringJsonVO::Wrapper LoginUpController::execSendCode(const SendCodeDTO::Wrapper& dto) {
	// ���巵�����ݶ���
	auto jvo = StringJsonVO::createShared();

	// ����У��
	// �ǿ�У��
	if (dto->phone->empty() || dto->codeType->empty() || dto->ipAddress->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// ����һ��Service
	LoginUpService service;
	// ִ�����ݲ���
	if (service.sendCode(dto)) {
		jvo->success(ZH_WORDS_GETTER("loginup.sendcode.success"));
	}
	else {
		jvo->fail(ZH_WORDS_GETTER("loginup.sendcode.fail"));
	}
	// ��Ӧ���
	return jvo;
}

LoginJsonVO::Wrapper LoginUpController::execRegister(const RegisterDTO::Wrapper& dto) {
	// ���巵�����ݶ���
	auto jvo = LoginJsonVO::createShared();

	// ����У��
	// �ǿ�У��
	if (dto->phone->empty() || dto->code->empty() || dto->password->empty() || dto->nickname->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// ����һ��Service
	LoginUpService service;

	// ִ�����ݲ���
	auto result = service.registerUser(dto);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// ��Ӧ���
	return jvo;
}

LoginJsonVO::Wrapper LoginUpController::execLoginByCode(const LoginByCodeDTO::Wrapper& dto) {
	// ���巵�����ݶ���
	auto jvo = LoginJsonVO::createShared();

	// ����У��
	// �ǿ�У��
	if (dto->phone->empty() || dto->code->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// ����һ��Service
	LoginUpService service;
	// ִ�����ݲ���
	auto result = service.loginByCode(dto);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// ��Ӧ���
	return jvo;
}

LoginJsonVO::Wrapper LoginUpController::execLoginByPassword(const LoginByPasswordDTO::Wrapper& dto) {
	// ���巵�����ݶ���
	auto jvo = LoginJsonVO::createShared();

	// ����У��
	// �ǿ�У��
	if (dto->phone->empty() || dto->password->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// ����һ��Service
	LoginUpService service;
	// ִ�����ݲ���
	auto result = service.loginByPassword(dto);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// ��Ӧ���
	return jvo;
}

StringJsonVO::Wrapper LoginUpController::execLogout(const PayloadDTO& payload) {
	// ���巵�����ݶ���
	auto jvo = StringJsonVO::createShared();

	// ����һ��Service
	LoginUpService service;
	// ִ�����ݲ���
	if (service.logout(payload)) {
		jvo->success(ZH_WORDS_GETTER("loginup.logout.success"));
	}
	else {
		jvo->fail(ZH_WORDS_GETTER("loginup.logout.fail"));
	}
	// ��Ӧ���
	return jvo;
}

StringJsonVO::Wrapper LoginUpController::execResetPassword(const ResetPasswordDTO::Wrapper& dto) {
	// ���巵�����ݶ���
	auto jvo = StringJsonVO::createShared();

	// ����У��
	// �ǿ�У��
	if (dto->phone->empty() || dto->code->empty() || dto->newPassword->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// ����һ��Service
	LoginUpService service;
	// ִ�����ݲ���
	if (service.resetPassword(dto)) {
		jvo->success(ZH_WORDS_GETTER("loginup.resetpassword.success"));
	}
	else {
		jvo->fail(ZH_WORDS_GETTER("loginup.resetpassword.fail"));
	}
	// ��Ӧ���
	return jvo;
}

UserProfileJsonVO::Wrapper LoginUpController::execGetUserProfile(const PayloadDTO& payload) {
	// ���巵�����ݶ���
	auto jvo = UserProfileJsonVO::createShared();

	// ����һ��Service
	LoginUpService service;
	// ִ�����ݲ���
	auto result = service.getUserProfile(payload);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// ��Ӧ���
	return jvo;
}

RefreshTokenJsonVO::Wrapper LoginUpController::execRefreshToken(const RefreshTokenDTO::Wrapper& dto) {
	// ���巵�����ݶ���
	auto jvo = RefreshTokenJsonVO::createShared();

	// ����У��
	// �ǿ�У��
	if (dto->refreshToken->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// ����һ��Service
	LoginUpService service;
	// ִ�����ݲ���
	auto result = service.refreshToken(dto);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// ��Ӧ���
	return jvo;
}