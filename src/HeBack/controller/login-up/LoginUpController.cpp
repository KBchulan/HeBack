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
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();

	// 参数校验
	// 非空校验
	if (dto->phone->empty() || dto->codeType->empty() || dto->ipAddress->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 定义一个Service
	LoginUpService service;
	// 执行数据操作
	if (service.sendCode(dto)) {
		jvo->success(ZH_WORDS_GETTER("loginup.sendcode.success"));
	}
	else {
		jvo->fail(ZH_WORDS_GETTER("loginup.sendcode.fail"));
	}
	// 响应结果
	return jvo;
}

LoginJsonVO::Wrapper LoginUpController::execRegister(const RegisterDTO::Wrapper& dto) {
	// 定义返回数据对象
	auto jvo = LoginJsonVO::createShared();

	// 参数校验
	// 非空校验
	if (dto->phone->empty() || dto->code->empty() || dto->password->empty() || dto->nickname->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 定义一个Service
	LoginUpService service;

	// 执行数据操作
	auto result = service.registerUser(dto);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// 响应结果
	return jvo;
}

LoginJsonVO::Wrapper LoginUpController::execLoginByCode(const LoginByCodeDTO::Wrapper& dto) {
	// 定义返回数据对象
	auto jvo = LoginJsonVO::createShared();

	// 参数校验
	// 非空校验
	if (dto->phone->empty() || dto->code->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 定义一个Service
	LoginUpService service;
	// 执行数据操作
	auto result = service.loginByCode(dto);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// 响应结果
	return jvo;
}

LoginJsonVO::Wrapper LoginUpController::execLoginByPassword(const LoginByPasswordDTO::Wrapper& dto) {
	// 定义返回数据对象
	auto jvo = LoginJsonVO::createShared();

	// 参数校验
	// 非空校验
	if (dto->phone->empty() || dto->password->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 定义一个Service
	LoginUpService service;
	// 执行数据操作
	auto result = service.loginByPassword(dto);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// 响应结果
	return jvo;
}

StringJsonVO::Wrapper LoginUpController::execLogout(const PayloadDTO& payload) {
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();

	// 定义一个Service
	LoginUpService service;
	// 执行数据操作
	if (service.logout(payload)) {
		jvo->success(ZH_WORDS_GETTER("loginup.logout.success"));
	}
	else {
		jvo->fail(ZH_WORDS_GETTER("loginup.logout.fail"));
	}
	// 响应结果
	return jvo;
}

StringJsonVO::Wrapper LoginUpController::execResetPassword(const ResetPasswordDTO::Wrapper& dto) {
	// 定义返回数据对象
	auto jvo = StringJsonVO::createShared();

	// 参数校验
	// 非空校验
	if (dto->phone->empty() || dto->code->empty() || dto->newPassword->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 定义一个Service
	LoginUpService service;
	// 执行数据操作
	if (service.resetPassword(dto)) {
		jvo->success(ZH_WORDS_GETTER("loginup.resetpassword.success"));
	}
	else {
		jvo->fail(ZH_WORDS_GETTER("loginup.resetpassword.fail"));
	}
	// 响应结果
	return jvo;
}

UserProfileJsonVO::Wrapper LoginUpController::execGetUserProfile(const PayloadDTO& payload) {
	// 定义返回数据对象
	auto jvo = UserProfileJsonVO::createShared();

	// 定义一个Service
	LoginUpService service;
	// 执行数据操作
	auto result = service.getUserProfile(payload);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// 响应结果
	return jvo;
}

RefreshTokenJsonVO::Wrapper LoginUpController::execRefreshToken(const RefreshTokenDTO::Wrapper& dto) {
	// 定义返回数据对象
	auto jvo = RefreshTokenJsonVO::createShared();

	// 参数校验
	// 非空校验
	if (dto->refreshToken->empty()) {
		jvo->init(nullptr, RS_PARAMS_INVALID);
		return jvo;
	}

	// 定义一个Service
	LoginUpService service;
	// 执行数据操作
	auto result = service.refreshToken(dto);
	if (result != nullptr) {
		jvo->success(result);
	}
	else {
		jvo->fail(nullptr);
	}
	// 响应结果
	return jvo;
}